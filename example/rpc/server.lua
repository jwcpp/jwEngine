require "logic"

-- init
event_init()

-- create server
server = NetServer:new()
server.on_accept = function(conn)
	role_enter(Logic:onConnect(conn))
end

server.on_close = function(conn)
	Logic:onClose(conn)
end

server.on_msg = function(conn, msgtype, pack)
	Logic:onMsg(conn, msgtype, pack)
end

server:listen("127.0.0.1", 3001, false)

event_run()