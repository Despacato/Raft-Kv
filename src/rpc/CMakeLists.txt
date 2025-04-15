# 获取当前源文件所在目录的绝对路径


# 将该目录下所有源文件添加到 SRC_LIST 变量中
#aux_source_directory(${RPC_DIR} RPC_LIST)

set(RPC_LIST
    ./mprpcapplication.cpp
    ./mprpcconfig.cpp
    ./rpcprovider.cpp
)

#生成动态库给其他用户使用
add_library(mprpc STATIC ${RPC_LIST})
target_link_libraries(mprpc provider protobuf muduo_net muduo_base)


