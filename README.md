# xDB

A DBMS implemented for Xidian University‘s compiler course


## Build

### Ubuntu 22.04

To get the third party dependencies(via source code), run:

```shell
git submodule init
git submodule update
```

Install the following libraries on your computer
```shell
# required by rocksdb
sudo apt-get install libgflags-dev libzstd-dev libsnappy-dev zlib1g-dev libbz2-dev liblz4-dev
# required by protobuf
sudo apt-get install libprotobuf-dev protobuf-compiler
```

## Supported SQL (Example)

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

More details:

```sql
use test;
select * from test.table1,table1;-- duplicated table (It is correctly checked)
```


## Build

```shell
mkdir build
cd build
cmake ..
make
```

## Project Structure

| File                 | Description                                         |
|----------------------|-----------------------------------------------------|
| include/*            | all the header files                                |
| sql/*                | intermediate representation(IR) for SQL             |
| executor/*           | execute SQL IR                                      |
| executor/processor/* | processor for SQL expression(visitor pattern)       |
| parser/sql.l         | lexical analysis for SQL(flex)                      |
| parser/sql.y         | syntax analysis for SQL(bison)                      |
| third_party/*        | third party dependencies, e.g., googletest, rocksdb |
| test/*               | unit test                                           |
| docs/*               | documentation for the project                       |
 
## Resources

+ [sqllogictest](https://www.sqlite.org/sqllogictest/doc/trunk/about.wiki)
+ [HuaDB](https://thu-db.github.io/huadb-doc/)
+ [BusTub](https://15445.courses.cs.cmu.edu/fall2022/schedule.html)
+ [USTC Compiler Principles](https://ustc-compiler-principles.github.io)
+ [SQL Parser](https://github.com/hyrise/sql-parser)
