 /**
 * @file uart.c
 * @brief implementation of uart.h
 *
 * this file implements uart.h, implementing interrupt functions for the
 * UART communication system
 *
 * @author Seth Miers and Jake Cazden
 * @date March 04, 2018
 *
 */
#include "uart.h"

#include "circbuf.h" /* so we can use the circular buffer in our uart */

#include<stdlib.h>
#if defined(PROJECT4)
#include "logger.h"
#include "logger_queue.h"
volatile uint8_t step;
volatile uint8_t* dataptr;
volatile uint16_t datacounter;
#endif

#ifdef KL25Z
#include "MKL25Z4.h"
#endif

extern CB_t* recieve_buffer;
#if defined(PROJECT1)||defined(PROJECT2)||defined(PROJECT3)
extern CB_t* transmit_buffer;
#endif
#if defined(PROJECT4)
extern LQ_t* log_buffer;
extern log_t* activeTransfer;
#endif

UART_e UART_configure()
{

    /* set PTE0/PTE20/PTA2/PTA14/PTB17/PTD7 to UART1_TX (i think it should be UART0)*/
    /* set PTE1/PTE21/PTA1/PTA15/PTB16/PTD6 to UART1_RX */

    /*turn on PORTA clocking with SIM_SCGC5 bit 9  (PORTA) (this may be active already)
     * thereby allowing pin manipulation*/
    SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;

    /*set PTA1 to ALT2, PTA2 to ALT2 using PORTA_PCR1, PORTA_PCR2
     * PORTA_PCR1[IRQC] = 0000; - no innterrupt from the pins directly
     * PORTA_PCR2[IRQC] = 0000; - no innterrupt from the pins directly
     * PORTA_PCR1[MUX] = 010; - ALT 2
     * PORTA_PCR2[MUX] = 010; - ALT 2
     * thereby clearing the lines for UART0*/
    PORTA_PCR1 =    PORT_PCR_ISF(CLEAR_PCR_ISF)
                    |PORT_PCR_IRQC(DISABLE_PCR_IRQC)
                    |PORT_PCR_MUX(PCR_MUX_ALT2);

    PORTA_PCR2 =    PORT_PCR_ISF(CLEAR_PCR_ISF)
                    |PORT_PCR_IRQC(DISABLE_PCR_IRQC)
                    |PORT_PCR_MUX(PCR_MUX_ALT2);
    /*set UART0 clock source with SIM_SOPT2
     * pins 27-26 (UART0SRC) = 01 for FLL select
     * pin 16 (PLLFLLSEL) = 0 for MCGFLLCLK (no division by 2)*/
    SIM_SOPT2 &= ~SIM_SOPT2_UART0SRC(SIM_SOPT2_UART0SRC_CLEAR);/*clear UART0SRC bits*/
    SIM_SOPT2 |=  SIM_SOPT2_UART0SRC(SIM_SOPT2_UART0SRC_MCGIRCLK);/*set  UART0SRC bits*/

    /*SIM_SOPT2 &= ~SIM_SOPT2_PLLFLLSEL(SIM_SOPT2_PLLFLLSEL_CLEAR); clear PLLFLLSEL*/
    /*SIM_SOPT2 |=  SIM_SOPT2_PLLFLLSEL(SIM_SOPT2_PLLFLLSEL_FLLSRC); set PLLFLLSEL to 0*/
    MCG_C1 |= MCG_C1_IRCLKEN(MCG_C1_IRCLKEN_ENABLED);
    MCG_C2 |= MCG_C2_IRCS(MCG_C2_IRCS_FAST);
    MCG_SC &= ~MCG_SC_FCRDIV(MCG_C2_FCRDIV_CLEAR);
    MCG_SC |= MCG_SC_FCRDIV(MCG_C2_FCRDIV_NODIVISION);
    /*choose UART0 tx and rx source with SIM_SOPT5
     * pin 2 = 0 for UART0_RX pin (UART0RXSRC)
     * pin 1-0 = 00 for UART_TX pin (UART0TXSRC)*/
    SIM_SOPT5 &= ~SIM_SOPT5_UART0RXSRC(SIM_SOPT5_UART0RXSRC_CLEAR);/*clear UART0RXSRC bits*/
    SIM_SOPT5 |=  SIM_SOPT5_UART0RXSRC(SIM_SOPT5_UART0RXSRC_RXPIN);/*set  UART0RXSRC bits to 0*/

    SIM_SOPT5 &= ~SIM_SOPT5_UART0TXSRC(SIM_SOPT5_UART0TXSRC_CLEAR);/*clear UART0TXSRC*/
    SIM_SOPT5 |=  SIM_SOPT5_UART0TXSRC(SIM_SOPT5_UART0TXSRC_TXPIN);/*set UART0TXSRC to 0*/

    /*enable clock to UART0 with SIM_SCGC4
     * pin 10 (UART0) =1 to activate clock gate*/
    SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;/*set UART0 to recieve clocking*/

    UART0_C4 |= UART0_C4_OSR(UART0_C4_OSR_SAMPLERATE);

    /*set uart0 baud rate high register
     * LBKDIE = 0, we arent using this interrupt
     * RXEDGIE = 0, we arent using this interrupt
     * SBNS = 0, 1 stop bit desired
     * SBR = (5 highest bits of baud rate)*/
    UART0_BDH = UART0_BDH_LBKDIE(UART0_BDH_LBKDIE_DISABLE)
                |UART0_BDH_RXEDGIE(UART0_BDH_RXEDGIE_DISABLE)
                |UART0_BDH_SBNS(UART0_BDH_SBNS_SINGLESTOPBIT)
                |UART0_BDH_SBR((CALCULATED_BAUD_MASK & SBR_HIGHMASK)>>UART0_BDL_SBR_WIDTH);

    /*set uart0 baud rate low register
     * SBR = (8 lower bits of baud rate)*/
    UART0_BDL = UART0_BDL_SBR(CALCULATED_BAUD_MASK&SBR_LOWMASK);

    /*set uart0 control 1 register
     * LOOPS = 0 - no looback
     * DOZEEN = 0 - UART enabled in wait mode (doesnt really matter)
     * RSRC = 0 - meaningless when LOOPS=0
     * M = 0 - use 8 bit transmission
     * WAKE = 0 - meaningless because RWU is 0
     * ILT  = 1 - idle character count starts after stopbit
     * PE  = 0 - parity disabled
     * PT = 0 - even parity (meaningless)
     * */
    UART0_C1 =   UART0_C1_LOOPS(UART0_C1_LOOPS_NORMALOPERATION)
                |UART0_C1_DOZEEN(UART0_C1_DOZEEN_ENABLED)
                |UART0_C1_RSRC(UART0_C1_RSRC_DEFAULT)
                |UART0_C1_M(UART0_C1_M_8BIT)
                |UART0_C1_WAKE(UART0_C1_WAKE_DEFAULT)
                |UART0_C1_ILT(UART0_C1_ILT_AFTERSTOP)
                |UART0_C1_PE(UART0_C1_PE_NOPARITY)
                |UART0_C1_PT(UART0_C1_PT_DEFAULTPARTIY);

    /*set uart0 control 2 register
     * TIE = 0, using TCIE interrupt for transmit complete
     * TCIE = 1, transmit complete interupt *is* enabled (when Transmit Complete=1)
     * RIE = 1, reciever interrupt *is* enabled (when Recieve data register full=1)
     * ILIE = 0, dont care about idle line interrupt
     * TE = 0, transmitter disabled during setup
     * RE = 0, reciever disabled during setup
     * RWU = 0, normal operation
     * SBK = 0, dont send break character
     * */
    UART0_C2 =   UART0_C2_TIE(UART0_C2_TIE_DISABLED)
                |UART0_C2_TCIE(UART0_C2_TCIE_DISABLED)
                |UART0_C2_RIE(UART0_C2_RIE_ENABLED)
                |UART0_C2_ILIE(UART0_C2_TLIE_DISABLED)
                |UART0_C2_TE(UART0_C2_TE_DISABLED)
                |UART0_C2_RE(UART0_C2_RE_DISABLED)
                |UART0_C2_RWU(UART0_C2_RWU_NOWAKEUP)
                |UART0_C2_SBK(UART0_C2_SBK_NOBREAK);

    /*register uart0_irqhandler with NVIC*/
    /*should be done by having the samem symbol name UART0_IRQHandler*/

    /*enable specific UART0_IRQ in NVIC
     * NVIC_ISER (interrupt set enable register)
     * or NVIC_EnableIRQ(IRQn_Type IRQn)*/
    NVIC_ClearPendingIRQ(UART0_IRQn);
    NVIC_EnableIRQ(UART0_IRQn);

    /*enable general NVIC interrupts
     * __enable_irq
     * cpsie/cpsid lowest bit, i think*/
    __enable_irq();


    /* initialize the buffers */
    CB_e bufferinitreturn1 = CB_init(recieve_buffer, BUFFER_LENGTH);
#if defined(PROJECT1)||defined(PROJECT2)||defined(PROJECT3)
    CB_e bufferinitreturn2 = CB_init(transmit_buffer, BUFFER_LENGTH);
#endif/*the log buffer gets initialized in the logger code*/

    /*turn on UART0 transmit and recieve
     * UART0_C2[TE]=1
     * UART0_C2[RE]=1;
     * */
    UART0_C2 |=  UART0_C2_TE(UART0_C2_TE_ENABLED)/*this will be turned on as needed*/
                |UART0_C2_RE(UART0_C2_RE_ENABLED);

#if defined(PROJECT1)||defined(PROJECT2)||defined(PROJECT3)
    if(bufferinitreturn1 != SUCCESS || bufferinitreturn2 !=SUCCESS)
    {
        return UART_FAILURE;
    }
#endif
#if defined(PROJECT4)
    if(bufferinitreturn1 != SUCCESS)
    {
        return UART_FAILURE;
    }
#endif

    return UART_SUCCESS;
}

