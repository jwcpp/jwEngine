local config = DBConfig:new()
config.device = "redis"
config.ip = "127.0.0.1"
config.port = 6379

pool = DBThreadPool:new(config)
pool:create(1)

func = function(errno, err, result)
	if errno ~= 0 then
		print(err)
		return
	end

	while(result:fetch())
	do
		local str = result:getString()
		-- errorLog(str)
		print(str)
	end
end

function exec()
	local command = RedisCommand:new("lrange")
	command:pushString("t1")
	command:pushString("0")
	command:pushString("10")
	command:addToPool(pool, func)
end

exec()
event_init()

timer = UTimer:new()
timer:start(function ()
	pool:update()
end, 10, 10)

event_run()