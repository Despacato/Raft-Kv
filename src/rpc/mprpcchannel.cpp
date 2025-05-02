#include "mprpcchannel.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <string>
#include "mprpccontroller.h"
#include "rpcheader.pb.h"
#include "util.h"

/*
header_size + service_name method_name args_size + args
*/

void MprpcChannel::CallMethod(const google::protobuf::MethodDescriptor *method,
                              google::protobuf::RpcController *controller, const google::protobuf::Message *request,
                              google::protobuf::Message *response, google::protobuf::Closure *done)
{
  if (m_clientFd == -1)
  {
    std::string errMsg;
    bool rt = newConnect(m_ip.c_str(), m_port, &errMsg);
    if (!rt)
    {
      DPrintf("[func-MprpcChannel::CallMethod]重连接ip：{%s} port{%d}失败", m_ip.c_str(), m_port);
      controller->SetFailed(errMsg);
      return;
    }
    else
    {
      DPrintf("[func-MprpcChannel::CallMethod]连接ip：{%s} port{%d}成功", m_ip.c_str(), m_port);
    }
  }

  const google::protobuf::ServiceDescriptor *sd = method->service();
  std::string service_name = sd->name();    // service_name
  std::string method_name = method->name(); // method_name

  // 获取参数的序列化字符串长度 args_size
  uint32_t args_size{};
  std::string args_str;
  if (request->SerializeToString(&args_str))
  {
    args_size = args_str.size();
  }
  else
  {
    controller->SetFailed("serialize request error!");
    return;
  }
  RPC::RpcHeader rpcHeader;
  rpcHeader.set_service_name(service_name);
  rpcHeader.set_method_name(method_name);
  rpcHeader.set_args_size(args_size);

  std::string rpc_header_str;
  if (!rpcHeader.SerializeToString(&rpc_header_str))
  {
    controller->SetFailed("serialize rpc header error!");
    return;
  }

  std::string send_rpc_str; // 用来存储最终发送的数据
  {

    google::protobuf::io::StringOutputStream string_output(&send_rpc_str);
    google::protobuf::io::CodedOutputStream coded_output(&string_output);

    coded_output.WriteVarint32(static_cast<uint32_t>(rpc_header_str.size()));

    coded_output.WriteString(rpc_header_str);
  }

  send_rpc_str += args_str;

  // 打印调试信息
  //    std::cout << "============================================" << std::endl;
  //    std::cout << "header_size: " << header_size << std::endl;
  //    std::cout << "rpc_header_str: " << rpc_header_str << std::endl;
  //    std::cout << "service_name: " << service_name << std::endl;
  //    std::cout << "method_name: " << method_name << std::endl;
  //    std::cout << "args_str: " << args_str << std::endl;
  //    std::cout << "============================================" << std::endl;

  while (-1 == send(m_clientFd, send_rpc_str.c_str(), send_rpc_str.size(), 0))
  {
    char errtxt[512] = {0};
    sprintf(errtxt, "send error! errno:%d", errno);
    std::cout << "尝试重新连接，对方ip：" << m_ip << " 对方端口" << m_port << std::endl;
    close(m_clientFd);
    m_clientFd = -1;
    std::string errMsg;
    bool rt = newConnect(m_ip.c_str(), m_port, &errMsg);
    if (!rt)
    {
      controller->SetFailed(errMsg);
      return;
    }
  }

  char recv_buf[1024] = {0};
  int recv_size = 0;
  if (-1 == (recv_size = recv(m_clientFd, recv_buf, 1024, 0)))
  {
    close(m_clientFd);
    m_clientFd = -1;
    char errtxt[512] = {0};
    sprintf(errtxt, "recv error! errno:%d", errno);
    controller->SetFailed(errtxt);
    return;
  }

  if (!response->ParseFromArray(recv_buf, recv_size))
  {
    char errtxt[1050] = {0};
    sprintf(errtxt, "parse error! response_str:%s", recv_buf);
    controller->SetFailed(errtxt);
    return;
  }
}

bool MprpcChannel::newConnect(const char *ip, uint16_t port, string *errMsg)
{
  int clientfd = socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == clientfd)
  {
    char errtxt[512] = {0};
    sprintf(errtxt, "create socket error! errno:%d", errno);
    m_clientFd = -1;
    *errMsg = errtxt;
    return false;
  }

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = inet_addr(ip);
  // 连接rpc服务节点
  if (-1 == connect(clientfd, (struct sockaddr *)&server_addr, sizeof(server_addr)))
  {
    close(clientfd);
    char errtxt[512] = {0};
    sprintf(errtxt, "connect fail! errno:%d", errno);
    m_clientFd = -1;
    *errMsg = errtxt;
    return false;
  }
  m_clientFd = clientfd;
  return true;
}

MprpcChannel::MprpcChannel(string ip, short port, bool connectNow) : m_ip(ip), m_port(port), m_clientFd(-1)
{
  if (!connectNow)
  {
    return;
  }
  std::string errMsg;
  auto rt = newConnect(ip.c_str(), port, &errMsg);
  int tryCount = 3;
  while (!rt && tryCount--)
  {
    std::cout << errMsg << std::endl;
    rt = newConnect(ip.c_str(), port, &errMsg);
  }
}