-- init
event_init()
init_wspacket_pool(10)

-- create server
server = WebSocketServer:new()
server.on_accept = function(conn)
	print("connect")
end

server.on_close = function(conn)
	print("close")
end

server.on_msg = function(conn, pack)
	--local p = WebSocketPacket:new()
	local str = pack:getString()
	print(utf8_gbk(str))
	pack:pushString("->hello")
	conn:sendPacket(pack)
end

server:listen("127.0.0.1", 8080, false)

event_run()