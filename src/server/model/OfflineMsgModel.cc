#include "server/model/OfflineMsgModel.hh"
#include "server/sql/MySQL.hh"

OfflineMsgModel::OfflineMsgModel()
{
}

OfflineMsgModel::~OfflineMsgModel()
{
}

bool OfflineMsgModel::insert (int userid, std::string msg)
{
    char sql[1024] = {0};
    ::sprintf(sql, "INSERT INTO offlinemessage VALUES('%d', '%s')", userid, msg.c_str());
    
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

bool OfflineMsgModel::remove(int userid)
{
    char sql[1024] = {0};
    ::sprintf(sql, "DELETE FROM offlinemessage WHERE userid = %d", userid);
    
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

std::vector<std::string> OfflineMsgModel::query(int userid)
{
    char sql[1024] = {0};
    ::sprintf(sql, "SELECT message FROM offlinemessage WHERE userid = %d", userid);

    std::vector<std::string> vec;
    MySQL mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row;;

            while ((row = ::mysql_fetch_row(res)) != nullptr)
            {
                vec.push_back(row[0]);
            }
            ::mysql_free_result(res);
        }
    }
    return vec;
}