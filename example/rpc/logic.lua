require "call"
require "role"

-- connect id
gconnid = 0
Logic = {
	roles = {}
}

function Logic:onConnect(conn)
	gconnid = gconnid + 1
	
	local role = Role:new()
	role.session = conn
	conn:setUsernum(gconnid)
	self.roles[gconnid] = role

	return role
end

function Logic:onClose(conn)
	local connid = conn:getUsernum()
	local role = self.roles[connid]
	role.session = nil
	self.roles[connid] = nil
end

function Logic:onMsg(conn, callid, pack)
	local connid = conn:getUsernum()
	local role = self.roles[connid]
	
	-- call
	local func = getcallfu(callid)
	func(role, pack)
end

-- role id
groleid = 1000;
function role_enter(role)
	groleid = groleid + 1
	role.roleid = groleid

	-- server exec
	print("role[".. groleid .. "] enter server")

	local pack = NetPacket:new()
	pack:pushString("sync role[".. groleid .. "] data")
	role:call(sync_role, pack)
end

function sync_role(role, pack)
	-- client exec
	print(pack:getString())

	local ret = NetPacket:new()
	ret:pushString("hello server")
	role:call(msg_one, ret)
end

function msg_one(role, pack)
	-- server exec
	print(pack:getString())

	local ret = NetPacket:new()
	ret:pushString("hello client")
	role:call(msg_two, ret)
end

function msg_two(role, pack)
	-- client exec
	print(pack:getString())

	local ret = NetPacket:new()
	ret:pushString("bye bye!")
	role:call(msg_bye, ret)
end

function msg_bye(role, pack)
	-- server exec
	print(pack:getString())
end

register(sync_role)
register(msg_one)
register(msg_two)
register(msg_bye)