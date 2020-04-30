Player = {}
Player.__index = Player
function Player:read(buffer)
	self.name = buffer:getString();
	if 4 + buffer:rpos() > buffer:wpos() then return false end
	self.level = buffer:getInt32();
	if self.level>=30 then
		if 4 + buffer:rpos() > buffer:wpos() then return false end
		self.msgcount = buffer:getInt32();

	end
	return true;
end
function Player:write(buffer)
	buffer:pushString(self.name);
	buffer:pushInt32(self.level);
	if self.level>=30 then
		buffer:pushInt32(self.msgcount);

	end
end
function Player:new()
	local o = {}
	setmetatable(o, Player)
	o.name = ""
	o.level = 0
	o.msgcount = 0
	return o
end

Attr = {}
Attr.__index = Attr
function Attr:read(buffer)
	if 4 + buffer:rpos() > buffer:wpos() then return false end
	self.attack = buffer:getInt32();
	if 4 + buffer:rpos() > buffer:wpos() then return false end
	self.hp = buffer:getInt32();
	return true;
end
function Attr:write(buffer)
	buffer:pushInt32(self.attack);
	buffer:pushInt32(self.hp);
end
function Attr:new()
	local o = {}
	setmetatable(o, Attr)
	o.attack = 0
	o.hp = 0
	return o
end


