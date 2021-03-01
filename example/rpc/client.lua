require "logic"

event_init()

-- create server
client = NetClient:new()
client.on_connect = function(conn, argv)
	Logic:onConnect(conn)
end

client.on_close = function(conn)
	Logic:onClose(conn)
end

client.on_msg = function(conn, msgtype, pack)
	Logic:onMsg(conn, msgtype, pack)
end

client:connect("127.0.0.1", 3001, false, 1);

event_run()