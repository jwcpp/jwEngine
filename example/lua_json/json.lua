local str = "{\"num\": 11, \"obj\":{\"a\":12, \"b\":13}, \"array\":[1314,520]}"
local buff = JsonBuff:new(str)
local jobj = json_parser(buff)
if jobj then
	-- C++ returns the parent class, need convert to subclass
	obj = json_to_obj(jobj)
	print("num:" .. obj:getInt("num") .. "\n")
	
	local str = json_write(jobj, true)
	print(str)
else
	print("parser error:" .. json_error(buff))
end

local json = require "jsonlib/json"

local tab = json.decode(str) -- json to lua table
local jstr = json.encode(tab) -- lua table to json
print(jstr)