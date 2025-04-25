#include <iostream>
#include "mprpcapplication.h"

int main(int argc, char **argv)
{

    MprpcApplication::Init(argc, argv);
    fixbug::UserServiceRpc_Stub stub(new MprpcChannel());
    fixbug::LoginRequest request;
    request.set_name("zhangsan");
    request.set_pwd("123456");

    fixbug::LoginResponse response;
    MprpcController controller;
    stub.Login(&controller, &request, &response, nullptr);

    if (!controller.Failed())
    {
        if (0 == response.result().errcode())
        {
            std::cout << "rpc login response success!" << std::endl;
        }
        else
        {
            std::cout << "rpc login response failed!" << std::endl;
        }
    }
    else
    {
        std::cout << controller.ErrorText() << std::endl;
    }

    fixbug::RegisterRequest register_request;
    register_request.set_id(1001);
    register_request.set_name("lisi");
    register_request.set_pwd("123456");

    fixbug::RegisterResponse register_response;
    stub.Register(nullptr, &register_request, &register_response, nullptr);

    if (controller.Failed())
    {
        std::cout << controller.ErrorText() << std::endl;
    }
    else
    {
        if (0 == register_response.result().errcode())
        {
            std::cout << "rpc register response success!" << std::endl;
        }
        else
        {
            std::cout << "rpc register response failed!" << std::endl;
        }
        return 0;
    }
}