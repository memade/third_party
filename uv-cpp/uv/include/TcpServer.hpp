/*
   Copyright © 2017-2019, orcaer@yeah.net  All rights reserved.

   Author: orcaer@yeah.net

   Last modified: 2019-12-31

   Description: https://github.com/wlgq2/uv-cpp
*/

#ifndef UV_TCP_SERVER_HPP
#define UV_TCP_SERVER_HPP

#include <functional>
#include <memory>
#include <set>
#include <map>

#include "TcpAcceptor.hpp"
#include "TcpConnection.hpp"
#include "TimerWheel.hpp"

namespace uv {

 using OnConnectionStatusCallback = std::function<void(std::weak_ptr<TcpConnection>)>;

 //no thread safe.
 class TcpServer {
 protected:
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 public:
  static void SetBufferMode(uv::GlobalConfig::BufferMode mode);
 public:
  TcpServer(EventLoop* loop, bool tcpNoDelay = true);
  virtual ~TcpServer();
 public:
  virtual int bindAndListen(SocketAddr& addr);

  virtual void close(DefaultCallback callback);

  virtual TcpConnectionPtr getConnection(const std::string& name);
  virtual void closeConnection(const std::string& name);

  virtual void setNewConnectCallback(OnConnectionStatusCallback callback);
  virtual void setConnectCloseCallback(OnConnectionStatusCallback callback);

  virtual void setMessageCallback(OnMessageCallback callback);

  virtual void write(TcpConnectionPtr connection, const char* buf, unsigned int size, AfterWriteCallback callback = nullptr);
  virtual void write(std::string& name, const char* buf, unsigned int size, AfterWriteCallback callback = nullptr);
  virtual void writeInLoop(TcpConnectionPtr connection, const char* buf, unsigned int size, AfterWriteCallback callback);
  virtual void writeInLoop(std::string& name, const char* buf, unsigned int size, AfterWriteCallback callback);

  virtual void setTimeout(unsigned int);
 protected:
  virtual void onAccept(EventLoop* loop, UVTcpPtr client);

  virtual void addConnection(std::string& name, TcpConnectionPtr connection);
  virtual void removeConnection(std::string& name);
  virtual void onMessage(TcpConnectionPtr connection, const char* buf, ssize_t size);
 protected:
  EventLoop* loop_;
 protected:
  bool tcpNoDelay_;
  SocketAddr::IPV ipv_;
  std::shared_ptr <TcpAcceptor> accetper_;
  std::map<std::string, TcpConnectionPtr>  connnections_;

  OnMessageCallback onMessageCallback_;
  OnConnectionStatusCallback onNewConnectCallback_;
  OnConnectionStatusCallback onConnectCloseCallback_;
  TimerWheel<ConnectionWrapper> timerWheel_;
 };


}///namespace uv
#endif
