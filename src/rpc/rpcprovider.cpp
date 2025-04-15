#include "rpcprovider.h"
#include "mprpcapplication.h"
#include <google/protobuf/descriptor.h>
#include <google/protobuf/service.h>
#include <string>

void RpcProvider::NotifyService(google::protobuf::Service *service)
{
    ServiceInfo service_info;
    //获取服务对象的描述信息
    const google::protobuf::ServiceDescriptor *desc = service->GetDescriptor();
    //获取服务名称
    std::string service_name = desc->name();
    //获取服务方法数量
    int method_count = desc->method_count();
    //获取服务方法
    for (int i = 0; i < method_count; ++i)
    {
        //获取方法描述信息
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
    
    //创建TcpServer对象
    muduo::net::TcpServer server(&m_eventLoop, address, "RpcProvider");
    //绑定连接回调和消息读写回调方法
    server.setConnectionCallback(std::bind(&RpcProvider::OnConnection, this, std::placeholders::_1));
    server.setMessageCallback(std::bind(&RpcProvider::OnMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    //设置muduo库的线程数量
    server.setThreadNum(4);

    std::cout << "RpcProvider is starting..." << std::endl;
    // 启动网络服务
    server.start();
    m_eventLoop.loop(); // 事件循环

    
}


void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr &conn)
{
  
}
void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buffer, muduo::Timestamp time)
{
    
}
