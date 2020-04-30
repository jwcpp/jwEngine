require("types")
testmsg = {}
testmsg.__index = testmsg
function testmsg:read(buffer)
	if 4 + buffer:rpos() > buffer:wpos() then return false end
	self.id = buffer:getUint32();
	self.play = Player:new()
	if self.play:read(buffer) == false then return false end;
	local len_array = 0
	local temp_array = nil
	if 4 + buffer:rpos() > buffer:wpos() then return false end
	len_array = buffer:getInt32();
	for i = 1, len_array, 1 do
		if 4 + buffer:rpos() > buffer:wpos() then return false end
		temp_array = buffer:getInt32();
		self.array[i] = temp_array
	end
	local len_attrs = 0
	local temp_attrs = nil
	if 4 + buffer:rpos() > buffer:wpos() then return false end
	len_attrs = buffer:getInt32();
	for i = 1, len_attrs, 1 do
		temp_attrs = Attr:new()
		if temp_attrs:read(buffer) == false then return false end;
		self.attrs[i] = temp_attrs
	end
	local len_vstr = 0
	local temp_vstr = nil
	if 4 + buffer:rpos() > buffer:wpos() then return false end
	len_vstr = buffer:getInt32();
	for i = 1, len_vstr, 1 do
		temp_vstr = buffer:getString();
		self.vstr[i] = temp_vstr
	end
	return true;
end
function testmsg:write(buffer)
	buffer:pushUint32(self.id);
	self.play:write(buffer);
	local len_array = #self.array
	buffer:pushInt32(len_array);
	for i = 1, len_array, 1 do 
		buffer:pushInt32(self.array[i]);
	end
	local len_attrs = #self.attrs
	buffer:pushInt32(len_attrs);
	for i = 1, len_attrs, 1 do 
		self.attrs[i]:write(buffer);
	end
	local len_vstr = #self.vstr
	buffer:pushInt32(len_vstr);
	for i = 1, len_vstr, 1 do 
		buffer:pushString(self.vstr[i]);
	end
end
function testmsg:new()
	local o = {}
	setmetatable(o, testmsg)
	o.id = 0
	o.play = {}
	o.array = {}
	o.attrs = {}
	o.vstr = {}
	return o
end


