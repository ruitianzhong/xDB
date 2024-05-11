# xDB

A DBMS implemented for Xidian University‘s compiler course

## Supported SQL Example

```sql
CREATE DATABASE example;
USE example;
CREATE TABLE user (id int);
SHOW TABLES;
INSERT INTO user (id) VALUES (1);
SELECT id from user WHERE id = 42;
UPDATE user SET id=1 WHERE id=42;
DELETE FROM user WHERE id=42;
DROP TABLE user;
```

## Build

```shell
mkdir build
cd build
cmake ..
make
```

## Tentative Structure

## Resources

+ [sqllogictest](https://www.sqlite.org/sqllogictest/doc/trunk/about.wiki)
+ [HuaDB](https://thu-db.github.io/huadb-doc/)
+ [BusTub](https://15445.courses.cs.cmu.edu/fall2022/schedule.html)
+ [USTC Compiler Principles](https://ustc-compiler-principles.github.io)
+ [SQL Parser](https://github.com/hyrise/sql-parser)
