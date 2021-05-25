require("struct")

local config = DBConfig:new()
config.device = "mysql"
config.ip = "192.168.1.77"
config.dbname = "jw_test"
config.user = "root"
config.pswd = "1111"
config.port = 3306

pool = DBThreadPool:new(config)
pool:create(1)

function select()
	local sql = SqlCommand:new("select id, blo from t_blob")
	local func = function(errno, err, result)
			while(result:fetch())
			do
				print("tableid:" .. result:getInt32())
				local pack = BasePacket:new()
				if result:readBlob(pack) > 0 then
					local st = test_blob:new()
					st:read(pack)
					print("id:".. st.id .. ", str:" .. st.str)
					for i = 1, #st.vec, 1 do
						print(st.vec[i])
					end
				end
			end
		end
	sql:addToPool(pool, func)
end

function insert()
	local sql = SqlCommand:new("insert into t_blob(id, blo) values(?, ?)")
	sql:pushInt32(1)
	
	local st = test_blob:new()
	st.id = 1314
	st.str = "hello c++20"
	st.vec = {1314,520,666}
	local pack = BasePacket:new()
	st:write(pack)
	sql:pushBlob(pack)
	sql:addToPool(pool, function(errno, err,result)
			select()
		end)
end

select()

event_init()

timer = UTimer:new()
timer:start(function ()
	pool:update()
end, 10, 10)

event_run()