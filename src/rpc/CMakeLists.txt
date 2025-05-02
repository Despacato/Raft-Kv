# 获取当前源文件所在目录的绝对路径


# 将该目录下所有源文件添加到 SRC_LIST 变量中
#aux_source_directory(${RPC_DIR} RPC_LIST)

set(RPC_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/mprpcchannel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mprpcconfig.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mprpccontroller.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rpcheader.pb.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/rpcprovider.cpp     
)

#生成动态库给其他用户使用
add_library(rpc_lib ${RPC_LIST} ${src_common})
target_link_libraries(rpc_lib boost_serialization)
set(src_rpc ${RPC_LIST} CACHE INTERNAL "Description of the variable")

