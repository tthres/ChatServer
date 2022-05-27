#pragma once

#include <mymuduo/TcpServer.h>
#include <mymuduo/EventLoop.h>
using namespace std;

class ChatServer
{
public:
    // 初始化聊天服务器对象
    ChatServer(EventLoop *loop,
                const InetAddress &listenAddr,
                const std::string &nameArg);

    void start();

private:
    // 上报链接相关的回调函数
    void onConnection(const TcpConnectionPtr&);

    // 上报读写信息相关的回调函数
    void onMessage(const TcpConnectionPtr&,
                        Buffer*,
                        Timestamp);

    TcpServer _server;
    EventLoop *_loop;
};