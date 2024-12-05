#include "server/model/GroupModel.hh"
#include "server/model/GroupUser.hh"
#include "server/sql/MySQL.hh"

GroupModel::GroupModel()
{
}

GroupModel::~GroupModel()
{
}

bool GroupModel::createGroup(Group &group)
{
    char sql[1024] = {0};
    ::sprintf(sql, "INSERT INTO allgroup(groupname, groupdesc) VALUES('%s', '%s')",
        group.getName().c_str(), group.getDesc().c_str());
    
    MySQL mysql;
    if (mysql.connect())
    {
        if (mysql.update(sql))
        {
            group.setId(::mysql_insert_id(mysql.getConnection()));
            return true;
        }
    }
    return false;
}

bool GroupModel::addGroup(int userid, int groupid, std::string role)
{
    char sql[1024] = {0};
    ::sprintf(sql, "INSERT INTO groupuser VALUES(%d, %d, '%s')",
        groupid, userid, role.c_str());
    
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

std::vector<Group> GroupModel::queryGroup(int userid)
{
    char sql[1024] = {0};
    ::sprintf(sql, "SELECT allgroup.id, allgroup.groupname, allgroup.groupdesc \
        FROM allgroup INNER JOIN groupuser ON allgroup.id = groupuser.groupid WHERE groupuser.userid = %d", userid);

    std::vector<Group> groupVec;
    MySQL mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row;
            while ((row = ::mysql_fetch_row(res)) != nullptr)
            {
                Group group;
                group.setId(::atoi(row[0]));
                group.setName(row[1]);
                group.setDesc(row[2]);
                groupVec.push_back(group);
            }
            ::mysql_free_result(res);
        }
    }

    for (Group &group : groupVec)
    {
        ::sprintf(sql, "SELECT user.id, user.name, user.state, groupuser.grouprole \
            FROM user INNER JOIN groupuser ON groupuser.userid = user.id WHERE groupuser.groupid = %d", group.getId());
        
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row;
            while ((row = ::mysql_fetch_row(res)) != nullptr)
            {
                GroupUser groupUser;
                groupUser.setId(::atoi(row[0]));
                groupUser.setName(row[1]);
                groupUser.setState(row[2]);
                groupUser.setRole(row[3]);
                group.getUsers().push_back(groupUser);
            }
            ::mysql_free_result(res);
        }
    }

    return groupVec;
}

std::vector<int> GroupModel::queryGroupUsers(int userid, int groupid)
{
    char sql[1024] = {0};
    ::sprintf(sql, "SELECT userid FROM groupuser WHERE groupid = %d and userid != %d", groupid , userid);

    std::vector<int> idVec;
    MySQL mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row;
            while ((row = ::mysql_fetch_row(res)) != nullptr)
            {
                idVec.push_back(::atoi(row[0]));
            }
            ::mysql_free_result(res);
        }
    }
    return idVec;
}
