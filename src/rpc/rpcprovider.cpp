#include "rpcprovider.h"
#include "mprpcapplication.h"
#include <google/protobuf/descriptor.h>
#include <google/protobuf/service.h>
#include <string>
#include "../rpcheader.pb.h"

void RpcProvider::NotifyService(google::protobuf::Service *service)
{
    ServiceInfo service_info;
    const google::protobuf::ServiceDescriptor *desc = service->GetDescriptor();
    std::string service_name = desc->name();
    int method_count = desc->method_count();

    LOG_INFO("service_name: %s", service_name.c_str());

    for (int i = 0; i < method_count; ++i)
    {
        const google::protobuf::MethodDescriptor *method = desc->method(i);
        std::string method_name = method->name();
        service_info.m_methodMap.insert({method_name, method});
    }
    service_info.m_service = service;
    m_serviceMap.insert({service_name, service_info});
}

void RpcProvider::Run()
{
    std::string ip = MprpcApplication::GetInstance().GetConfig().Load("rpcserverip");
    uint16_t port = static_cast<uint16_t>(std::stoi(MprpcApplication::GetInstance().GetConfig().Load("rpcserverport")));
    muduo::net::InetAddress address(ip, port);

    muduo::net::TcpServer server(&m_eventLoop, address, "RpcProvider");

    server.setConnectionCallback(std::bind(&RpcProvider::OnConnection, this, std::placeholders::_1));
    server.setMessageCallback(std::bind(&RpcProvider::OnMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    server.setThreadNum(4);

    std::cout << "RpcProvider is starting..." << std::endl;

    server.start();
    m_eventLoop.loop();
}

void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr &conn)
{
    if (!conn->connected())
    {
        conn->shutdown();
    }
}

/* 在框架内部，RpcProvider和Rpconsumer协商好之间通信用的protobuf数据类型 service_name method_name args 定义proto的message类型进行数据的序列化和反序列化
service_name method_name args_size
16UserServiceLoginzhangsan123456

header_size(4字节) + header_str + args_str
*/

// 已建立连接用户的读写事件回调，如果远程有一个rpc服务的调用方法，那么onmessage方法就会响应。
void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buffer, muduo::Timestamp time)
{

    std::string recv_buf = buffer->retrieveAllAsString();

    uint32_t header_size = 0;
    recv_buf.copy((char *)&header_size, 4, 0);
    std::string header_str = recv_buf.substr(4, header_size);

    mprpc::RpcHeader rpc_header;
    std::string service_name;
    std::string method_name;
    uint32_t args_size = 0;

    if (rpc_header.ParseFromString(header_str))
    {
        service_name = rpc_header.service_name();
        method_name = rpc_header.method_name();
        args_size = rpc_header.args_size();
    }
    else
    {
        std::cout << "rpc header parse error!" << std::endl;
        return;
    }

    std::string args_str = recv_buf.substr(4 + header_size, args_size);

    // 打印调试信息
    std::cout << "=================================" << std::endl;
    std::cout << "service_name: " << service_name << std::endl;
    std::cout << "method_name: " << method_name << std::endl;
    std::cout << "args_size: " << args_size << std::endl;
    std::cout << "=================================" << std::endl;

    auto it = m_serviceMap.find(service_name);
    if (it == m_serviceMap.end())
    {
        std::cout << "service not found!" << std::endl;
        return;
    }

    auto mit = it->second.m_methodMap.find(method_name);
    if (mit == it->second.m_methodMap.end())
    {
        std::cout << "method not found!" << std::endl;
        return;
    }
    google::protobuf::Service *service = it->second.m_service;
    const google::protobuf::MethodDescriptor *method = mit->second;

    google::protobuf::Message *request = service->GetRequestPrototype(method).New();
    google::protobuf::Message *response = service->GetResponsePrototype(method).New();

    google::protobuf::Closure *done = google::protobuf::NewCallback<RpcProvider, const muduo::net::TcpConnectionPtr &, google::protobuf::Message *>(this, &RpcProvider::SendRpcResponse, conn, response);

    service->CallMethod(method, nullptr, request, response, done);
}

void RpcProvider::SendRpcResponse(const muduo::net::TcpConnectionPtr &conn, google::protobuf::Message *response)
{
    std::string response_str;
    if (response->SerializeToString(&response_str))
    {
        conn->send(response_str);
    }
    else
    {
        std::cout << "response serialize error!" << std::endl;
    }
    conn->shutdown();
}
