#include "mprpcchannel.h"
#include <iostream>
#include "user.pb.h"
#include "rpcheader.pb.h"
#include "mprpcapplication.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include "mprpccontroller.h"

/*
    hader_size + serverce_name method_name args_size + args
*/

void MprpcChannel::CallMethod(const google::protobuf::MethodDescriptor *method,
                              google::protobuf::RpcController *controller,
                              const google::protobuf::Message *request,
                              google::protobuf::Message *response,
                              google::protobuf::Closure *done)
{
    const google::protobuf::ServiceDescriptor *service_desc = method->service();
    const std::string service_name = service_desc->name();
    const std::string method_name = method->name();

    int args_size = 0;
    std::string args_str;
    if (request->SerializeToString(&args_str))
    {
        args_size = args_str.size();
    }
    else
    {
        std::cout << "request serialize error!" << std::endl;
        controller->SetFailed("request serialize error!");
        return;
    }
    mprpc::RpcHeader rpc_header;
    rpc_header.set_service_name(service_name);
    rpc_header.set_method_name(method_name);
    rpc_header.set_args_size(args_size);

    uint32_t header_size = 0;
    std::string header_str;
    if (rpc_header.SerializeToString(&header_str))
    {
        header_size = header_str.size();
    }
    else
    {
        std::cout << "rpc header serialize error!" << std::endl;
        controller->SetFailed("rpc header serialize error!");
        return;
    }

    std::string send_rpc_str;
    send_rpc_str.insert(0, std::string((char *)&header_size, 4));
    send_rpc_str += header_str;
    send_rpc_str += args_str;

    // 打印调试信息
    std::cout << "=================================" << std::endl;
    std::cout << "service_name: " << service_name << std::endl;
    std::cout << "method_name: " << method_name << std::endl;
    std::cout << "args_size: " << args_size << std::endl;
    std::cout << "header_size: " << header_size << std::endl;
    std::cout << "send_rpc_str: " << send_rpc_str << std::endl;
    std::cout << "=================================" << std::endl;

    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == client_fd)
    {
        std::cout << "create socket error!" << std::endl;
        controller->SetFailed("create socket error!");
        exit(EXIT_FAILURE);
    }

    std::string server_ip = MprpcApplication::GetInstance().GetConfig().Load("rpcserverip");
    uint16_t port = atoi(MprpcApplication::GetInstance().GetConfig().Load("rpcserverport").c_str());

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip.c_str());

    if (-1 == connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)))
    {
        std::cout << "connect error!" << std::endl;
        controller->SetFailed("connect error!");
        close(client_fd);
        exit(EXIT_FAILURE);
    }
    if (-1 == send(client_fd, send_rpc_str.c_str(), send_rpc_str.size(), 0))
    {
        std::cout << "send error!" << std::endl;
        controller->SetFailed("send error!");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    char recv_buf[1024] = {0};
    int recv_size = 0;
    if (-1 == (recv_size = recv(client_fd, recv_buf, sizeof(recv_buf), 0)))
    {
        std::cout << "recv error!" << std::endl;
        controller->SetFailed("recv error!");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    std::string response_str(recv_buf, 0, recv_size);
    if (!response->ParseFromString(response_str))
    {
        std::cout << "pase error response_str" << response_str << std::endl;
        controller->SetFailed("pase error response_str");
        close(client_fd);
        return;
    }

    close(client_fd);
}