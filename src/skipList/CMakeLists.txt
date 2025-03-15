# 获取当前源文件所在目录的绝对路径
get_filename_component(SKIPLIST_DIR "${CMAKE_CURRENT_SOURCE_DIR}" ABSOLUTE)

# 将该目录下所有源文件添加到 SRC_LIST 变量中
#aux_source_directory(${SKIPLIST_DIR} SKIPLIST_LIST)
file(GLOB SKIPLIST_LIST ${SKIPLIST_DIR}/*.cpp)
set(src_skipList ${SKIPLIST_LIST} CACHE INTERNAL "Description of the variable")

# 添加测试可执行文件
add_executable(skiplist_test skiplist_test.cpp)