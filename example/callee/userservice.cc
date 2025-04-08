#include <iostream>
#include <string>
#include "user.pb.h" // 这里包含了生成的头文件

class UserService : public fixbug::UserServiceRpc // 继承自生成的 UserService 类
{
public:
    UserService() = default;
    ~UserService() = default;

    bool Login(std::string name, std::string pwd)
    {
        // 处理登录请求
        std::cout << "Login request received: " << name << ", " << pwd << std::endl;
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

int main()
{
    return 0;
}
//