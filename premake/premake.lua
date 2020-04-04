--solution "jwEngine"
workspace "jwEngine"
location "../"
configurations { "Debug", "Release" }
platforms {'x32', 'x64'}

-- global scope, all workspaces will receive these values
 if os.target() == "windows" then
    defines '_CRT_SECURE_NO_WARNINGS'
 end

project "libuv"
	-- 工程生成目录
	location "../dependencies"
	-- 附加包含目录
	includedirs{
		"../dependencies/libuv/include",
		"../dependencies/libuv/src" --有两个头文件
	}
	language "C++"
	kind "StaticLib"
	local codedir = "../dependencies/libuv/src";
	files { codedir.."/**.h",codedir.."/**.hpp", codedir.."/**.c", codedir.."/**.cc", codedir.."/**.cpp"}
	removefiles {codedir .. "/unix/**"}
	targetdir "../libs"
	filter "configurations:Debug"
		targetname "d_libuv"
	filter "configurations:Release"
		targetname "r_libuv"

project "liblua"
	-- 工程生成目录
	location "../dependencies"
	-- 附加包含目录
	includedirs{
		"../dependencies/lua/include",
	}
	language "C++"
	kind "StaticLib"
	local codedir = "../dependencies/lua";
	files { codedir.."/**.h",codedir.."/**.hpp", codedir.."/**.c", codedir.."/**.cc", codedir.."/**.cpp"}
	removefiles {codedir .. "/testes/**"}
	removefiles {codedir .. "/lua.c"}
	removefiles {codedir .. "/onelua.c"}
	targetdir "../libs"
	filter "configurations:Debug"
		targetname "d_liblua"
	filter "configurations:Release"
		targetname "r_liblua"
		
project "hiredis"
	-- 工程生成目录
	location "../dependencies"
	-- 附加包含目录
	includedirs{
		--"../dependencies/lua/include",
	}
	language "C++"
	kind "StaticLib"
	local codedir = "../dependencies/hiredis";
	files { codedir.."/**.h",codedir.."/**.hpp", codedir.."/**.c", codedir.."/**.cc", codedir.."/**.cpp"}
	removefiles {codedir .. "/test.c"}
	removefiles {codedir .. "/ssl.c"}
	removefiles {codedir .. "/examples/**"}
	targetdir "../libs"
	filter "configurations:Debug"
		targetname "d_redis"
	filter "configurations:Release"
		targetname "r_redis"
		
project "engine"
	-- 工程生成目录
	location "../src"
	-- 附加包含目录
	includedirs{
		"../dependencies",
		"../dependencies/sol2/include",
		"../dependencies/lua",
		"../dependencies/mysql",
		"../dependencies/hiredis",
		"../src/common",
		"../src/network",
		"../src/db"
	}
	language "C++"
	kind "ConsoleApp"
	local codedir = "../src/common";
	files { codedir.."/**.h",codedir.."/**.hpp", codedir.."/**.c", codedir.."/**.cc", codedir.."/**.cpp"}
	local codedir = "../src/network";
	files { codedir.."/**.h",codedir.."/**.hpp", codedir.."/**.c", codedir.."/**.cc", codedir.."/**.cpp"}
	local codedir = "../src/db";
	files { codedir.."/**.h",codedir.."/**.hpp", codedir.."/**.c", codedir.."/**.cc", codedir.."/**.cpp"}
	local codedir = "../src/script";
	files { codedir.."/**.h",codedir.."/**.hpp", codedir.."/**.c", codedir.."/**.cc", codedir.."/**.cpp"}
	libdirs{"../libs"}
	targetdir "../bin"
	filter "configurations:Debug"
		links {'ws2_32'}
		links {'Iphlpapi'}
		links {'Psapi'}
		links {'Userenv'}
		links {'libmysqlclient'}
		links {'d_liblua'}
		links {'d_libuv'}
		links {'d_redis'}
	filter "configurations:Release"
		links {'r_liblua'}
		links {'r_libuv'}
		links {'r_redis'}
		
project "serialization"
	location "../src"
	language "C++"
	kind "ConsoleApp"
	local codedir = "../src/serialization";
	files { codedir.."/**.h",codedir.."/**.hpp", codedir.."/**.c", codedir.."/**.cc", codedir.."/**.cpp"}
	targetdir "../tool"