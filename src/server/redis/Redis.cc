#include "server/redis/Redis.hh"

#include <iostream>
#include <thread>

Redis::Redis() : pubContext_(nullptr), subContext_(nullptr)
{
}

Redis::~Redis()
{
    if (pubContext_ != nullptr)
        ::redisFree(pubContext_);
    if (subContext_ != nullptr)
        ::redisFree(subContext_);
}

bool Redis::connect(std::string ip, int port)
{
    pubContext_ = redisConnect(ip.c_str(), port);
    if (pubContext_ == nullptr)
    {
        std::cerr << "connect redis failed!" << std::endl;
        return false;
    }

    subContext_ = redisConnect(ip.c_str(), port);
    if (subContext_ == nullptr)
    {
        std::cerr << "connect redis failed!" << std::endl;
        return false;
    }

    std::thread t(&Redis::observeChannelMessage, this);
    t.detach();

    return true;
}

bool Redis::publish(int channel, const std::string &msg)
{
    // redisCommand (redisAppendCommand + redisBufferWrite + redisGetReply)
    redisReply *reply = (redisReply*)redisCommand(pubContext_, "PUBLISH %d %s", channel, msg.c_str());
    if (reply == nullptr)
    {
        std::cerr << "publish command failed!" << std::endl;
        return false;
    }

    freeReplyObject(reply);
    return true;
}

bool Redis::subscribe(int channel)
{
    // redisAppendCommand
    if (redisAppendCommand(subContext_, "SUBSCRIBE %d", channel) == REDIS_ERR)
    {
        std::cerr << "subscribe command failed!" << std::endl;
        return false;
    }

    // redisBufferWrite
    int done = 0;
    while (!done)
    {
        if (redisBufferWrite(subContext_, &done) == REDIS_ERR)
        {
            std::cerr << "subscribe command failed" << std::endl;
            return false;
        }
    }
    
    // redisGetReply

    return true;
}

bool Redis::unsubscribe(int channel)
{
    // redisAppendCommand
    if (redisAppendCommand(subContext_, "UNSUBSCRIBE %d", channel) == REDIS_ERR)
    {
        std::cerr << "unsubscribe command failed!" << std::endl;
        return false;
    }

    // redisBufferWrite
    int done = 0;
    while (!done)
    {
        if (redisBufferWrite(subContext_, &done) == REDIS_ERR)
        {
            std::cerr << "unsubscribe command failed" << std::endl;
            return false;
        }
    }
    
    // redisGetReply

    return true;
}

void Redis::observeChannelMessage()
{
    redisReply *reply = nullptr;
    while (redisGetReply(subContext_, (void**)&reply) == REDIS_OK)
    {
        if (reply != nullptr && reply->element[2] != nullptr && reply->element[2]->str != nullptr)
        {
            notifyMsgHandler_(std::atoi(reply->element[1]->str), reply->element[2]->str);
        }
        freeReplyObject(reply);
    }
    std::cerr << ">>>>>>>>>>>>> observeChannelMessage quit <<<<<<<<<<<<<" << std::endl;
}

void Redis::setNotifyHandler(std::function<void(int, std::string)> func)
{
    notifyMsgHandler_ = func;
}
