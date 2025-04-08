#include <iostream>
#include <string>
#include "user.pb.h"          // 这里包含了生成的头文件
#include "mprpcapplication.h" // MprpcApplication类的头文件
#include "rpcprovider.h"      // RpcProvider类的头文件

class UserService : public fixbug::UserServiceRpc // 继承自生成的 UserService 类
{
public:
       bool Login(std::string name, std::string pwd)
    {
        // 处理登录请求
        std::cout << "Login request received: " << name << ", " << pwd << std::endl;
        return true; // 假设登录成功
    }

    void Login(::google::protobuf::RpcController *controller,
               const ::fixbug::LoginRequest *request,
               ::fixbug::LoginResponse *response,
               ::google::protobuf::Closure *done)
    {
        // 处理登录请求
        std::string name = request->name();
        std::string pwd = request->pwd();

        bool login_result = Login(name, pwd);

        // 设置响应结果
        fixbug::ResultCode *result = response->mutable_result();
        result->set_errcode(0);
        result->set_errmsg("Login successful");
        response->set_success(login_result);

        // 调用完成回调 //执行响应对象数据的序列化和网络发送
        done->Run();
    }
};

int main(int argc, char **argv)
{
    // 初始化rpc网络服务对象
    // 这里的MprpcApplication::Init(argc,argv)是一个初始化函数
    // 它会解析命令行参数，并初始化rpc网络服务对象
    // 这个函数的实现是在MprpcApplication类中
    MprpcApplication::Init(argc, argv);

    // provider 是一个rpc网络服务对象，把UserSErvice对象注册到rpc网络服务对象上
    //  这样rpc网络服务对象就可以通过UserService对象来处理rpc请求了
    RpcProvider provider;
    provider.NotifyService(new UserService());
    // 启动rpc服务,进入阻塞状态，等待rpc请求到来
    provider.Run();
    return 0;
}
//