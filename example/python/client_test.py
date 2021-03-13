import socket
import socket_buffer

client = socket.socket()
client.connect(('localhost',3001))

pack = socket_buffer.NetPacket()
pack.pushString("abcdefg")
pack.pushInt32(1314)
pack.pushString("jwEngine")
pack.pushInt32(520)
pack.setHead(100011)

client.send(pack.getSendData())
# client.send(cmd.encode("utf-8"))
data = client.recv(1024)
rpack = socket_buffer.NetPacket()
rpack.receive(data)
print(rpack.getInt32())
print(rpack.getString())
print(rpack.getInt16())

client.close()