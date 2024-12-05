#ifndef __FRIEND_MODEL_HH
#define __FRIEND_MODEL_HH

#include "server/model/User.hh"

#include <vector>

class FriendModel
{
public:
    FriendModel();
    ~FriendModel();

    bool insert(int userid, int friendid);
    std::vector<User> query(int userid);

private:
};

#endif
