cmake_minimum_required(VERSION 3.6)
project(Rein_interval)

# 如果代码需要支持C++11，就直接加上这句
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++0x")
# 如果想要生成的可执行文件拥有符号表，可以gdb调试，就直接加上这句
# add_definitions("-Wall -g")

# 编译子文件夹的CMakeLists.txt
# add_subdirectory(proto_pb2)

# set(SOURCE_FILES  data_structure.h generator.h generator.cpp printer.cpp printer.h util.h util.cpp rein.h rein.cpp)
# add_executable(main ${SOURCE_FILES})

aux_source_directory(. SRC_LIST)
add_executable(cmakeRB ${SRC_LIST})



# cmake -G "MinGW Makefiles" .
# mingw32-make
