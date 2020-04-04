require("testmsg")

event_init()
init_netpacket_pool(10)

-- create server
client = NetClient:new()
client.on_connect = function(conn)
	print("on onnect")
end

client.on_close = function(conn)
	print("close")
end

client.on_msg = function(conn, pack)

		msg = testmsg:new();
		msg:read(pack);

		print("id:"..msg.id);
		print("play.name :" .. msg.play.name);
		print("play.level:" .. msg.play.level);
		print("play.msgcount:".. msg.play.msgcount);

		for i = 1, #msg.array, 1 do
			print("array: " .. msg.array[i]);
		end

		for i = 1, #msg.attrs, 1 do
		
			print(i.. " attack:" ..msg.attrs[i].attack);
			print(i.. " hp:" .. msg.attrs[i].hp);
		end

		for i = 1, #msg.vstr, 1 do
		
			print("vstr: " .. msg.vstr[i]);
		end

		print("----------------------------------------");
		
		conn:sendPacket(1, pack)
end

client:connect("127.0.0.1", 3001, false);

event_run()