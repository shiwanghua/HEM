cmake_minimum_required(VERSION 3.6)
project(MAC)

# 设置用debug还是release模式。debug允许断点，而release更快
#set( CMAKE_BUILD_TYPE Debug )
set( CMAKE_BUILD_TYPE Release )

#FIND_PACKAGE(OpenMP REQUIRED)
#if(OPENMP_FOUND)
#    message("OPENMP FOUND")
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -O3 -std=c++0x")
#endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
#set(CMAKE_C_COMPILER /usr/bin/mpicc)
#set(CMAKE_CXX_COMPILER /usr/bin/mpic++)
#SET(CMAKE_CXX_COMPILER "g++")
#find_package(MPI REQUIRED)

# 按地址把MPI库包含进来
#include_directories(${MPI_INCLUDE_PATH})
#message("MPI_INCLUDE_PATH: ${MPI_INCLUDE_PATH}")

#set(CMAKE_CXX_STANDARD 11 )

#SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++0x")
#set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3" )
LINK_LIBRARIES(m)
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g -march=native -mavx2  -O3 -pthread ") #-mavx2  -static -L /usr/local/lib/  -lboost_system -lboost_thread -lpthread  -g  -DDEBUG -mavx512bw -mavx512vl -mavx512f -mavx512cd -mavx512dq -msse -lboost_thread -lboost_system -std=c++2a
#set(CMAKE_C_STANDARD 17)
#set(CMAKE_CXX_FLAGS_DEBUG “${CMAKE_CXX_FLAGS_DEBUG} -O0”)
#set(CMAKE_C_FLAGS_DEBUG “${CMAKE_C_FLAGS_DEBUG} -O0”)
# 如果想要生成的可执行文件拥有符号表，可以gdb调试，就直接加上这句
# add_definitions("-Wall -g")

# 编译子文件夹的CMakeLists.txt
# add_subdirectory(proto_pb2)

# set(SOURCE_FILES  data_structure.h generator.h generator.cpp printer.cpp printer.h util.h util.cpp rein.h rein.cpp)
# add_executable(main ${SOURCE_FILES})

aux_source_directory(. SRC_LIST)
add_executable(run ${SRC_LIST} BGTree_d_vrs.cpp BGTree_d_vrs.h ThreadPool.h thpool.cpp thpool.h) # pRein.cpp pRein.h
# target_link_libraries(hello ${MPI_LIBRARIES})
target_link_libraries(run PUBLIC Threads::Threads)

# cmake -G "MinGW Makefiles" .
# mingw32-make
