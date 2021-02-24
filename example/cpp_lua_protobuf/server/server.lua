-- 父目录添加到包路径
package.path = package.path .. ";..\\?.lua;"

local pb = require "pb"
local serpent = require "liblua/serpent"

pb.loadfile("msgtest.pb")

-- init
event_init()
init_netpacket_pool(100)

-- create server
server = NetServer:new()
server.on_accept = function(conn)
	
end

server.on_close = function(conn)

end

server.on_msg = function(conn, msgtype, pack)
	-- 从二进制数据解析出实际消息
    local msg = assert(pb.decode("Family", pack:readData()))
    print(serpent.block(msg))

	---------------> return data to 

	local data = assert(pb.encode("Family", msg))
	conn:sendMsg(111, data);
end

server:listen("127.0.0.1", 3001, false)

event_run()