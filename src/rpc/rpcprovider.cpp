#include "rpcprovider.h"
#include "mprpcapplication.h"
#include <string>

void RpcProvider::NotifyService(google::protobuf::Service *service)
{

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
