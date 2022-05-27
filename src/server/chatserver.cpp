#include "chatserver.hpp"
#include "chatservice.hpp"
#include "json.hpp"

#include <functional>
#include <string>
using namespace std;
using namespace placeholders;
using json = nlohmann::json;



ChatServer::ChatServer(EventLoop *loop,
                const InetAddress &listenAddr,
                const std::string &nameArg)
                : _server(loop, listenAddr, nameArg)
                , _loop(loop)
{
    _server.seConnectionCallback(bind(&ChatServer::onConnection, this, _1));
    _server.setMessageCallback(bind(&ChatServer::onMessage, this, _1, _2, _3));

    _server.setThreadNum(4);
}

void ChatServer::start()
{
    _server.start();
}

// 上报链接相关的回调函数
void ChatServer::onConnection(const TcpConnectionPtr &conn)
{
    if (!conn->connected())
    {
        ChatService::instance()->clientCloseException(conn);
        conn->shutdown();
    }
}

// 上报读写信息相关的回调函数
void ChatServer::onMessage(const TcpConnectionPtr &conn,
                    Buffer *buffer,
                    Timestamp time)
{
    string buf = buffer->retrieveAllAsString();
    // 数据的反序列化
    json js = json::parse(buf);
    
    // 完全解耦 网络模块 和 业务模块 的代码
    // 通过js【msgid】获取回调操作
    auto msgHandler = ChatService::instance()->getHandler(js["msgid"].get<int>());
    msgHandler(conn, js, time);
}