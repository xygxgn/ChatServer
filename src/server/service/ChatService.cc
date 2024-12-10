#include "server/service/ChatService.hh"
#include "common.hh"

#include <muduo/base/Logging.h>
using namespace muduo;

#include <functional>
#include <vector>
#include <iostream>

ChatService* ChatService::instance()
{
    static ChatService service;
    return &service;
}

ChatService::ChatService()
{
    msgHandlerMap_.insert({LOGIN_MSG, std::bind(&ChatService::login, this,
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)});
    msgHandlerMap_.insert({LOGINOUT_MSG, std::bind(&ChatService::loginout, this,
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)});
    msgHandlerMap_.insert({REG_MSG, std::bind(&ChatService::reg, this,
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)});
    msgHandlerMap_.insert({ONE_CHAT_MSG, std::bind(&ChatService::oneChat, this,
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)});
    msgHandlerMap_.insert({ADD_FRIEND_MSG, std::bind(&ChatService::addFriend, this,
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)});
    msgHandlerMap_.insert({CREATE_GROUP_MSG, std::bind(&ChatService::createGroup, this,
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)});
    msgHandlerMap_.insert({ADD_GROUP_MSG, std::bind(&ChatService::addGroup, this,
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)});
    msgHandlerMap_.insert({GROUP_CHAT_MSG, std::bind(&ChatService::groupChat, this,
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)});
    
    if (redis_.connect())
    {
        redis_.setNotifyHandler(std::bind(&ChatService::handleRedisSubscribeMessage, this,
            std::placeholders::_1, std::placeholders::_2));
    }
}

ChatService::~ChatService()
{
    userModel_.resetState();
}

void ChatService::login(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    int id = js["id"];
    std::string pwd = js["password"];

    User user = userModel_.query(id);
    std::cout << "user.id" << id << std::endl;
    std::cout << "user.password" << pwd << std::endl;
    if (user.getId() == id && user.getPwd() == pwd)
    {
        if (user.getState() == "online")
        {
            json response;
            response["msgid"] = LOGIN_MSG_ACK;
            response["errno"] = 2;
            response["errmsg"] = "this account is using, input another!";
            conn->send(response.dump());
        }
        else
        {
            {
                std::lock_guard<std::mutex> locker(userConnectionMutex_);
                userConnectionMap_.insert({id, conn});
            }

            redis_.subscribe(id);

            user.setState("online");
            userModel_.updateState(user);

            json response;
            response["msgid"] = LOGIN_MSG_ACK;
            response["errno"] = 0;
            response["id"] = user.getId();
            response["name"] = user.getName();
            std::vector<std::string> vec = offlineMsgModel_.query(id);
            if (!vec.empty())
            {
                response["offlinemsg"] = vec;
                offlineMsgModel_.remove(id);
            }
            std::vector<User> userVec = friendModel_.query(id);
            if (!userVec.empty())
            {
                std::vector<std::string> vec2;
                for (User &user : userVec)
                {
                    json js;
                    js["id"] = user.getId();
                    js["name"] = user.getName();
                    js["state"] = user.getState();
                    vec2.push_back(js.dump());
                }
                response["friends"] = vec2;
            }

            std::vector<Group> groupuserVec = groupModel_.queryGroup(id);
            if (!groupuserVec.empty())
            {
                // group:[{groupid:[xxx, xxx, xxx, xxx]}]
                std::vector<std::string> groupVec;
                for (Group &group : groupuserVec)
                {
                    json grpjson;
                    grpjson["id"] = group.getId();
                    grpjson["groupname"] = group.getName();
                    grpjson["groupdesc"] = group.getDesc();
                    std::vector<std::string> userV;
                    for (GroupUser &user : group.getUsers())
                    {
                        json js;
                        js["id"] = user.getId();
                        js["name"] = user.getName();
                        js["state"] = user.getState();
                        js["role"] = user.getRole();
                        userV.push_back(js.dump());
                    }
                    grpjson["users"] = userV;
                    groupVec.push_back(grpjson.dump());
                }

                response["groups"] = groupVec;
            }

            conn->send(response.dump());
        }
    }
    else
    {
        json response;
        response["msgid"] = LOGIN_MSG_ACK;
        response["errno"] = 1;
        response["errmsg"] = "id or password is invalid!";
        conn->send(response.dump());
    }
}

