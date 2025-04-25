#include <iostream>
#include <string>
#include "user.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"
#include "logger.h"

class UserService : public fixbug::UserServiceRpc
{
public:
    bool Login(std::string name, std::string pwd)
    {
        std::cout << "Login request received: " << name << ", " << pwd << std::endl;
        return true;
    }

    bool Register(u_int32_t id, std::string name, std::string pwd)
    {
        std::cout << "Register request received: " << name << ", " << pwd << std::endl;
        return true;
    }

    void Login(::google::protobuf::RpcController *controller,
               const ::fixbug::LoginRequest *request,
               ::fixbug::LoginResponse *response,
               ::google::protobuf::Closure *done)
    {

        std::string name = request->name();
        std::string pwd = request->pwd();

        bool login_result = Login(name, pwd);

        fixbug::ResultCode *result = response->mutable_result();
        result->set_errcode(0);
        result->set_errmsg("Login successful");
        response->set_success(login_result);

        done->Run();
    }

    void Register(::google::protobuf::RpcController *controller,
                  const ::fixbug::RegisterRequest *request,
                  ::fixbug::RegisterResponse *response,
                  ::google::protobuf::Closure *done)
    {

        uint32_t id = request->id();
        std::string name = request->name();
        std::string pwd = request->pwd();

        bool register_result = Register(id, name, pwd);

        fixbug::ResultCode *result = response->mutable_result();
        result->set_errcode(0);
        result->set_errmsg("Register successful");
        response->set_success(register_result);

        done->Run();
    }
};

int main(int argc, char **argv)
{
    LOG_INFO("first log message");
    LOG_ERR("%s:%s:%d", __FILE__, __FUNCTION__, __LINE__);

    MprpcApplication::Init(argc, argv);

    RpcProvider provider;

    provider.NotifyService(new UserService());

    provider.Run();

    return 0;
}
//