UART_e UART_send(uint8_t *data)
{
    if(data==NULL)
    {
        return UART_FAILURE;
    }
    uint8_t transmitenabledflag;
    uint8_t transmitinterruptenabledflag;
    if(UART0_C2 & UART0_C2_TCIE_MASK)/*check if transmit interrupt is enabled*/
    {
        transmitinterruptenabledflag=1;/*save initial state*/
        UART0_C2 &= ~UART0_C2_TCIE(UART0_C2_TCIE_ENABLED);/*disable if not off*/
    }
    else
    {
        transmitinterruptenabledflag=0;/*save initial state*/
    }
    if(UART0_C2 & UART0_C2_TE_MASK)/*check if transmit is enabled*/
    {
        transmitenabledflag=1;/*save initial state*/
    }
    else
    {
        transmitenabledflag=0;/*save initial state*/
        UART0_C2 |= UART0_C2_TE(UART0_C2_TE_ENABLED);/*enable if off*/
    }

    UART0_D = *data;/*push data into UART0_D register*/
    while(((UART0_S1 & UART0_S1_TC_MASK)>>UART0_S1_TC_SHIFT)==UART0_S1_TC_ACTIVE);/*block on transmit*/

    if(!transmitenabledflag)
    { /*restore transmit state*/
        UART0_C2 &= ~UART0_C2_TE(UART0_C2_TE_ENABLED);
    }
    if(transmitinterruptenabledflag)
    { /*restore transmit interrupt state*/
        UART0_C2 |= UART0_C2_TCIE(UART0_C2_TCIE_ENABLED);
    }
    return UART_SUCCESS;
}

