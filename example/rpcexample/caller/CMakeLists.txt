

set(SRC_LIST 

    ./calluserserver.cpp
    ../user.pb.cc
)

add_executable(consumer ${SRC_LIST})

target_link_libraries(consumer mprpc protobuf muduo_net muduo_base)