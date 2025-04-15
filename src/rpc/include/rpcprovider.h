#pragma once
#include "google/protobuf/service.h"
#include <memory>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <muduo/net/InetAddress.h>
#include <string>
#include <unordered_map>

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

    // service服务类型信息
    struct ServiceInfo
    {
        google::protobuf::Service *m_service;                                                    // 服务对象
        std::unordered_map<std::string, const google::protobuf::MethodDescriptor *> m_methodMap; // 方法描述信息
    };
    // 存储注册成功的服务对象和其服务方法的所有信息
    std::unordered_map<std::string, ServiceInfo> m_serviceMap;
    // 新的socket连接回调
    void OnConnection(const muduo::net::TcpConnectionPtr &conn);
    void OnMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buffer, muduo::Timestamp time);
};