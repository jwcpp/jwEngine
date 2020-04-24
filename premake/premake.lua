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
	targetdir "../libs"
	local codedir = "../dependencies/libuv/src";
	files { codedir.."/*.h",codedir.."/*.hpp", codedir.."/*.c", codedir.."/*.cc", codedir.."/*.cpp"}
	filter "system:windows"
		local windir = codedir .. "/win"
		files { windir.."/*.h",windir.."/*.hpp", windir.."/*.c", windir.."/*.cc", windir.."/*.cpp"}
	filter "system:not windows"
		local lindir = codedir .. "/unix"
		files {
		   lindir .. "/async.c",
		   lindir .. "/core.c",
		   lindir .. "/dl.c",
		   lindir .. "/fs.c",
		   lindir .. "/getaddrinfo.c",
		   lindir .. "/getnameinfo.c",
		   lindir .. "/loop-watcher.c",
		   lindir .. "/loop.c",
		   lindir .. "/pipe.c",
		   lindir .. "/poll.c",
		   lindir .. "/process.c",
		   lindir .. "/random-devurandom.c",
		   lindir .. "/signal.c",
		   lindir .. "/stream.c",
		   lindir .. "/tcp.c",
		   lindir .. "/thread.c",
		   lindir .. "/tty.c",
		   lindir .. "/udp.c",
		   ----------
		   lindir .. "/proctitle.c",
		   ----------
		   lindir .. "/linux-core.c",
		   lindir .. "/linux-inotify.c",
		   lindir .. "/linux-syscalls.c",
		   lindir .. "/procfs-exepath.c",
		   lindir .. "/random-getrandom.c",
		   lindir .. "/random-sysctl-linux.c",
		   lindir .. "/sysinfo-loadavg.c"
		}
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
	files { codedir.."/*.h",codedir.."/*.hpp", codedir.."/*.c", codedir.."/*.cc", codedir.."/*.cpp"}
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
		"../dependencies/libuv/include",
		"../dependencies/sol2/include",
		"../dependencies/lua",
		"../dependencies/mysql",
		"../dependencies/hiredis",
		"../src/common",
		"../src/network",
		"../src/db"
	}
	language "C++"
	buildoptions {"-std=c++17"}
	linkoptions {"-pthread"}
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
	filter "system:windows"
		links {'ws2_32'}
		links {'Iphlpapi'}
		links {'Psapi'}
		links {'Userenv'}
		links {'libmysqlclient'}
	filter "system:not windows"
		links {'pthread'}
		links {'mysqlclient'}
		links {'dl', 'rt'}
	filter "configurations:Debug"
		links {'d_liblua'}
		links {'d_libuv'}
		links {'d_redis'}
	filter "configurations:Release"
		links {'r_liblua'}
		links {'r_libuv'}
		links {'r_redis'}
	filter {}
		
		
project "serialization"
	location "../src"
	language "C++"
	kind "ConsoleApp"
	local codedir = "../src/serialization";
	files { codedir.."/**.h",codedir.."/**.hpp", codedir.."/**.c", codedir.."/**.cc", codedir.."/**.cpp"}
	targetdir "../tool"