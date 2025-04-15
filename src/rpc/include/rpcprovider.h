#pragma once
#include "google/protobuf/service.h"
#include <memory>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <muduo/net/InetAddress.h>

class RpcProvider
{
public:
    void NotifyService(google::protobuf::Service *service);
    // 启动rpc服务,开始网络监听
    void Run();

private:
    // 组合了TcpServer对象
    // std::unique_ptr<muduo::net::TcpServer> m_tcpServerPtr; // 网络服务对象
    muduo::net::EventLoop m_eventLoop; // 事件循环对象

    // 新的socket连接回调
    void OnConnection(const muduo::net::TcpConnectionPtr &conn); 
    void OnMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buffer, muduo::Timestamp time);
};