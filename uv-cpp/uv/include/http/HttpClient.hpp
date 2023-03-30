/*
   Copyright © 2017-2020, orcaer@yeah.net  All rights reserved.

   Author: orcaer@yeah.net

   Last modified: 2020-3-30

   Description: https://github.com/wlgq2/uv-cpp
*/

#ifndef UV_HTTP_CLIENT_HPP
#define UV_HTTP_CLIENT_HPP

#include "../TcpClient.hpp"
#include "Request.hpp"
#include "Response.hpp"

namespace uv
{
 namespace http
 {

  class HttpClient
  {
  public:
   enum ReqResult
   {
    Success = 0,
    ConnectFail = 1,
    ParseFail = 2,
    Unknow = 3,
   };
   using OnRespCallback = std::function<void(ReqResult, Response*)>;
  public:
   HttpClient(EventLoop* loop);
   virtual ~HttpClient();

   virtual void Req(uv::SocketAddr& addr, Request& req);
   virtual void setOnResp(OnRespCallback callback);

  protected:
   TcpClient* client_;
   OnRespCallback callback_;
   Request req_;
   std::string buffer_;

  protected:
   void onResp(ReqResult rst, Response* resp);
   void onConnectStatus(TcpClient::ConnectStatus status);
   void onMessage(const char* data, ssize_t size);
   bool isConnected;
  };

 }
}
#endif
