# 设置cmake的最低版本和项目名称
cmake_minimum_required(VERSION 3.22)
project(KVRaftCpp)

set(CMAKE_CXX_STANDARD 20)
# 生成debug版本，可以进行gdb调试
set(CMAKE_BUILD_TYPE "Debug")


# 设置项目可执行文件输出的路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# 设置项目库文件输出的路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#项目头文件的搜索路径
include_directories(${PROJECT_SOURCE_DIR}/example/include)
include_directories(${PROJECT_SOURCE_DIR}/src/rpc/include)
include_directories(${PROJECT_SOURCE_DIR}/example)
include_directories(${PROJECT_SOURCE_DIR}/src/raftCore/include)
include_directories(${PROJECT_SOURCE_DIR}/src/raftClerk/include)
include_directories(${PROJECT_SOURCE_DIR}/src/skipList/include)

#项目库文件的搜索路径
link_directories(${PROJECT_SOURCE_DIR}/lib)

#SRC
add_subdirectory(src)
add_subdirectory(example)
#add_subdirectory(test)
#EXAMPLE
#add_subdirectory(example)

#add_library(skip_list_on_raft STATIC  ${src_rpc} ${src_raftCore} )
#target_link_libraries(skip_list_on_raft muduo_net muduo_base pthread dl)
# 添加格式化目标 start
# from : https://blog.csdn.net/guotianqing/article/details/121661067

