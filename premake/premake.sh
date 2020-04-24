# 1. premake5 生成makefile
# 2. make 编译
./premake5 --file=premake.lua --cc=gcc gmake
../make config=release_x64