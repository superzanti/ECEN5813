#Project 2 Readme File

Project 2 consists up furthering the work from project 1 to include the
following:

1. New circular buffer structure to hold information. tests for this object
must work on all plaforms.

2. Unit testing of existing code with new unit test platform CMOKA - this
must work on the host machine

3. GPIO setup code and UART setup code for the KL25z, this only has to work
with the KL25z, and not with the other systems

4. Input data processing in project2.c, consisting of counting characters
from input strings. this must work on the host machine with gets or scanf,
and on the KL25z over interrupt driven UART & GPIO

5. Data handling on the KL25z must occur in the project2.c file (main code
execution), while inputing data to the circular buffer must occur in the
interrupt code

####New Files

port.c, port.h

    These files will contain GPIO setup and general pin and LED handling
    likely dependencies: stdint.h, CMSIS code

circbuf.c, circbuf.h

    These files will define the circular buffer code. The buffer will have to
    be interrupt safe, so it needs to be volatile *at least*

uart.c, uart.h

    These files will need to contain code (including, i think, the interrupt
    function call), to manage the UART system on the kl25z, including setup
    and byte sending. hopefully the kl25z is clocked fast enough that the UART
    is relatively slow and we dont need to worry about nested interrupts

project2.c

    this will contain the code to do the data processing, and some mechanism
    by which the interrupt can tell project2.c to begin data processing

####Updates

arch\_arm32.c and arch\_arm32.h

    these need updating to include further detail and macros

Makefile

    this needs to be updated to allow for non-default Verbose mode,
    as well as project specific flags to compile for project 1 or project 2
