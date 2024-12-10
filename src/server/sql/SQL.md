## SQL 基本语法
### DDL 语句 (用来定义数据库对象, 包括数据库、表、字段)
  <!-- 数据库操作 -->
  * 查询所有的数据库
    * `SHOW DATABASES;`
  * 查看当前所在数据库名称
    * `SELECT DATABASE();`
  * 选择使用某个数据库
    * `USE 数据库名;`
  * 创建数据库
    * `CREATE DATABASE [IF NOT EXISTS] 数据库名 [DEFAULT CHARSET 字符集] [COLLATE 排序规则];`
  * 删除数据库
    * `DROP DATABASE [IF EXISTS] 数据库名;`
  <!-- 表操作 -->
  * 查询数据库中所有表
    * `SHOW TABLES;`
  * 查询表结构
    * `DESC 表名;`
  * 查询指定表的建表语句
    * `SHOW CREATE TABLE 表名;`
  * 创建表
    * `CREATE TABLE 表名(字段1 字段1类型 [COMMENT 字段1注释], 字段1 字段1类型 [COMMENT 字段1注释], ...) [COMMENT 表注释];`
  * 添加表中字段
    * `ALTER TABLE 表名 ADD 字段名 类型(长度) [COMMENT 注释] [约束];`
  * 修改表中字段数据类型
    * `ALTER TABLE 表名 MODIFY 字段名 新数据类型(长度);`
  * 修改表中字段名和数据类型
    * `ALTER TABLE 表名 CHANGE 旧字段名 新字段名 类型(长度) [COMMENT 注释] [约束];`
  * 删除表中字段
    * `ALTER TABLE 表名 DROP 字段名;`
  * 修改表名
    * `ALTER TABLE 表名 RENAME TO 新表名;`
  * 删除表
    * `DROP TABLE [IF EXISTS] 表名;`
  * 删除表并重新创建表
    * `TRUNCATE TABLE 表名;`

### DML 语句 (用来对数据库中表的数据记录进行增删改操作)
  <!-- 添加数据 -->
  * 指定字段添加数据记录
    * `INSERT INTO 表名(字段名1, 字段名2, ...) VALUES (值1, 值2, ...);`
  * 全部字段添加数据记录
    * `INSERT INTO 表名 VALUES (值1, 值2, ...);`
  * 批量添加数据记录
    * `INSERT INTO 表名(字段名1, 字段名2, ...) VALUES (值1, 值2, ...), (值1, 值2, ...), ...;`
    * `INSERT INTO 表名 VALUES (值1, 值2, ...), (值1, 值2, ...), ...;`
  <!-- 修改数据 -->
  * 修改表中字段数据
    * `UPDATE 表名 SET 字段名1=值1, 字段名2=值2, ... [WHERE 条件];`
  <!-- 删除数据 -->
  * 删除表中数据记录
    * `DELETE FROM 表名 [WHERE 条件];`

### DQL 语句 (用来查询数据库中表的记录)
  <!-- 查询数据 -->
  * 编写顺序
    * `SELECT 字段列表 FROM 表名列表 WHERE 条件列表 GROUP BY 分组字段列表 HAVING 分组后条件列表 ORDER BY 排序字段列表 LIMIT 分页参数;`
  * 执行顺序
    * `FROM 表名列表 WHERE 条件列表 GROUP BY 分组字段列表 HAVING 分组后条件列表 SELECT 字段列表 ORDER BY 排序字段列表 LIMIT 分页参数;`
  <!-- 基本查询 -->
  * 基本查询语法
    * `SELECT [DISTINCT] 字段1 [[AS] 别名1], 字段2 [[AS] 别名2], ... FROM 表名;`
  * 查询所有字段
    * `SELECT * FROM 表名;`
  * 查询多个字段并设置别名
    * `SELECT 字段1 [AS] 别名1, 字段2 [AS] 别名2, ... FROM 表名;`
  * 去重查询多个字段
    * `SELECT DISTINCT 字段列表 FROM 表名;`
  <!-- 条件查询 -->
  * 比较运算符
    * `>, >=, <, <=, =, <>, !=, BETWEEN ... AND ..., IN(...), LIKE 占位符, IS NULL`  其中占位符'_'表示占一个字符, '%'表示占任意个字符
  * 逻辑运算符
    * `AND, &&, OR, ||, NOT, !`
  * 条件查询语法
    * `SELECT 字段列表 FROM 表名 WHERE 条件列表;`
  <!-- 聚合函数查询 -->
  * 聚合函数
    * `count(), max(), min(), avg(), sum()`
  * 聚合函数查询语法
    * `SELECT 聚合函数(字段列表) FROM 表名;`
  <!-- 分组查询 -->
  * 分组查询语法
    * `SELECT 字段列表 FROM 表名 [WHERE 条件列表] GROUP BY 分组字段名 [HAVING 分组后过滤条件];`
  * WHERE 和 HAVING 的区别: 执行时机 WHERE 在分组之前, HAVING 在分组之后; 判断条件 WHERE 不能对聚合函数判断而 HAVING 可以
  <!-- 排序查询 -->
  * 排序查询语法
    * `SELECT 字段列表 FROM 表名 ORDER BY 字段1 排序方式, 字段2 排序方式2, ...;` 排序方式: 升序 ASC (默认), 降序 DESC
  <!-- 分页查询 -->
  * 分页查询语法
    * `SELECT 字段列表 FROM 表名 LIMIT 起始索引, 查询记录数;`