// 处理注销业务
void ChatService::loginout(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    int userid = js["id"].get<int>();

    {
        std::lock_guard<std::mutex> lock(userConnectionMutex_);
        auto iter = userConnectionMap_.find(userid);
        if (iter != userConnectionMap_.end())
        {
            userConnectionMap_.erase(iter);
        }
    }

    redis_.unsubscribe(userid); 

    User user(userid, "", "", "offline");
    userModel_.updateState(user);
}

void ChatService::reg(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    std::string name = js["name"];
    std::string pwd = js["password"];

    User user;
    user.setName(name);
    user.setPwd(pwd);
    bool state = userModel_.insert(user);
    if (state)
    {
        json response;
        response["msgid"] = REG_MSG_ACK;
        response["errno"] = 0;
        response["id"] = user.getId();
        conn->send(response.dump());
    }
    else
    {
        json response;
        response["msgid"] = REG_MSG_ACK;
        response["errno"] = 1;
        conn->send(response.dump());
    }
}

void ChatService::oneChat(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    int toid = js["toid"].get<int>();
    {
        std::lock_guard<std::mutex> locker(userConnectionMutex_);
        auto iter = userConnectionMap_.find(toid);
        if (iter != userConnectionMap_.end())
        {
            iter->second->send(js.dump());
            return;
        }
    }

    User user = userModel_.query(toid);
    if (user.getState() == "online")
    {
        redis_.publish(toid, js.dump());
        return;
    }

    offlineMsgModel_.insert(toid, js.dump());
}

void ChatService::addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    int userid = js["id"].get<int>();
    int friendid = js["friendid"].get<int>();

    friendModel_.insert(userid, friendid);

}

void ChatService::createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    int userid = js["id"].get<int>();
    std::string name = js["groupname"];
    std::string desc = js["groupdesc"];

    Group group(-1, name, desc);
    if (groupModel_.createGroup(group))
    {
        groupModel_.addGroup(userid, group.getId(), "creator");
    }
}

void ChatService::addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    int userid = js["id"].get<int>();
    int groupid = js["groupid"].get<int>();
    groupModel_.addGroup(userid, groupid, "normal");
}

void ChatService::groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    int userid = js["id"].get<int>();
    int groupid = js["groupid"].get<int>();
    std::vector<int> useridVec = groupModel_.queryGroupUsers(userid, groupid);
    if (useridVec.empty())
        std::cout << "no group find!" << std::endl;
    for (int id : useridVec)
    {
        std::lock_guard<std::mutex> locker(userConnectionMutex_);
        auto iter = userConnectionMap_.find(id);
        if (iter != userConnectionMap_.end())
        {
            iter->second->send(js.dump());
        }
        else
        {
            User user = userModel_.query(id);
            if (user.getState() == "online")
            {
                redis_.publish(id, js.dump());
            }
            else
            {
                offlineMsgModel_.insert(id, js.dump());
            }
        }
    }
}

MsgHandler ChatService::getHandler(int msgid)
{
    auto iter = msgHandlerMap_.find(msgid);
    if (iter == msgHandlerMap_.end())
    {
        return [=](const TcpConnectionPtr &conn, json &js, Timestamp time){
            LOG_ERROR << "msgid: " << msgid << " can not find handler!";
        };
    }
    return msgHandlerMap_[msgid];
}

void ChatService::reset()
{
    userModel_.resetState();
}


void ChatService::clientCloseException(const TcpConnectionPtr &conn)
{
    User user;
    {
        std::lock_guard<std::mutex> locker(userConnectionMutex_);
        for (auto iter = userConnectionMap_.begin(); iter != userConnectionMap_.end(); ++iter)
        {
            if (iter->second == conn)
            {
                user.setId(iter->first);
                userConnectionMap_.erase(iter);
                break;
            }
        }
    }

    redis_.unsubscribe(user.getId());

    if (user.getId() != -1)
    {
        user.setState("offline");
        userModel_.updateState(user);
    }
}

void ChatService::handleRedisSubscribeMessage(int userid, std::string msg)
{
    std::lock_guard<std::mutex> locker(userConnectionMutex_);
    auto iter = userConnectionMap_.find(userid);
    if (iter != userConnectionMap_.end())
    {
        iter->second->send(msg);
        return;
    }

    offlineMsgModel_.insert(userid, msg);
}
