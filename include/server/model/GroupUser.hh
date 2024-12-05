#ifndef __GROUP_USER_HH
#define __GROUP_USER_HH

#include "server/model/User.hh"

class GroupUser : public User
{
public:
    void setRole(std::string role) { role_ = role; }
    std::string getRole() { return role_; }
private:
    std::string role_;
};

#endif
