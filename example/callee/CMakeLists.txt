

set(SRC_LIST 

    ./userservice.cpp
    ../user.pb.cc

)

add_library(provider ${SRC_LIST})

target_link_libraries(provider protobuf muduo_net muduo_base)