local g = Go.new()
local t = g:run()
print(t:getId())
g:backfunc(function(i)
	print("backfunc :" .. i)
end)

func({a = 111, b = 222})