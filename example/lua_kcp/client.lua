event_init()

-- create server
client = KcpClient:new()
client.on_connect = function(conn, argv)
	print("on onnect:".. argv)
end

client.on_close = function(conn)
	print("close")
end

count = 0
client.on_msg = function(conn, msgtype, pack)
	local str = pack:getString()
	--print(str)
	conn:sendPacket(1, pack)
	count = count + 1
end

client:createSession("127.0.0.1", 3001, false);

t = UTimer:new()
t:start(function()
	print(count)
	count = 0
end, 1000,1000)

event_run()