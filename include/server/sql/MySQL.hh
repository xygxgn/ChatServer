#ifndef __MYSQL_HH
#define __MYSQL_HH

#include <mysql/mysql.h>
#include <string>

class MySQL
{
public:
    MySQL();
    ~MySQL();

    bool connect();
    bool update(std::string sql);
    MYSQL_RES* query(std::string sql);
    MYSQL* getConnection();
private:
    MYSQL *conn_;
};

#endif
