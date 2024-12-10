#ifndef __CHAT_SERVICE_HH
#define __CHAT_SERVICE_HH

#include "server/model/UserModel.hh"
#include "server/model/OfflineMsgModel.hh"
#include "server/model/FriendModel.hh"
#include "server/model/GroupModel.hh"
#include "server/redis/Redis.hh"

#include "json.hpp"
using json = nlohmann::json;

#include <muduo/net/TcpConnection.h>
using namespace muduo;
using namespace muduo::net;

#include <functional>
#include <unordered_map>
#include <mutex>

using MsgHandler = std::function<void(const TcpConnectionPtr &conn, json &js, Timestamp time)>;

class ChatService
{
public:
    static ChatService *instance();
    ~ChatService();

    void login(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void loginout(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void reg(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void oneChat(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    void groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time);

    MsgHandler getHandler(int msgid);
    void handleRedisSubscribeMessage(int userid, std::string msg);

    void clientCloseException(const TcpConnectionPtr &conn);
    void reset();
private:
    ChatService();
    std::unordered_map<int, MsgHandler> msgHandlerMap_;

    std::mutex userConnectionMutex_;
    std::unordered_map<int, TcpConnectionPtr> userConnectionMap_;

    UserModel userModel_;
    OfflineMsgModel offlineMsgModel_;
    FriendModel friendModel_;
    GroupModel groupModel_;

    Redis redis_;
};

#endif
