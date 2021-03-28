import threading
import queue
import struct
import socket

HEAD_SIZE = 8
class TcpClientBase(threading.Thread):
    def __init__(self, ip, port):
        threading.Thread.__init__(self)

        self.socket = None
        self.ip = ip
        self.port = port

        self.sends = queue.Queue()
        self.cursend = None
        self.sendpos = 0

        self.reads = queue.Queue()
        self.curread = bytearray()
        self.readpos = 0

    def connect(self):
        self.socket = socket.socket()
        self.socket.connect((self.ip, self.port))
        self.socket.setblocking(False)

    def __startRecv(self, size):
        try:
            recvdata = self.socket.recv(size)
        except:
            recvdata = None

        if recvdata:
            self.curread += recvdata
            self.readpos += len(recvdata)

    def __handleRead(self):
        if self.readpos < HEAD_SIZE:
            self.__startRecv(HEAD_SIZE - self.readpos)

        if self.readpos >= HEAD_SIZE:
            bodylen = struct.unpack_from('i', self.curread, 0)[0]
            if self.readpos >= HEAD_SIZE + bodylen:
                self.reads.put(self.curread)
                self.curread = bytearray()
                self.readpos = 0
            else:
                self.__startRecv(HEAD_SIZE + bodylen - self.readpos)


    def __handleWrite(self):
        if self.cursend is None and not self.sends.empty():
            self.cursend = self.sends.get_nowait()
        if self.cursend is None:
            return

        self.sendpos += self.socket.send(self.cursend[self.sendpos:])
        if self.sendpos == len(self.cursend):
            self.sendpos = 0
            self.cursend = None

    def run(self):
        while True:
            self.__handleWrite()
            self.__handleRead()


    def send(self, buff):
        self.sends.put(buff)

    def getBuff(self):
        if self.reads.empty():
            return None
        return self.reads.get_nowait()

    def close(self):
        self.socket.close()



class TcpClient(TcpClientBase):
    def __init__(self, ip, port):
        TcpClientBase.__init__(self, ip, port)
        self.on_msg = None

    def set_on_msg(self, func):
        self.on_msg = func

    def start(self):
        TcpClientBase.connect(self)
        TcpClientBase.start(self)

    def update(self):
        buff = self.getBuff()
        if buff and self.on_msg:
            msgid = struct.unpack_from('i', buff, 4)[0]
            self.on_msg(msgid, buff)