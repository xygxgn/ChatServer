#include "server/net/ChatServer.hh"
#include "server/service/ChatService.hh"

#include "json.hpp"
using json = nlohmann::json;

#include <functional>
#include <iostream>

ChatServer::ChatServer(EventLoop *loop, const InetAddress listenAddr, const std::string &nameArg)
    : server_(loop, listenAddr, nameArg), loop_(loop)
{
    server_.setConnectionCallback(std::bind(&ChatServer::onConnection, this, std::placeholders::_1));
    server_.setMessageCallback(std::bind(&ChatServer::onMessage, this,
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    server_.setThreadNum(4);
}

ChatServer::~ChatServer()
{
}

void ChatServer::start()
{
    server_.start();
}

void ChatServer::onConnection(const TcpConnectionPtr &conn)
{
    if (!conn->connected())
    {
        ChatService::instance()->clientCloseException(conn);
        conn->shutdown();
    }
}

void ChatServer::onMessage(const TcpConnectionPtr &conn, Buffer *buffer, Timestamp time)
{
    std::string buf = buffer->retrieveAllAsString();
    try
    {
        json js = json::parse(buf);
        if (js.contains("msgid") && js["msgid"].is_number_integer())
        {
            MsgHandler msgHandler = ChatService::instance()->getHandler(js["msgid"].get<int>());
            msgHandler(conn, js, time);
        } 
        else
        {
            std::cerr << "Key 'msgid' does not exist or is not an integer." << std::endl;
        }
    }
    catch (const json::exception& e)
    {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
    }
}
