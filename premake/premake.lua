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

project "common"
	-- 工程生成目录
	location "../common"
	-- 附加包含目录
	includedirs{
		"../dependencies"
	}
	language "C++"
	kind "StaticLib"
	local codedir = "../common/";
	files { codedir.."/**.h",codedir.."/**.hpp", codedir.."/**.c", codedir.."/**.cc", codedir.."/**.cpp"}
	libdirs{"../libs"}
	targetdir "../libs"
	filter "configurations:Debug"
		links {'d_libuv'}
		targetname "d_common"
	filter "configurations:Release"
		links {'r_libuv'}
		targetname "r_common"
		
project "network"
	-- 工程生成目录
	location "../network"
	-- 附加包含目录
	includedirs{
		"../dependencies",
		"../common"
	}
	language "C++"
	kind "StaticLib"
	local codedir = "../network/";
	files { codedir.."/**.h",codedir.."/**.hpp", codedir.."/**.c", codedir.."/**.cc", codedir.."/**.cpp"}
	libdirs{"../libs"}
	targetdir "../libs"
	filter "configurations:Debug"
		links {'d_libuv'}
		links {'d_common'}
		targetname "d_network"
	filter "configurations:Release"
		links {'r_libuv'}
		links {'r_common'}
		targetname "r_network"
		
project "example"
	-- 工程生成目录
	location "../example"
	-- 附加包含目录
	includedirs{
		"../dependencies",
		"../common",
		"../network"
	}
	language "C++"
	kind "ConsoleApp"
	local codedir = "../example/";
	files { codedir.."/**.h",codedir.."/**.hpp", codedir.."/**.c", codedir.."/**.cc", codedir.."/**.cpp"}
	libdirs{"../libs"}
	filter "configurations:Debug"
		links {'d_libuv'}
		links {'ws2_32'}
		links {'Iphlpapi'}
		links {'Psapi'}
		links {'Userenv'}
		links {'d_common'}
		links {'d_network'}
	filter "configurations:Release"
		links {'r_libuv'}
		links {'r_common'}
		links {'r_network'}