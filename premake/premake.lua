--solution "jwEngine"
workspace "libs"
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
