import wx
import wx.xrc
import wx.adv
import os

import time
import binascii

import com
import re
from serial.tools import list_ports
import threading
import queue

class convertFile():
    def __init__(self, filein, fileout):
        self.filein = filein
        self.fileout = fileout

        self.SYSTEM_ID = 0
        self.SYSTEM_VERSION = 1
        self.LOGGER_INITIALIZED = 2
        self.GPIO_INITIALIZED = 3
        self.SYSTEM_INITIALIZED = 4
        self.SYSTEM_HALTED = 5
        self.INFO = 6
        self.WARNING = 7
        self.ERROR = 8
        self.PROFILING_STARTED = 9
        self.PROFILING_RESULT = 10
        self.PROFILING_COMPLETED = 11
        self.DATA_RECIEVED = 12
        self.DATA_ANALYSIS_STARTED = 13
        self.DATA_ALPHA_COUNT = 14
        self.DATA_NUMERIC_COUNT = 15
        self.DATA_PUNCTUATION_COUNT = 16
        self.DATA_MISC_COUNT = 17
        self.DATA_ANALYSIS_COMPLETED = 18
        self.HEARTBEAT = 19
        self.CORE_DUMP = 20

        self.FUNC_CIRCBUF = 0
        self.FUNC_CONVERSION = 1
        self.FUNC_DATA = 2
        self.FUNC_DEBUG = 3
        self.FUNC_LOGGER = 4
        self.FUNC_LOGGER_QUEUE = 5
        self.FUNC_MAIN = 6
        self.FUNC_MEMORY = 7
        self.FUNC_NORDIC = 8
        self.FUNC_PORT = 9
        self.FUNC_PROJECT1 = 10
        self.FUNC_PROJECT2 = 11
        self.FUNC_PROJECT3 = 12
        self.FUNC_PROJECT4 = 13
        self.FUNC_SPI = 14
        self.FUNC_UART = 15
        self.FUNC_UNITTEST = 16

        self.totallog = ''

    def readfilein(self):
        pass

    def convertascii(self, packet):
        return str(bytearray(packet))

    def convertint(self, packet, length):
        packet_int = 0
        for i in range(0, length):
            packet_int += (2^(i*8))*int(byte_s[i].encode('hex'),16)
        return str(packet_int)

    def convertraw(self, packet):
        return binascii.hexlify(packet)

    def convert(self):
        logid = 0
        moduleid = 0
        loglength = 0
        timestamp = 0
        payload = bytearray(b'')
        checksum = 0
        packetstring = ''
        payloadstring = ''
        with open(self.filein, 'rb') as f:
            while(1):
                byte_s = f.read(1)
                if not byte_s:
                    break
                logid = int(byte_s[0].encode('hex'), 16)
                checksum ^= logid
                byte_s = f.read(1)
                if not byte_s:
                    break
                moduleid = int(byte_s[0].encode('hex'), 16)
                checksum ^= moduleid
                byte_s = f.read(2)
                loglength = int(byte_s[0].encode('hex'),16)+\
                        (256*(int(byte_s[1].encode('hex'),16)))
                for i in range(0,2):
                    checksum ^= int(byte_s[i].encode('hex'),16)
                byte_s = f.read(4)
                timestamp = int(byte_s[0].encode('hex'),16)+\
                        (256*(int(byte_s[1].encode('hex'),16)))+\
                        (65536*(int(byte_s[2].encode('hex'),16)))+\
                        (16777216*(int(byte_s[3].encode('hex'),16)))
                for i in range(0,4):
                    checksum ^= int(byte_s[i].encode('hex'),16)
                payload = f.read(loglength)
                for i in range(0,loglength):
                    checksum ^= int(payload[i].encode('hex'),16)
                byte_s = f.read(1)
                checksum ^= int(byte_s[0].encode('hex'), 16)
                # format all the packet data and print to file
                if not checksum:
                    packetstring = "Checksum: PASS\t\t"
                else:
                    packetstring = "Checksum: FAIL\t\t"
                packetstring += time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(timestamp))
                packetstring += "\t\t"

                # I really hate how python doesn't have a case switch
                if logid == self.SYSTEM_ID:
                    packetstring += "SYSTEM_ID\t\t"
                    payloadstring = self.convertraw(payload)
                elif logid == self.SYSTEM_VERSION:
                    packetstring += "SYSTEM_VERSION\t\t"
                    payloadstring = self.convertraw(payload)
                elif logid == self.LOGGER_INITIALIZED:
                    packetstring += "LOGGER_INITIALIZED\t\t"
                    payloadstring = self.convertraw(payload)
                elif logid == self.GPIO_INITIALIZED:
                    packetstring += "GPIO_INITIALIZED\t\t"
                    payloadstring = self.convertraw(payload)
                elif logid == self.SYSTEM_INITIALIZED:
                    packetstring += "SYSTEM_INITIALIZED\t\t"
                    payloadstring = self.convertraw(payload)
                elif logid == self.SYSTEM_HALTED:
                    packetstring += "SYSTEM_HALTED\t\t"
                    payloadstring = self.convertraw(payload)
                elif logid == self.INFO:
                    packetstring += "INFO\t\t\t"
                    payloadstring = self.convertascii(payload)
                elif logid == self.WARNING:
                    packetstring += "WARNING\t\t\t"
                    payloadstring = self.convertascii(payload)
                elif logid == self.ERROR:
                    packetstring += "ERROR\t\t\t"
                    payloadstring = self.convertascii(payload)
                elif logid == self.PROFILING_STARTED:
                    packetstring += "PROFILING_STARTED\t\t"
                    payloadstring = self.convertraw(payload)
                elif logid == self.PROFILING_RESULT:
                    packetstring += "PROFILING_RESULT\t\t"
                    payloadstring = self.convertraw(payload)
                elif logid == self.PROFILING_COMPLETED:
                    packetstring += "PROFILING_COMPLETED\t\t"
                    payloadstring = self.convertraw(payload)
                elif logid == self.DATA_RECIEVED:
                    packetstring += "DATA_RECIEVED\t\t"
                    payloadstring = self.convertint(payload, loglength)
                elif logid == self.DATA_ANALYSIS_STARTED:
                    packetstring += "DATA_ANALYSIS_STARTED\t\t"
                    payloadstring = self.convertint(payload, loglength)
                elif logid == self.DATA_ALPHA_COUNT:
                    packetstring += "DATA_ALPHA_COUNT\t\t"
                    payloadstring = self.convertint(payload, loglength)
                elif logid == self.DATA_NUMERIC_COUNT:
                    packetstring += "DATA_NUMERIC_COUNT\t\t"
                    payloadstring = self.convertint(payload, loglength)
                elif logid == self.DATA_PUNCTUATION_COUNT:
                    packetstring += "DATA_PUNCTUATION_COUNT\t\t"
                    payloadstring = self.convertint(payload, loglength)
                elif logid == self.DATA_MISC_COUNT:
                    packetstring += "DATA_MISC_COUNT\t\t"
                    payloadstring = self.convertint(payload, loglength)
                elif logid == self.DATA_ANALYSIS_COMPLETED:
                    packetstring += "DATA_ANALYSIS_COMPLETED\t\t"
                    payloadstring = self.convertint(payload, loglength)
                elif logid == self.HEARTBEAT:
                    packetstring += "HEARTBEAT\t\t"
                    payloadstring = self.convertraw(payload)
                elif logid == self.CORE_DUMP:
                    packetstring += "CORE_DUMP\t\t"
                    payloadstring = self.convertraw(payload)
                else:
                    packetstring += "BAD_PACKET\t\t"
                    payloadstring = self.convertraw(payload)

                if moduleid == self.FUNC_CIRCBUF:
                    packetstring += "FUNC_CIRCBUF\t\t"
                elif moduleid == self.FUNC_CONVERSION:
                    packetstring += "FUNC_CONVERSION\t\t"
                elif moduleid == self.FUNC_DATA:
                    packetstring += "FUNC_DATA\t\t"
                elif moduleid == self.FUNC_DEBUG:
                    packetstring += "FUNC_DEBUG\t\t"
                elif moduleid == self.FUNC_LOGGER:
                    packetstring += "FUNC_LOGGER\t\t"
                elif moduleid == self.FUNC_LOGGER_QUEUE:
                    packetstring += "FUNC_LOGGER_QUEUE\t\t"
                elif moduleid == self.FUNC_MAIN:
                    packetstring += "FUNC_MAIN\t\t"
                elif moduleid == self.FUNC_MEMORY:
                    packetstring += "FUNC_MEMORY\t\t"
                elif moduleid == self.FUNC_NORDIC:
                    packetstring += "FUNC_NORDIC\t\t"
                elif moduleid == self.FUNC_PORT:
                    packetstring += "FUNC_PORT\t\t"
                elif moduleid == self.FUNC_PROJECT1:
                    packetstring += "FUNC_PROJECT1\t\t"
                elif moduleid == self.FUNC_PROJECT2:
                    packetstring += "FUNC_PROJECT2\t\t"
                elif moduleid == self.FUNC_PROJECT3:
                    packetstring += "FUNC_PROJECT3\t\t"
                elif moduleid == self.FUNC_PROJECT4:
                    packetstring += "FUNC_PROJECT4\t\t"
                elif moduleid == self.FUNC_SPI:
                    packetstring += "FUNC_SPI\t\t"
                elif moduleid == self.FUNC_UART:
                    packetstring += "FUNC_UART\t\t"
                elif moduleid == self.FUNC_UNITTEST:
                    packetstring += "FUNC_UNITTEST\t\t"
                else:
                    packetstring += "BAD_PACKET\t\t"

                packetstring += "Length = " + str(loglength) + " Bytes\t\t"
                packetstring += "Data = " + payloadstring + "\r\n"

                print packetstring
                self.totallog += packetstring

