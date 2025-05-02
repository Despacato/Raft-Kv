

set(SRC_LIST 

    ${CMAKE_CURRENT_SOURCE_DIR}/userservice.cpp
    ../user.pb.cc

)

add_executable(provider ${SRC_LIST})

target_link_libraries(provider mprpc protobuf muduo_net muduo_base)