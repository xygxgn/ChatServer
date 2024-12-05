#ifndef __GROUP_MODEL_HH
#define __GROUP_MODEL_HH

#include "server/model/Group.hh"

class GroupModel
{
public:
    GroupModel();
    ~GroupModel();

    bool createGroup(Group &group);
    bool addGroup(int userid, int groupid, std::string role);
    std::vector<Group> queryGroup(int userid);
    std::vector<int> queryGroupUsers(int userid, int groupid);
private:
};

#endif
