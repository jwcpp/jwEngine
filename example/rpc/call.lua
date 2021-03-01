rpcfunc = {}
rpcids = {}
function register(func)
	local callid = #rpcfunc + 1
	rpcfunc[callid] = func
	rpcids[func] = callid
end

function getcallid(func)
	return rpcids[func]
end

function getcallfu(callid)
	return rpcfunc[callid]
end