msg = [[<html>
<body>
<h1>hello world</h1>
</body>
</html>]]

event_init()

server = HttpServer:new()
server:listen("127.0.0.1", 80)
server:addGet("/", function(conn, data)
	local hp = HttpParam:new(data, '&')
	if hp:find("a") and hp:find("b") then
		local a = hp:getStr("a")
		local b = hp:getStr("b")
		print(a.."|"..b)
	end
	conn:autoMsg(msg)
end)


event_run()