#ifndef __REDIS_HH
#define __REDIS_HH

#include <hiredis/hiredis.h>
#include <functional>
#include <string>

class Redis
{
public:
    Redis();
    ~Redis();

    bool connect(std::string ip = "127.0.0.1", int port = 6379);
    bool publish(int channel, const std::string &msg);
    bool subscribe(int channel);
    bool unsubscribe(int channel);
    void observeChannelMessage();
    void setNotifyHandler(std::function<void(int, std::string)> func);
private:
    redisContext *pubContext_;
    redisContext *subContext_;
    std::function<void(int, std::string)> notifyMsgHandler_;
};

#endif
