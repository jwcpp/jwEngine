import struct

class BaseBuffer:
	def __init__(self, size):
		self.rpos = 0
		self.wpos = size
		self.buff = bytearray(size)

	def write(self, fmt, value, size):
		self.buff += struct.pack(fmt,value)
		self.wpos += size

	def read(self, fmt, size):
		ret = struct.unpack_from(fmt,self.buff, self.rpos)
		self.rpos += size
		return ret

	def pushInt8(self, value):
		self.write('b',value, 1)
	def pushUint8(self, value):
		self.write('B',value, 1)
	def pushInt16(self, value):
		self.write('h',value, 2)
	def pushUint16(self, value):
		self.write('H',value, 2)
	def pushInt32(self, value):
		self.write('i',value, 4)
	def pushUint32(self, value):
		self.write('I',value, 4)
	def pushInt64(self, value):
		self.write('q',value, 8)
	def pushUint64(self, value):
		self.write('Q',value, 8)
	def pushFloat(self, value):
		self.write('f',value, 4)
	def pushDouble(self, value):
		self.write('d',value, 8)
	def pushString(self, value):
		value += '\0'
		self.buff += value.encode('utf-8')
		self.wpos += len(value)

	def getInt8(self):
		return self.read('b', 1)[0]
	def getUint8(self):
		return self.read('B', 1)[0]
	def getInt16(self):
		return self.read('h', 2)[0]
	def getUint16(self):
		return self.read('H', 2)[0]
	def getInt32(self):
		return self.read('i', 4)[0]
	def getUint32(self):
		return self.read('I', 4)[0]
	def getInt64(self):
		return self.read('q', 8)[0]
	def getUint64(self):
		return self.read('Q', 8)[0]
	def getFloat(self):
		return self.read('f', 4)[0]
	def getDouble(self):
		return self.read('d', 8)[0]
	def getString(self):
		slen = 0
		for i in range(len(self.buff) - self.rpos):
			char_ = self.buff[self.rpos + i]
			if char_ == 0:
				break
			slen += 1
		by = self.buff[self.rpos: self.rpos + slen]
		self.rpos += (slen + 1)
		return by.decode()

class NetPacket(BaseBuffer):
	def __init__(self):
		BaseBuffer.__init__(self, 8)
	
	def receive(self, buff):
		self.rpos = 8
		self.wpos = len(buff)
		self.buff = buff

	def setHead(self, msgtype):
		struct.pack_into('I',self.buff,0, self.wpos - 8)
		struct.pack_into('I',self.buff,4,msgtype)

	def getSendData(self):
		return self.buff

	def getSendSize(self):
		return self.wpos