### DCL 语句 (用来创建数据库用户, 控制数据库的访问权限)
  <!-- 管理用户 -->
  * 查询用户
    * `USE mysql;`
    * `SELECT Host, User FROM user;`
  * 创建用户
    * `CREATE USER '用户名'@'主机名' IDENTIFIED BY '密码';`
  * 修改用户密码
    * `ALTER USER '用户名'@'主机名' IDENTIFIED WITH mysql_native_password BY '新密码';`
  * 删除用户
    * `DROP USER '用户名'@'主机名';`
  <!-- 管理权限 -->
  * 常用权限
    * `ALL, ALL PRIVILEGES, SELECT, INSERT, UPDATE, DELETE, ALTER, DROP, CREATE`
  * 查询权限
    * `SHOW GRANTS FOR '用户名'@'主机名';`
  * 授予权限
    * `GRANT 权限列表 ON 数据库.表名 TO '用户名'@'主机名';`
  * 撤销权限
    * `REVOKE 权限列表 ON 数据库.表名 FROM '用户名'@'主机名';`

## 函数
  * 显示函数结果
    * `SELECT 函数(参数)`
### 字符串函数
  * 字符串拼接
    * `CONCATE(S1, S2, ..., Sn)`
  * 字符串转大写
    * `UPPER(str)`
  * 字符串转小写
    * `LOWER(str)`
  * 左填充 (用字符串pad对str的左边进行填充, 达到 n 个字符串长度)
    * `LPAD(str, n, pad)`
  * 右填充 (用字符串pad对str的右边进行填充, 达到 n 个字符串长度)
    * `RPAD(str, n, pad)`
  * 去掉字符串头部和尾部的空格
    * `TRIM(str)`
  * 返回字符串str从start起的长度为n的字符串
    * `SUBSTRING(str, start, len)`
### 数值函数
  * 向上取整
    * `CEIL(x)`
  * 向下取整
    * `FLOOR(x)`
  * 返回 x/y 的模
    * `MOD(x, y)`
  * 返回 [0,1]的随机数
    * `RAND()`
  * 求参数 x 的四舍五入值, 保留 y 位小数
    * `ROUND(x, y)`
### 日期函数
  * 返回当前日期
    * `CURDATE()`
  * 返回当前时间
    * `CURTIME()`
  * 返回当前日期和时间
    * `NOW()`
  * 获取指定 date 的年份
    * `YEAR(date)`
  * 获取指定 date 的月份
    * `MONTH(date)`
  * 获取指定 date 的日期
    * `DAY(date)`
  * 返回一个日期/时间加上一个时间间隔 expr 后的时间
    * `DATE_ADD(date, INTERVAL expr type)`
  * 返回起始时间 date1 和结束时间 date2 之间的天数
    * `DATEDIFF(date1, date2)`
### 流程函数
  * 如果 value 为 true, 返回 t, 否则返回 f
    * `IF(value, t, f)`
  * 如果 value1 不为空, 返回 value1, 否则返回 value2
    * `IFNULL(value1, value2)`
  * 如果 val1 为 true, 返回 res1, ..., 否则返回 default 默认值
    * `CASE WHEN [val1] THEN [res1] ... ELSE [default] END`
  * 如果 expr 的值为 val1, 返回 res1, ..., 否则返回 default 默认值
    * `CASE [expr] WHEN [val1] THEN [RES1] ... ELSE [default] END`


## 约束 (作用于表中字段, 可以在创建表/修改表的时候添加约束)
### 非空约束
  * 非空约束关键字: NOT NULL (限制该字段的数据不能为null)
    * `CREATE TABLE 表名(字段名 数据类型 [NOT NULL]);`

### 唯一约束
  * 唯一约束关键字: UNIQUE (保证该字段的所有数据都是唯一的、不重复的)
    * `CREATE TABLE 表名(字段名 数据类型 [UNIQUE]);`

### 主键约束
  * 主键约束关键字: PRIMARY KEY (主键是一行数据的唯一标识, 要求非空且唯一)
    * `CREATE TABLE 表名(字段名 数据类型 [PRIMARY KEY]);`

### 默认约束
  * 默认约束关键字: DEFAULT (保存数据时, 如果未指定该字段的值, 则采用默认值)
    * `CREATE TABLE 表名(字段名 数据类型 [DEFAULT 默认值]);`

### 检查约束
  * 检查约束关键字: CHECK (保证字段值满足某一个条件)
    * `CREATE TABLE 表名(字段名 数据类型 [CHECK(条件列表)]);`

