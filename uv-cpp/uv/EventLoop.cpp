/*
   Copyright © 2017-2020, orcaer@yeah.net  All rights reserved.

   Author: orcaer@yeah.net

   Last modified: 2019-6-17

   Description: https://github.com/wlgq2/uv-cpp
*/

#include <EventLoop.hpp>
#include <TcpConnection.hpp>
#include <Async.hpp>

using namespace uv;

EventLoop::EventLoop()
 :EventLoop(EventLoop::Mode::New)
{
}

EventLoop::EventLoop(EventLoop::Mode mode)
 :loop_(nullptr),
 async_(nullptr),
 status_(NotStarted)
{
 if (mode == EventLoop::Mode::New)
 {
  loop_ = new uv_loop_t();
  ::uv_loop_init(loop_);
 }
 else
 {
  loop_ = uv_default_loop();
 }
 async_ = new Async(this);
}

EventLoop::~EventLoop()
{
 if (loop_ != uv_default_loop())
 {
  //uv_loop_close(loop_);
  uv_shutdown(loop_);
  delete async_;
  delete loop_;
 }
 //uv_loop_close(default_loop_);
}

EventLoop* uv::EventLoop::DefaultLoop()
{
 static EventLoop defaultLoop(EventLoop::Mode::Default);
 return &defaultLoop;
}

uv_loop_t* EventLoop::handle()
{
 return loop_;
}

int EventLoop::run()
{
 if (status_ == Status::NotStarted)
 {
  async_->init();
  loopThreadId_ = std::this_thread::get_id();
  status_ = Status::Started;
  auto rst = ::uv_run(loop_, UV_RUN_DEFAULT);
  status_ = Status::Stopped;
  return rst;
 }
 return -1;
}

int uv::EventLoop::runNoWait()
{
 if (status_ == Status::NotStarted)
 {
  async_->init();
  loopThreadId_ = std::this_thread::get_id();
  status_ = Status::Started;
  auto rst = ::uv_run(loop_, UV_RUN_NOWAIT);
  status_ = Status::NotStarted;
  return rst;
 }
 return -1;
}

int uv::EventLoop::stop()
{
 if (status_ == Status::Started)
 {
  async_->close([](Async* ptr)
   {
    ::uv_stop(ptr->Loop()->handle());
   });
  return 0;
 }
 return -1;
}

bool EventLoop::isStopped()
{
 return status_ == Status::Stopped;
}
void EventLoop::setStatus(const Status& status) {
 status_ = status;
}
EventLoop::Status EventLoop::getStatus()
{
 return status_;
}


bool EventLoop::isRunInLoopThread()
{
 if (status_ == Status::Started)
 {
  return std::this_thread::get_id() == loopThreadId_;
 }
 //EventLoop未运行.
 return false;
}

void uv::EventLoop::runInThisLoop(const DefaultCallback func)
{
 if (nullptr == func)
  return;

 if (isRunInLoopThread() || isStopped())
 {
  func();
  return;
 }
 async_->runInThisLoop(func);
}

const char* EventLoop::GetErrorMessage(int status)
{
 if (WriteInfo::Disconnected == status)
 {
  static char info[] = "the connection is disconnected";
  return info;
 }
 return uv_strerror(status);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
static void close_walk_cb(uv_handle_t* handle, void* arg) {
 if (!uv_is_closing(handle))
  uv_close(handle, NULL);
}
static void close_loop(uv_loop_t* loop) {
 uv_walk(loop, close_walk_cb, NULL);
 uv_run(loop, UV_RUN_DEFAULT);
}
static int can_ipv6(void) {
 uv_interface_address_t* addr = nullptr;
 int supported = 0;
 int count = 0;
 int i = 0;
 if (uv_interface_addresses(&addr, &count))
  return 0;  /* Assume no IPv6 support on failure. */
 supported = 0;
 for (i = 0; supported == 0 && i < count; i += 1)
  supported = (AF_INET6 == addr[i].address.address6.sin6_family);
 uv_free_interface_addresses(addr, count);
 return supported;
}

void EventLoop::uv_shutdown(uv_loop_t* loop_ /*= nullptr*/) {
 do {
  if (loop_) {
   close_loop(loop_);
   uv_loop_close(loop_);
  }
  close_loop(uv_default_loop());
  uv_loop_close(uv_default_loop());
  uv_library_shutdown();
 } while (0);
}