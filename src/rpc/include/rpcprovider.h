#pragma once
#include "google/protobuf/service.h"

class RpcProvider
{
public:
    void NotifyService(google::protobuf::Service *service);
    // 启动rpc服务,开始网络监听
    void Run();
};