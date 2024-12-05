#include "server/model/FriendModel.hh"
#include "server/sql/MySQL.hh"

FriendModel::FriendModel()
{
}

FriendModel::~FriendModel()
{
}

bool FriendModel::insert(int userid, int friendid)
{
    char sql[1024] = {0};
    ::sprintf(sql, "INSERT INTO friend VALUES('%d', '%d')", userid, friendid);
    
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

std::vector<User> FriendModel::query(int userid)
{
    char sql[1024] = {0};
    ::sprintf(sql, "SELECT user.id, user.name, user.state FROM user INNER JOIN friend ON friend.friendid = user.id WHERE userid = %d", userid);

    std::vector<User> vec;
    MySQL mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row;;

            while ((row = ::mysql_fetch_row(res)) != nullptr)
            {
                User user;
                user.setId(::atoi(row[0]));
                user.setName(row[1]);
                user.setState(row[2]);
                vec.push_back(user);
            }
            ::mysql_free_result(res);
        }
    }
    return vec;
}
