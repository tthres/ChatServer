#pragma once

#include "json.hpp"
#include "usermodel.hpp"
#include "offlinemessagemodel.hpp"
#include "friendmodel.hpp"
#include "groupmodel.hpp"
#include "redis.hpp"

#include <mymuduo/TcpConnection.h>
#include <unordered_map>
#include <functional>
#include <mutex>
using namespace std;
using json = nlohmann::json;

using MsgHandler = function<void(const TcpConnectionPtr &conn,
                                    json &js, 
                                    Timestamp time)>;

// 聊天服务器业务类 单例
class ChatService
{
public:
    // 单例的接口
    static ChatService* instance();

    // 处理登录业务
    void login(const TcpConnectionPtr &conn,
                json &js, 
                Timestamp time);

    // 处理注销业务
    void loginout(const TcpConnectionPtr &conn, json &js, Timestamp time);

    // 处理注册业务
    void reg(const TcpConnectionPtr &conn,
                json &js, 
                Timestamp time);

    // 一对一聊天业务
    void oneChat(const TcpConnectionPtr &conn,
                    json &js, 
                    Timestamp time);

    // 添加好友业务
    void addFriend(const TcpConnectionPtr &conn,
                    json &js, 
                    Timestamp time);

    // 创建群组业务
    void createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);

    // 加入群组业务
    void addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    
    // 群组聊天业务
    void groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time);

    // 获取消息对应的处理器
    MsgHandler getHandler(int msgid);

    // 服务器异常，业务重置方法
    void reset();

    // 处理客户端异常退出
    void clientCloseException(const TcpConnectionPtr &conn);

    void handleRedisSubscribeMessage(int, string);

private:
    // 单例
    ChatService();

    // 消息id对应的处理操作
    unordered_map<int, MsgHandler> _msgHandlerMap; 

    // 存储在线用户的通信连接, 这个要注意线程安全
    unordered_map<int, TcpConnectionPtr> _userConnMap;

    // 互斥锁， ↑
    mutex _connMutex;

    // 数据操作类对象
    UserModel _userModel;
    OfflineMsgModel _offlineMsgModel;
    FriendModel _friendModel;
    GroupModel _groupModel;

    // redis操作对象
    Redis _redis;

};