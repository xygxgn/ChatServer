#include "server/model/UserModel.hh"
#include "server/sql/MySQL.hh"

#include <iostream>

UserModel::UserModel()
{
}

UserModel::~UserModel()
{
}


bool UserModel::insert(User &user)
{
    char sql[1024] = {0};
    ::sprintf(sql, "INSERT INTO user(name, password, state) VALUES('%s', '%s', '%s')",
        user.getName().c_str(), user.getPwd().c_str(), user.getState().c_str());
    
    MySQL mysql;
    if (mysql.connect())
    {
        if (mysql.update(sql))
        {
            user.setId(::mysql_insert_id(mysql.getConnection()));
            return true;
        }
    }
    return false;
}


User UserModel::query(int id)
{
    char sql[1024] = {0};
    ::sprintf(sql, "SELECT * FROM user WHERE id = %d", id);

    MySQL mysql;
    User user;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row;
            if ((row = ::mysql_fetch_row(res)) != nullptr)
            {
                user.setId(::atoi(row[0]));
                user.setName(row[1]);
                user.setPwd(row[2]);
                user.setState(row[3]);
            }
            ::mysql_free_result(res);
        }
    }
    return user;
}

bool UserModel::updateState(User &user)
{
    char sql[1024] = {0};
    ::sprintf(sql, "UPDATE user SET state = '%s' WHERE id = '%d'", user.getState().c_str(), user.getId());

    MySQL mysql;
    if (mysql.connect())
    {
        if (mysql.update(sql))
        {
            return true;
        }
    }
    return false;
}

bool UserModel::resetState()
{
    char sql[1024] = "UPDATE user SET state = 'offline' WHERE state = 'online'";

    MySQL mysql;
    if (mysql.connect())
    {
        if (mysql.update(sql))
        {
            return true;
        }
    }
    return false;
}
