#include "rpcprovider.h"
#include "mprpcapplication.h"
#include <google/protobuf/descriptor.h>
#include <google/protobuf/service.h>
#include <string>
#include "../rpcheader.pb.h"

void RpcProvider::NotifyService(google::protobuf::Service *service)
{
    ServiceInfo service_info;
    // 获取服务对象的描述信息
    const google::protobuf::ServiceDescriptor *desc = service->GetDescriptor();
    // 获取服务名称
    std::string service_name = desc->name();
    // 获取服务方法数量
    int method_count = desc->method_count();
    // 获取服务方法
    for (int i = 0; i < method_count; ++i)
    {
        // 获取方法描述信息
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

    // 创建TcpServer对象
    muduo::net::TcpServer server(&m_eventLoop, address, "RpcProvider");
    // 绑定连接回调和消息读写回调方法
    server.setConnectionCallback(std::bind(&RpcProvider::OnConnection, this, std::placeholders::_1));
    server.setMessageCallback(std::bind(&RpcProvider::OnMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    // 设置muduo库的线程数量
    server.setThreadNum(4);

    std::cout << "RpcProvider is starting..." << std::endl;
    // 启动网络服务
    server.start();
    m_eventLoop.loop(); // 事件循环
}

void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr &conn)
{
    if (!conn->connected())
    {
        conn->shutdown(); // 关闭连接
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
    // 网络上接收的远程rpc调用请求的字符流
    std::string recv_buf = buffer->retrieveAllAsString();
    // 从字符流中解析出header_size
    uint32_t header_size = 0;
    recv_buf.copy((char *)&header_size, 4, 0);

    // 根据header_size读取数据头的原始字符流
    std::string header_str = recv_buf.substr(4, header_size);

    mprpc::RpcHeader rpc_header;
    std::string service_name;
    std::string method_name;
    uint32_t args_size = 0;

    if (rpc_header.ParseFromString(header_str))
    {
        // 数据头反序列化成功
        service_name = rpc_header.service_name();
        method_name = rpc_header.method_name();
        args_size = rpc_header.args_size();
    }
    else
    {
        std::cout << "rpc header parse error!" << std::endl;
        return;
    }
    // 根据rpc_header中的service_name和method_name获取服务对象
    // 获取rpc方法参数的字符流数据

    std::string args_str = recv_buf.substr(4 + header_size, args_size);

    // 打印调试信息
    std::cout << "=================================" << std::endl;
    std::cout << "service_name: " << service_name << std::endl;
    std::cout << "method_name: " << method_name << std::endl;
    std::cout << "args_size: " << args_size << std::endl;
    std::cout << "=================================" << std::endl;

    // 获取sercice对象和method对象
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

    // 生成rpc方法调用的请求request和响应response对象
    google::protobuf::Message *request = service->GetRequestPrototype(method).New();
    google::protobuf::Message *response = service->GetResponsePrototype(method).New();

    // 给下面的method方法的调用，绑定一个Closure的回调函数。
    google::protobuf::NewCallback();
    // 在框架上根据远端rpc请求，调用当前rpc节点上发布的方法

    service->CallMethod(method, nullptr, request, response, );
}

void RpcProvider::SendRpcResponse(const muduo::net::TcpConnectionPtr &conn, google::protobuf::Message *response)
{
    // 1.序列化响应数据
    std::string send_str;
    response->SerializeToString(&send_str);
    // 2.获取响应数据的大小
    uint32_t send_size = static_cast<uint32_t>(send_str.size());
    // 3.拼接rpc响应数据头
    mprpc::RpcHeader rpc_header;
    rpc_header.set_service_name("UserService");
    rpc_header.set_method_name("Login");
    rpc_header.set_args_size(send_size);
    std::string header_str;
    rpc_header.SerializeToString(&header_str);
    // 4.拼接rpc响应数据头和rpc响应数据
    std::string send_buf;
    send_buf.append((char *)&send_size, 4);
    send_buf.append(header_str);
    send_buf.append(send_str);

    // 5.发送rpc响应数据
    conn->send(send_buf);
}