class Project4 ( wx.Frame ):
    def __init__( self ):
        wx.Frame.__init__ ( self, None, id = wx.ID_ANY, title = "Project 4 Logging Applicaiton", pos = wx.DefaultPosition, size = wx.Size( 640,360 ), style = wx.DEFAULT_FRAME_STYLE|wx.TAB_TRAVERSAL )

        self.SetSizeHints( wx.DefaultSize, wx.DefaultSize )

        main_sizer = wx.BoxSizer( wx.VERTICAL )

        self.recieve_sizer = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.TE_MULTILINE|wx.TE_READONLY )
        main_sizer.Add( self.recieve_sizer, 1, wx.ALIGN_CENTER_HORIZONTAL|wx.EXPAND|wx.LEFT|wx.RIGHT|wx.TOP, 5 )

        send_sizer = wx.BoxSizer( wx.HORIZONTAL )

        self.send_txt = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
        send_sizer.Add( self.send_txt, 1, wx.ALL|wx.EXPAND, 5 )

        self.send_btn = wx.Button( self, wx.ID_ANY, u"Send", wx.DefaultPosition, wx.DefaultSize, 0 )
        send_sizer.Add( self.send_btn, 0, wx.ALIGN_CENTER_VERTICAL|wx.BOTTOM|wx.EXPAND|wx.RIGHT|wx.TOP, 5 )


        main_sizer.Add( send_sizer, 0, wx.EXPAND, 5 )


        self.SetSizer( main_sizer )
        self.Layout()
        self.statusbar = self.CreateStatusBar( 1, wx.CAPTION, wx.ID_ANY )
        self.menubar = wx.MenuBar( 0 )
        self.logging_menu = wx.Menu()
        self.menubar.Append( self.logging_menu, u"Logging" )

        self.com_menu = wx.Menu()
        self.menubar.Append( self.com_menu, u"Communication" )

        self.SetMenuBar( self.menubar )

        if os.path.exists(os.path.join(os.getcwd(), 'mainicon.ico')):
            self.favicon = wx.Icon(os.path.join(os.getcwd(), 'mainicon.ico'), wx.BITMAP_TYPE_ICO)
        else:
            self.favicon = wx.Icon(os.path.join(getattr(sys, '_MEIPASS', os.getcwd()), 'mainicon.ico'), wx.BITMAP_TYPE_ICO)
        self.SetIcon(self.favicon)
        self.tb_icon = wx.adv.TaskBarIcon()
        self.tb_icon.SetIcon(self.favicon, "Project 4 Logging Appliction")


        self.Centre( wx.BOTH )

        device = ""
        ports = list(list_ports.comports())
        search_ports = []
        for port in ports:
            search_ports.append("D:[" + str(port.device) + "]N:[" + str(port.name) + "]I:[" + \
                str(port.interface) + "]H:[" + str(port.hwid) + "]V:[" + str(port.vid) + "]P:[" + \
                str(port.pid) + "]S:[" + str(port.serial_number) + "]L:[" + str(port.location) + \
                "]P:[" + str(port.product) + "]M:[" + str(port.manufacturer) + "]D:[" + \
                str(port.description) + "]")
        uartport = "OpenSDA - CDC Serial Port"
        for i in range(0, len(search_ports)):
            if re.search(uartport, str(search_ports[i])):
                device = ports[i].device
        print "uart port is: " + str(device)

        self.inqueue = queue.Queue()
        self.outqueue = queue.Queue()
        self.comport = com.comPort(self.inqueue, self.outqueue, device, 19200)

    def __del__( self ):
        self.comport.Stop()
        pass

"""
Jump to the main application
"""
if __name__ == "__main__":
    app = wx.App(None)
    boardui = Project4().Show()
    app.MainLoop()
