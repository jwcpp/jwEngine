-- init
event_init()

-- create server
server = WebSocketServer:new()
server.on_accept = function(conn)
	print("connect")
end

server.on_close = function(conn)
	print("close")
end

server.on_msg = function(conn, pack)
	print(pack:getInt32())
	print(utf8_gbk(pack:getString()))
	print(pack:getInt32())

	local retpak = WebSocketPacket:new()
	retpak:pushString('hello client')
	conn:sendPacket(retpak)
end

server:listen("127.0.0.1", 8080, false)

event_run()