UART_e UART_send_n(uint8_t *data, size_t num_bytes)
{
    if(data==NULL)
    {
        return UART_FAILURE;
    }
    if(num_bytes==0)
    {
        return UART_SUCCESS;
    }
    uint8_t transmitenabledflag;
    uint8_t transmitinterruptenabledflag;
    if(UART0_C2 & UART0_C2_TCIE_MASK)/*check if transmit interrupt is enabled*/
    {
        transmitinterruptenabledflag=1;/*save initial state*/
        UART0_C2 &= ~UART0_C2_TCIE(UART0_C2_TCIE_ENABLED);/*disable if not off*/
    }
    else
    {
        transmitinterruptenabledflag=0;/*save initial state*/
    }
    if(UART0_C2 & UART0_C2_TE_MASK)/*check if transmit is enabled*/
    {
        transmitenabledflag=1;/*save initial state*/
    }
    else
    {
        transmitenabledflag=0;/*save initial state*/
        UART0_C2 |= UART0_C2_TE(UART0_C2_TE_ENABLED);/*enable if off*/
    }

    size_t i = 0;
    for(i=0;i<num_bytes;i++)
    {/*block here and transmit all the data using UART_send*/
        UART0_D = *(data+i);/*push data into UART0_D register*/
        while(((UART0_S1 & UART0_S1_TC_MASK)>>UART0_S1_TC_SHIFT)==UART0_S1_TC_ACTIVE);/*block on transmit*/
    }

    if(!transmitenabledflag)
    { /*restore transmit state*/
        UART0_C2 &= ~UART0_C2_TE(UART0_C2_TE_ENABLED);
    }
    if(transmitinterruptenabledflag)
    { /*restore transmit interrupt state*/
        UART0_C2 |= UART0_C2_TCIE(UART0_C2_TCIE_ENABLED);
    }
    return UART_SUCCESS;
}

