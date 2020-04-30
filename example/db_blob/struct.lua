test_blob = {}
test_blob.__index = test_blob
function test_blob:read(buffer)
	if 4 + buffer:rpos() > buffer:wpos() then return false end
	self.id = buffer:getInt32();
	self.str = buffer:getString();
	local len_vec = 0
	local temp_vec = nil
	if 4 + buffer:rpos() > buffer:wpos() then return false end
	len_vec = buffer:getInt32();
	for i = 1, len_vec, 1 do
		if 4 + buffer:rpos() > buffer:wpos() then return false end
		temp_vec = buffer:getInt32();
		self.vec[i] = temp_vec
	end
	return true;
end
function test_blob:write(buffer)
	buffer:pushInt32(self.id);
	buffer:pushString(self.str);
	local len_vec = #self.vec
	buffer:pushInt32(len_vec);
	for i = 1, len_vec, 1 do 
		buffer:pushInt32(self.vec[i]);
	end
end
function test_blob:new()
	local o = {}
	setmetatable(o, test_blob)
	o.id = 0
	o.str = ""
	o.vec = {}
	return o
end


