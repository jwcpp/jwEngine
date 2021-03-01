-- init
event_init()

-- create server
server = KcpServer:new()
server.on_accept = function(conn)
	local pack = KcpPacket:new()
	pack:pushString("hello world, hello c++")
	conn:sendPacket(1, pack)
end

server.on_close = function(conn)
	print("close")
end

server.on_msg = function(conn, msgtype, pack)
	local str = pack:getString()
	--print(str)
	conn:sendPacket(1, pack)
end

server:start("127.0.0.1", 3001, false)

event_run()