UART_e UART_recieve(uint8_t *data)
{
    if(data==NULL)
    {
        return UART_FAILURE;
    }
    uint8_t recieveenabledflag;
    uint8_t recieveinterruptenabledflag;
    if(UART0_C2 & UART0_C2_RIE_MASK)/*check if reciever  interrupt is enabled*/
    {
        recieveinterruptenabledflag=1;/*save initial state*/
        UART0_C2 &= ~UART0_C2_RIE(UART0_C2_RIE_ENABLED);/*disable if not off*/
    }
    else
    {
        recieveinterruptenabledflag=0;/*save initial state*/
    }
    if(UART0_C2 & UART0_C2_RE_MASK)/*check if recieve is enabled*/
    {
        recieveenabledflag=1;/*save initial state*/
    }
    else
    {
        recieveenabledflag=0;/*save initial state*/
        UART0_C2 |= UART0_C2_RE(UART0_C2_RE_ENABLED);/*enable if off*/
    }

    while(((UART0_S1 & UART0_S1_RDRF_MASK)>>UART0_S1_RDRF_SHIFT)==UART0_S1_RDRF_EMPTY);/*block on recieve*/
    *data = UART0_D;/*read from UART0_D into data*/

    if(!recieveenabledflag)
    { /*restore recieve state to disabled if necessary*/
        UART0_C2 &= ~UART0_C2_RE(UART0_C2_RE_ENABLED);
    }
    if(recieveinterruptenabledflag)
    { /*restore recieve interrupt state to enabled if necessary*/
        UART0_C2 |= UART0_C2_RIE(UART0_C2_RIE_ENABLED);
    }
    return UART_SUCCESS;
}

UART_e UART_recieve_n(uint8_t *data, size_t num_bytes)
{
    if(data==NULL)
    {
        return UART_FAILURE;
    }
    if(num_bytes==0)
    {
        return UART_SUCCESS;
    }
    uint8_t recieveenabledflag;
    uint8_t recieveinterruptenabledflag;
    if(UART0_C2 & UART0_C2_RIE_MASK)/*check if reciever  interrupt is enabled*/
    {
        recieveinterruptenabledflag=1;/*save initial state*/
        UART0_C2 &= ~UART0_C2_RIE(UART0_C2_RIE_ENABLED);/*disable if not off*/
    }
    else
    {
        recieveinterruptenabledflag=0;/*save initial state*/
    }
    if(UART0_C2 & UART0_C2_RE_MASK)/*check if recieve is enabled*/
    {
        recieveenabledflag=1;/*save initial state*/
    }
    else
    {
        recieveenabledflag=0;/*save initial state*/
        UART0_C2 |= UART0_C2_RE(UART0_C2_RE_ENABLED);/*enable if off*/
    }

    size_t i = 0;
    for(i=0;i<num_bytes;i++);
    {
        while(((UART0_S1 & UART0_S1_RDRF_MASK)>>UART0_S1_RDRF_SHIFT)==UART0_S1_RDRF_EMPTY);/*block on recieve*/
        *(data+i) = UART0_D;/*read from UART0_D into data*/
    }

    if(!recieveenabledflag)
    { /*restore recieve state to disabled if necessary*/
        UART0_C2 &= ~UART0_C2_RE(UART0_C2_RE_ENABLED);
    }
    if(recieveinterruptenabledflag)
    { /*restore recieve interrupt state to enabled if necessary*/
        UART0_C2 |= UART0_C2_RIE(UART0_C2_RIE_ENABLED);
    }
    return UART_SUCCESS;
}

