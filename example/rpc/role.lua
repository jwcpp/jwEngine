require "call"

Role = {}
Role.__index = Role

function Role:new()
	local o = {}
	setmetatable(o, Role)
	o.session = nil
	o.roleid = 0
	return o
end

function Role:call(func, pack)
	local callid = getcallid(func)
	self.session:sendPacket(callid, pack)
end
