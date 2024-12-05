#ifndef __OFFLINE_MSG_MODEL_HH
#define __OFFLINE_MSG_MODEL_HH

#include <string>
#include <vector>

class OfflineMsgModel
{
public:
    OfflineMsgModel(/* args */);
    ~OfflineMsgModel();

    bool insert (int userid, std::string msg);
    bool remove(int userid);
    std::vector<std::string> query(int userid);
private:
};

#endif
