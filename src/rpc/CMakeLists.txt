# 获取当前源文件所在目录的绝对路径
get_filename_component(RPC_DIR "${CMAKE_CURRENT_SOURCE_DIR}" ABSOLUTE)

# 将该目录下所有源文件添加到 SRC_LIST 变量中
#aux_source_directory(${RPC_DIR} RPC_LIST)
file(GLOB RPC_LIST ${RPC_DIR}/*.cpp)
set(src_rpc ${RPC_LIST} CACHE INTERNAL "Description of the variable")