void UART0_IRQHandler()
{
#if defined(PROJECT1)||defined(PROJECT2)||defined(PROJECT3)
    if(((UART0_S1 & UART0_S1_RDRF_MASK)>>UART0_S1_RDRF_SHIFT)==UART0_S1_RDRF_FULL)
    {
        volatile uint8_t sink = UART0_D;
        if(((UART0_S1 & UART0_S1_TC_MASK)>>UART0_S1_TC_SHIFT)==UART0_S1_TC_IDLE)
    {
        UART0_D = sink;
    }
    else if(transmit_buffer!=NULL)
    {
        CB_buffer_add_item(transmit_buffer,sink);
                UART0_C2 |= (UART0_C2_TIE(UART0_C2_TIE_ENABLED));
    }
        if(recieve_buffer!=NULL)
        {/*discard the data to clear the flag*/
            CB_buffer_add_item(recieve_buffer,sink);
        }
        sink=0;
    }
    if(((UART0_S1 & UART0_S1_TDRE_MASK)>>UART0_S1_TDRE_SHIFT)==UART0_S1_TDRE_EMPTY)
    {
        CB_e ret = EMPTY;
        uint8_t temp;
        if(transmit_buffer!=NULL)
        {
            ret = CB_buffer_remove_item(transmit_buffer, &temp);
            UART0_D = temp;
        }
        if(ret==EMPTY)
        {
            UART0_C2 &= ~(UART0_C2_TIE(UART0_C2_TIE_ENABLED));
        }
    }
    NVIC_ClearPendingIRQ(UART0_IRQn);
#endif
#if defined(PROJECT4)
    if(((UART0_S1 & UART0_S1_RDRF_MASK)>>UART0_S1_RDRF_SHIFT)==UART0_S1_RDRF_FULL)
    {
        volatile uint8_t sink = UART0_D;
        if(recieve_buffer!=NULL)
        {/*discard the data to clear the flag*/
            CB_buffer_add_item(recieve_buffer,sink);
        }
        sink=0;
    }
    if(((UART0_S1 & UART0_S1_TDRE_MASK)>>UART0_S1_TDRE_SHIFT)==UART0_S1_TDRE_EMPTY)
    {
        LQ_e ret = LOGQUEUE_EMPTY;
        if(log_buffer!=NULL)
        {
        if(activeTransfer==NULL)
        {
            /*if there's no active transfer, start one
             * by pulling a new log_t from the queue*/
            step=0;
            datacounter=0;
                        ret = LQ_buffer_remove_item(log_buffer, &activeTransfer);
        }
        if(activeTransfer!=NULL)
        {/*if there's an active transfer, step through the transfer process*/
            ret = LOGQUEUE_SUCCESS;
            switch(step)/*switch on how far through the struct transfer you are*/
            {
                case 0:/*step 0, transfer log ID*/
                    step++;
                    UART0_D = activeTransfer->LogID;
                    break;
                case 1:/*step 1, transfer module ID*/
                    step++;
                    UART0_D = activeTransfer->ModuleID;
                    break;
                case 2:/*step 2, 2 byte sequence to transfer log length*/
                    if(datacounter==0)
                    {
                    /*set up continuous pointer
                     * to current location in multibyte data*/
                        dataptr=(uint8_t*)(&activeTransfer->LogLength);
                    }
                    if(datacounter<1)
                    {
                    /*increment through multibyte data on
                     * successive interrupts*/
                        UART0_D = *(dataptr++);
                        datacounter++;
                    }
                    else if(datacounter>=1)
                    {
                    /*on the last byte, increment to the next step
                     * and return the tracking counter to zero for
                     * the next piece of multibyte information*/
                        UART0_D=*dataptr;
                        datacounter=0;
                        step++;
                    }
                    break;
                case 3:/*step 3, 4 byte sequence to transfer timestamp*/
                    if(datacounter==0)
                    {
                    /*set up continuous pointer
                     * to current location in multibyte data*/
                        dataptr=(uint8_t*)(&activeTransfer->Timestamp);
                    }
                    if(datacounter<3)
                    {
                    /*increment through multibyte data on
                     * successive interrupts*/
                        UART0_D = *(dataptr++);
                        datacounter++;
                    }
                    else if(datacounter>=3)
                    {
                    /*on the last byte, increment to the next step
                     * and return the tracking counter to zero for
                     * the next piece of multibyte information*/
                        UART0_D=*dataptr;
                        datacounter=0;
                        step++;
                    }
                    break;
                case 4:/*step 4, LogLength byte sequence to transmit payload*/
                    if(activeTransfer->LogLength==0)
                    {
                        step=6;
                        UART0_D = activeTransfer->Checksum;
                        break;
                    }
                    if(datacounter==0)
                    {
                    /*set up continuous pointer
                     * to current location in multibyte data*/
                        dataptr=(activeTransfer->PayloadData);
                    }
                    if(datacounter < activeTransfer->LogLength-1)
                    {
                    /*increment through multibyte data on
                     * successive interrupts*/
                        UART0_D = *(dataptr++);
                        datacounter++;
                    }
                    else if(datacounter>= activeTransfer->LogLength-1 )
                    {
                    /*on the last byte, increment to the next step
                     * and return the tracking counter to zero for
                     * the next piece of multibyte information*/
                        UART0_D=*dataptr;
                        datacounter=0;
                        step++;
                    }
                    break;
                case 5:/*step 5, transmit checksum*/
                    step++;
                    UART0_D = activeTransfer->Checksum;
                    break;
                default:
                    /*ostensibly step 6, because the transfer will still be active
                     * after the step 5 case, we free that data here, but we then
                     * have to check whether there's more data, in which case we
                     * have to start the next one, or if we can turn off the
                     * transmitter, just like in the outermost conditional*/
                    step=0;
                    if(activeTransfer->PayloadData)free(activeTransfer->PayloadData);
                    if(activeTransfer)free(activeTransfer);
                    activeTransfer=NULL;
                    dataptr=NULL;
                    datacounter=0;
                    ret=LQ_buffer_remove_item(log_buffer,&activeTransfer);
                    if(ret==LOGQUEUE_SUCCESS)
                    {/*succesfully pulled new data, do step 0*/
                        step++;
                        UART0_D = activeTransfer->LogID;
                    }
                    else if(ret==LOGQUEUE_EMPTY)
                    {/*the queue is empty, turn the transmitter off so we
                     * dont come back to the interrupt handler*/
                        UART0_C2 &= ~(UART0_C2_TIE(UART0_C2_TIE_ENABLED));
                    }
                    break;
            }
        }
        }
        if(ret==LOGQUEUE_EMPTY)
        {/*if we check and the log buffer is empty, then turn off the interrupt*/
            UART0_C2 &= ~(UART0_C2_TIE(UART0_C2_TIE_ENABLED));
        }
    }
    NVIC_ClearPendingIRQ(UART0_IRQn);/*interrupt has been dealt with*/
#endif
}

UART_e UART_start_buffered_transmission()
{
    UART0_C2 |= UART0_C2_TE(UART0_C2_TE_ENABLED);/*begin transmission*/
    UART0_C2 |= UART0_C2_TIE(UART0_C2_TIE_ENABLED); /*enable transmission interrupt*/
    return UART_SUCCESS;
}
