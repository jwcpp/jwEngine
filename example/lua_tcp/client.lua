event_init()
init_netpacket_pool(10)

-- create server
client = NetClient:new()
client.on_connect = function(conn, argv)
	print("on onnect:".. argv)
end

client.on_close = function(conn)
	print("close")
end

client.on_msg = function(conn, pack)
	local str = pack:getString()
	print(str)
	conn:sendPacket(1, pack)
end

client:connect("127.0.0.1", 3001, false, 1);

event_run()