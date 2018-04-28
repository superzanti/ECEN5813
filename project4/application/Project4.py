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

class convertFile(threading.Thread):
    def __init__(self, filein, fileout, inqueue):
        threading.Thread.__init__(self)
        self.setDaemon(True)

        self.filein = filein
        self.fileout = fileout
        self.inqueue = inqueue

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

        self.log = bytearray(b'')
        self.totallog = ''
        self.newdata = ''

        self.logid = 0
        self.moduleid = 0
        self.loglength = 0
        self.timestamp = 0
        self.payload = bytearray(b'')
        self.checksum = 0

        self.stop = False
        if not self.stop:
            self.start()

    def run(self):
        while not self.stop:
            byte_s = [self.inqueue.get()]
            self.log.extend(byte_s)
            if not byte_s:
                break
            self.logid = int(byte_s[0].encode('hex'), 16)
            self.checksum ^= self.logid
            byte_s = [self.inqueue.get()]
            self.log.extend(byte_s)
            if not byte_s:
                break
            self.moduleid = int(byte_s[0].encode('hex'), 16)
            self.checksum ^= self.moduleid
            byte_s = [self.inqueue.get()]
            byte_s.append(self.inqueue.get())
            self.log.extend(byte_s)
            self.loglength = int(byte_s[0].encode('hex'),16)+\
                    (256*(int(byte_s[1].encode('hex'),16)))
            for i in range(0,2):
                self.checksum ^= int(byte_s[i].encode('hex'),16)
            byte_s = [self.inqueue.get()]
            byte_s.append(self.inqueue.get())
            byte_s.append(self.inqueue.get())
            byte_s.append(self.inqueue.get())
            self.log.extend(byte_s)
            self.timestamp = int(byte_s[0].encode('hex'),16)+\
                    (256*(int(byte_s[1].encode('hex'),16)))+\
                    (65536*(int(byte_s[2].encode('hex'),16)))+\
                    (16777216*(int(byte_s[3].encode('hex'),16)))
            for i in range(0,4):
                self.checksum ^= int(byte_s[i].encode('hex'),16)
            for i in range(0,self.loglength):
                self.payload.append(self.inqueue.get())
            self.log.extend(self.payload)
            for i in range(0,self.loglength):
                self.checksum ^= int(str(self.payload[i]).encode('hex'),16)
            byte_s = [self.inqueue.get()]
            self.log.extend(byte_s)
            self.checksum ^= int(byte_s[0].encode('hex'), 16)
            # format all the packet data and print to file
            self.formatpacket()

    def convertascii(self, packet):
        return str(bytearray(packet))

    def convertint(self, packet, length):
        packet_int = 0
        for i in range(0, length):
            packet_int += (2^(i*8))*int(byte_s[i].encode('hex'),16)
        return str(packet_int)

    def convertraw(self, packet):
        return binascii.hexlify(packet)

    def formatpacket(self):
        packetstring = ''

        if not self.checksum:
            packetstring = "Checksum: PASS\t\t"
        else:
            packetstring = "Checksum: FAIL\t\t"
        packetstring += time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(self.timestamp))
        packetstring += "\t\t"

        # I really hate how python doesn't have a case switch
        if self.logid == self.SYSTEM_ID:
            packetstring += "SYSTEM_ID\t\t"
            payloadstring = self.convertraw(self.payload)
        elif self.logid == self.SYSTEM_VERSION:
            packetstring += "SYSTEM_VERSION\t\t"
            payloadstring = self.convertraw(self.payload)
        elif self.logid == self.LOGGER_INITIALIZED:
            packetstring += "LOGGER_INITIALIZED\t\t"
            payloadstring = self.convertraw(self.payload)
        elif self.logid == self.GPIO_INITIALIZED:
            packetstring += "GPIO_INITIALIZED\t\t"
            payloadstring = self.convertraw(self.payload)
        elif self.logid == self.SYSTEM_INITIALIZED:
            packetstring += "SYSTEM_INITIALIZED\t\t"
            payloadstring = self.convertraw(self.payload)
        elif self.logid == self.SYSTEM_HALTED:
            packetstring += "SYSTEM_HALTED\t\t"
            payloadstring = self.convertraw(self.payload)
        elif self.logid == self.INFO:
            packetstring += "INFO\t\t\t"
            payloadstring = self.convertascii(self.payload)
        elif self.logid == self.WARNING:
            packetstring += "WARNING\t\t\t"
            payloadstring = self.convertascii(self.payload)
        elif self.logid == self.ERROR:
            packetstring += "ERROR\t\t\t"
            payloadstring = self.convertascii(self.payload)
        elif self.logid == self.PROFILING_STARTED:
            packetstring += "PROFILING_STARTED\t\t"
            payloadstring = self.convertraw(self.payload)
        elif self.logid == self.PROFILING_RESULT:
            packetstring += "PROFILING_RESULT\t\t"
            payloadstring = self.convertraw(self.payload)
        elif self.logid == self.PROFILING_COMPLETED:
            packetstring += "PROFILING_COMPLETED\t\t"
            payloadstring = self.convertraw(self.payload)
        elif self.logid == self.DATA_RECIEVED:
            packetstring += "DATA_RECIEVED\t\t"
            payloadstring = self.convertint(self.payload, self.loglength)
        elif self.logid == self.DATA_ANALYSIS_STARTED:
            packetstring += "DATA_ANALYSIS_STARTED\t\t"
            payloadstring = self.convertint(self.payload, self.loglength)
        elif self.logid == self.DATA_ALPHA_COUNT:
            packetstring += "DATA_ALPHA_COUNT\t\t"
            payloadstring = self.convertint(self.payload, self.loglength)
        elif self.logid == self.DATA_NUMERIC_COUNT:
            packetstring += "DATA_NUMERIC_COUNT\t\t"
            payloadstring = self.convertint(self.payload, self.loglength)
        elif self.logid == self.DATA_PUNCTUATION_COUNT:
            packetstring += "DATA_PUNCTUATION_COUNT\t\t"
            payloadstring = self.convertint(self.payload, self.loglength)
        elif self.logid == self.DATA_MISC_COUNT:
            packetstring += "DATA_MISC_COUNT\t\t"
            payloadstring = self.convertint(self.payload, self.loglength)
        elif self.logid == self.DATA_ANALYSIS_COMPLETED:
            packetstring += "DATA_ANALYSIS_COMPLETED\t\t"
            payloadstring = self.convertint(self.payload, self.loglength)
        elif self.logid == self.HEARTBEAT:
            packetstring += "HEARTBEAT\t\t"
            payloadstring = self.convertraw(self.payload)
        elif self.logid == self.CORE_DUMP:
            packetstring += "CORE_DUMP\t\t"
            payloadstring = self.convertraw(self.payload)
        else:
            packetstring += "BAD_PACKET\t\t"
            payloadstring = self.convertraw(self.payload)

        if self.moduleid == self.FUNC_CIRCBUF:
            packetstring += "FUNC_CIRCBUF\t\t"
        elif self.moduleid == self.FUNC_CONVERSION:
            packetstring += "FUNC_CONVERSION\t\t"
        elif self.moduleid == self.FUNC_DATA:
            packetstring += "FUNC_DATA\t\t"
        elif self.moduleid == self.FUNC_DEBUG:
            packetstring += "FUNC_DEBUG\t\t"
        elif self.moduleid == self.FUNC_LOGGER:
            packetstring += "FUNC_LOGGER\t\t"
        elif self.moduleid == self.FUNC_LOGGER_QUEUE:
            packetstring += "FUNC_LOGGER_QUEUE\t\t"
        elif self.moduleid == self.FUNC_MAIN:
            packetstring += "FUNC_MAIN\t\t"
        elif self.moduleid == self.FUNC_MEMORY:
            packetstring += "FUNC_MEMORY\t\t"
        elif self.moduleid == self.FUNC_NORDIC:
            packetstring += "FUNC_NORDIC\t\t"
        elif self.moduleid == self.FUNC_PORT:
            packetstring += "FUNC_PORT\t\t"
        elif self.moduleid == self.FUNC_PROJECT1:
            packetstring += "FUNC_PROJECT1\t\t"
        elif self.moduleid == self.FUNC_PROJECT2:
            packetstring += "FUNC_PROJECT2\t\t"
        elif self.moduleid == self.FUNC_PROJECT3:
            packetstring += "FUNC_PROJECT3\t\t"
        elif self.moduleid == self.FUNC_PROJECT4:
            packetstring += "FUNC_PROJECT4\t\t"
        elif self.moduleid == self.FUNC_SPI:
            packetstring += "FUNC_SPI\t\t"
        elif self.moduleid == self.FUNC_UART:
            packetstring += "FUNC_UART\t\t"
        elif self.moduleid == self.FUNC_UNITTEST:
            packetstring += "FUNC_UNITTEST\t\t"
        else:
            packetstring += "BAD_PACKET\t\t"

        packetstring += "Length = " + str(self.loglength) + " Bytes\t\t"
        packetstring += "Data = " + payloadstring + "\r\n"

        self.payload = bytearray(b'')
        self.totallog += packetstring
        self.newdata += packetstring

    def convert(self):
        self.log = bytearray(b'')
        with open(self.filein, 'rb') as f:
            while(1):
                byte_s = f.read(1)
                self.log.extend(byte_s)
                if not byte_s:
                    break
                self.logid = int(byte_s[0].encode('hex'), 16)
                self.checksum ^= self.logid
                byte_s = f.read(1)
                self.log.extend(byte_s)
                if not byte_s:
                    break
                self.moduleid = int(byte_s[0].encode('hex'), 16)
                self.checksum ^= self.moduleid
                byte_s = f.read(2)
                self.log.extend(byte_s)
                self.loglength = int(byte_s[0].encode('hex'),16)+\
                        (256*(int(byte_s[1].encode('hex'),16)))
                for i in range(0,2):
                    self.checksum ^= int(byte_s[i].encode('hex'),16)
                byte_s = f.read(4)
                self.log.extend(byte_s)
                self.timestamp = int(byte_s[0].encode('hex'),16)+\
                        (256*(int(byte_s[1].encode('hex'),16)))+\
                        (65536*(int(byte_s[2].encode('hex'),16)))+\
                        (16777216*(int(byte_s[3].encode('hex'),16)))
                for i in range(0,4):
                    self.checksum ^= int(byte_s[i].encode('hex'),16)
                self.payload = f.read(self.loglength)
                self.log.extend(self.payload)
                for i in range(0,self.loglength):
                    self.checksum ^= int(self.payload[i].encode('hex'),16)
                byte_s = f.read(1)
                self.log.extend(byte_s)
                self.checksum ^= int(byte_s[0].encode('hex'), 16)
                # format all the packet data and print to file
                self.formatpacket()

