#ifndef __USER_MODEL_HH
#define __USER_MODEL_HH

#include "server/model/User.hh"

class UserModel
{
public:
    UserModel();
    ~UserModel();
    
    bool insert(User &user);
    User query(int id);
    bool updateState(User &user);
    bool resetState();
private:
};

#endif
