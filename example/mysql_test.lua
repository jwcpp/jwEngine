pool = DBThreadPool:new("127.0.0.1", "jw_test", "root", "1111", 3306)
pool:create(1)

func = function(result)
			while(result:fetch())
			do
				local id = result:getInt32()
				local num = result:getInt32()
				local name = result:getString()
				
				local str = "id:" .. id .. ", num:" .. num .. ", name:" .. name
				-- errorLog(str)
				print(str)
			end
		end

function exec()
	local sql = SqlCommand:new("select * from test where id = ?")
	sql:pushInt8(1)
	sql:setBackfunc(func)
	sql:addToPool(pool)
	local t = TEST.new()
end


exec()

event_init()

timer = UTimer:new()
timer:start(function ()
	pool:update()
end, 10, 10)

event_run()