### 外键约束
  * 外键约束关键字: FOREIGN KEY (用来让两张表的数据之间建立连接, 保证数据的一致性和完整性)
    * `CREATE TABLE 表名(字段名 数据类型 [CONSTRAINT] [外键名称] FOREIGN KEY (外键字段名) REFERENCES 主表(主表列名));`
    * `ALTER TABLE 表名 ADD CONSTRAINT 外键名称 FOREIGN KEY(外键字段名) REFERENCES 主表(主表列名));`
    * `ALTER TABLE 表名 DROP FOREIGN KEY 外键名称;`
  * 行为 (NO ACTION, RESTRICT, CASCADE, SET NULL, SET DEFAULT)
    * `ALTER TABLE 表名 ADD CONSTRAINT 外键名称 FOREIGN KEY(外键字段名) REFERENCES 主表(主表列名)) ON UPDATE CASCADE ON DELETE CASCADE;`
    * `ALTER TABLE 表名 ADD CONSTRAINT 外键名称 FOREIGN KEY(外键字段名) REFERENCES 主表(主表列名)) ON UPDATE SET NULL ON DELETE SET NULL;`

## 多表查询
### 多表关系
  * 一对多(多对一): 在多的一方建立外键, 指向一的一方的主键
  * 多对多: 建立第三张中间表, 中间表至少包含两个外键, 分别关联两方主键
  * 一对一: 在任意一方加入外键, 关联另一方的主键, 并且设置外键为唯一的(UNIQUE)

### 多表查询分类
  * 连接查询
    * 内连接: 查询交集部分的数据
    * 外连接: 左外连接(查询左表所有数据, 以及两张表交集部分的数据) 右外连接(查询右表所有数据, 以及两张表交集部分的数据)
    * 自连接: 当前表与自身的连接查询, 自连接必须使用表别名
  * 子查询

### 连接查询
  <!-- 内连接 -->
  * 隐式内连接
    * `SELECT 字段列表 FROM 表1 表2 WHERE 条件列表;`
  * 显式内连接
    * `SELECT 字段列表 FROM 表1 [INNER] JOIN 表2 ON 条件列表;`
  <!-- 外连接 -->
  * 左外连接
    * `SELECT 字段列表 FROM 表1 LEFT [OUTER] JOIN 表2 ON 条件列表;`
  * 右外连接
    * `SELECT 字段列表 FROM 表1 RIGHT [OUTER] JOIN 表2 ON 条件列表;`
  <!-- 自连接(可以是内连接也可以是外连接) -->
  * 自连接
    * `SELECT 字段列表 FROM 表1 别名1 JOIN 表1 别名2 ON 条件列表;`

### 联合查询
  * 把多次查询的结果合并起来, 形成一个新的查询结果集
    * `SELECT 字段列表 FROM 表1, ... UNION [ALL] SELECT 字段列表 FROM 表2, ...;`

### 子查询(嵌套查询)
  * 子查询语法
    * `SELECT 字段列表 FROM 表1, ... WHERE column1 = (SELECT column1 FROM 表2);`
    * `SELECT 字段列表 FROM (SELECT column1 FROM 表1) WHERE 条件列表;`
  * 标量子查询(子查询结果为一个值)
    * `>, >=, <, <=, =, <>`
  * 列子查询(子查询结果为一列)
    * `IN, NOT IN, ANY, SOME, ALL`
  * 行子查询(子查询结果为一行)
    * `=, <>, !=, IN, NOT IN`
  * 表子查询(子查询结果为多行多列)
    * `IN`


## 事务
### 事务操作
  * 查看事务提交方式
    * `SELECT @@autocommit;`
  * 设置事务手动提交方式
    * `SET @@autocommit=0;`
  * 设置事务自动提交方式
    * `SET @@autocommit=1;`
  * 开启事务
    * `START TRANSACTION;` 或者 `BEGIN;`
  * 提交事务
    * `COMMIT;`
  * 回滚事务
    * `ROLLBACK;`

### 事务特性
  * 原子性
  * 一致性
  * 隔离性
  * 持久性

### 并发事务问题
  * 脏读(一个事务读到另一个事务还没有提交的数据)
  * 不可重复读(一个事务先后读取同一条记录, 但两次读取的数据不同)
  * 幻读(一个事务按照条件查询时, 没有对应的数据行, 但是在插入数据时发现这行数据已经存在)
  
  |                   |  **脏读**  |  **不可重复读**  |  **幻读**  |
  |:------------------|-----------|----------------|------------|
  | read uncommitted  |    √   |      √     |    √   |
  | read committed    |    ×   |      √     |    √   |
  | repeatable read   |    ×   |      ×     |    √   |
  | serializable      |    ×   |      ×     |    ×   |

  * 查看事务隔离级别
    * `SELECT @@TRANSACTION_ISOLATION;`
  * 设置事务隔离级别
    * `SET [SESSION|GLOBAL] TRANSACTION ISOLATION LEVEL {READ UNCOMMITTED | READ COMMITTED | REPEATABLE READ | SERIALIZATION};`
