import socket_buffer
import net
import time

def onMsg(type, buff):
	print("------------------->"+str(type))
	rpack = socket_buffer.NetPacket()
	rpack.receive(buff)
	print(rpack.getInt32())
	print(rpack.getString())
	print(rpack.getInt16())

client = net.TcpClient("127.0.0.1", 3001)
client.set_on_msg(onMsg)
client.start()

pack = socket_buffer.NetPacket()
pack.pushString("abcdefg")
pack.pushInt32(1314)
pack.pushString("jwEngine")
pack.pushInt32(520)
pack.setHead(100011)

client.send(pack.getSendData())


while True:
	time.sleep(0.1)
	client.update()


client.close()