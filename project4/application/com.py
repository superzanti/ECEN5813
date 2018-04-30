# the com port will be a thread that is running the whole time
import threading
# we need to communicate with a serial device
import serial

import binascii

# for printing and logging errors
import traceback
import logging

import time

try:
    import queue
except:
    import Queue as queue

class comPort(threading.Thread):
    def __init__(self, inqueue, outqueue, device, baud = 115200, _timeout = 0.001, _stopbits = serial.STOPBITS_ONE, support_early = False):
        # initialize the thread
        threading.Thread.__init__(self)
        self.setDaemon(True)

        self.logger = logging.getLogger("BoardUI.CharStream")
        self.loggermain = logging.getLogger("BoardUI.Main")

        self.loggermain.debug("Setting up comPort thread")

        self.inqueue = inqueue
        self.outqueue = outqueue
        self.support_early = support_early

        self.stop = False
        self.com = None

        timeout = time.time() + 1
        self.loggermain.debug("Opening serial port")
        while self.com == None:
            try:
                self.com = serial.Serial()
                self.com.port = device
                self.com.baudrate = baud
                self.com.timeout = _timeout
                self.com.stopbits = _stopbits
                self.com.open()
                self.com.reset_input_buffer()
                self.com.reset_output_buffer()
                self.com.flush()
                self.stop = False
            except:
                self.stop = True
                self.com = None
                if time.time() > timeout:
                    self.loggermain.debug("Could not connect to the serial port. Canceling.")
                    break

        if not self.stop:
            self.start()

    def run(self):
        self.readerThread = threading.Thread(target=self.reader, name='comrx')
        self.readerThread.setDaemon(True)
        self.readerThread.start()
        self.writerThread = threading.Thread(target=self.writer, name='comtx')
        self.writerThread.setDaemon(True)
        self.writerThread.start()
        while not self.stop:
            pass
        self.readerThread.join()
        self.writerThread.join()
        try:
            self.loggermain.debug("Closing serial port")
            while(self.com.isOpen()):
                self.com.close()
        except:
            self.loggermain.error(traceback.format_exc())
            self.loggermain.debug("Couldn't close the serial device properly")

    def reader(self):
        while not self.stop:
            try:
                buff_in = self.com.read(1)
                if len(buff_in) > 0:
                    self.inqueue.put(buff_in)
                    #print "Got: " + str(binascii.hexlify(buff_in))
            except:
                pass
            finally:
                buff_in = ''

    def writer(self):
        while not self.stop:
            try:
                buff_out = self.outqueue.get(timeout = 0)
                if len(buff_out) > 0:
                    for c in buff_out:
                        self.com.write(c)
                        print "Sent: " + str(binascii.hexlify(c))
                        time.sleep(0.02)
            except:
                pass
            finally:
                buff_out = ''

    def Stop(self):
        self.loggermain.debug("Stopping comPort thread")
        self.stop = True