class Project4 ( wx.Frame ):
    def __init__( self ):
        wx.Frame.__init__ ( self, None, id = wx.ID_ANY, title = "Project 4 Logging Applicaiton", pos = wx.DefaultPosition, size = wx.Size( 640,360 ), style = wx.DEFAULT_FRAME_STYLE|wx.TAB_TRAVERSAL )

        self.SetSizeHints( wx.DefaultSize, wx.DefaultSize )

        main_sizer = wx.BoxSizer( wx.VERTICAL )

        self.recieve_sizer = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, \
                wx.TE_MULTILINE|wx.TE_READONLY|wx.TE_DONTWRAP )
        main_sizer.Add( self.recieve_sizer, 1, wx.ALIGN_CENTER_HORIZONTAL|wx.EXPAND|wx.LEFT|wx.RIGHT|wx.TOP, 5 )

        send_sizer = wx.BoxSizer( wx.HORIZONTAL )

        self.send_txt = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, 0 )
        send_sizer.Add( self.send_txt, 1, wx.ALL|wx.EXPAND, 5 )

        self.send_btn = wx.Button( self, wx.ID_ANY, u"Send", wx.DefaultPosition, wx.DefaultSize, 0 )
        send_sizer.Add( self.send_btn, 0, wx.ALIGN_CENTER_VERTICAL|wx.BOTTOM|wx.EXPAND|wx.RIGHT|wx.TOP, 5 )
        self.send_btn.Bind(wx.EVT_BUTTON, self.uartsend)


        main_sizer.Add( send_sizer, 0, wx.EXPAND, 5 )


        self.SetSizer( main_sizer )
        self.Layout()
        self.statusbar = self.CreateStatusBar( 1, wx.CAPTION, wx.ID_ANY )
        self.menubar = wx.MenuBar( 0 )

        self.file_menu = wx.Menu()
        self.menubar.Append( self.file_menu, u"File" )
        self.open_existing_menu = wx.MenuItem(self.file_menu, wx.ID_ANY, u"Open existing binary log", \
                "This will start appending to the new log", wx.ITEM_NORMAL )
        self.file_menu.Append(self.open_existing_menu)
        self.Bind(wx.EVT_MENU, self.openexisting, id = self.open_existing_menu.GetId())

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

        #timer for updating the log
        self.timer = wx.Timer(self, wx.ID_ANY)
        self.Bind(wx.EVT_TIMER, self.OnTimer)
        self.timer.Start(10) # 10 times a second

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
        self.statusbar.SetStatusText("UART port is: " + str(device))

        self.inqueue = queue.Queue()
        self.outqueue = queue.Queue()
        self.comport = com.comPort(self.inqueue, self.outqueue, device, 19200)

        #add in the ability for converting logs
        self.ConvertFile = convertFile('out.txt', 'log.txt', self.inqueue)

        # TODO: first 4 bytes on connect should send the epoch time
        #calendar.timegm(time.gmtime())

    def __del__( self ):
        self.comport.Stop()

    def OnTimer(self, event):
        if len(self.ConvertFile.newdata) > 0:
            self.recieve_sizer.AppendText(self.ConvertFile.newdata)
            self.ConvertFile.newdata = ''

    def uartsend(self,event):
        sendval = self.send_txt.GetValue()
        self.recieve_sizer.AppendText("SENT: " + str(sendval) + "\r\n")
        self.send_txt.SetValue('')
        self.outqueue.put(sendval.decode('string_escape'))

    def openexisting(self,event):
        with wx.FileDialog(self, "Open a log file", \
                style=wx.FD_OPEN | wx.FD_FILE_MUST_EXIST) as fileDialog:
            if fileDialog.ShowModal() == wx.ID_CANCEL:
                return
            pathname = fileDialog.GetPath()
            try:
                self.ConvertFile.filein = pathname
                self.ConvertFile.convert()
                self.recieve_sizer.AppendText(self.ConvertFile.newdata)
                self.ConvertFile.newdata = ''
            except IOError:
                self.statusbar.SetStatusText("Cannot open file " + str(pathname))

"""
Jump to the main application
"""
if __name__ == "__main__":
    app = wx.App(None)
    boardui = Project4().Show()
    app.MainLoop()
