create database test;

use test;

create table t1 (x int,y char(20));
insert into t1 values (42,'hello');
select * from t1;
insert into t1 values (43,'hello');
insert into t1 values (42,NULL);
insert into t1 values (NULL,NULL);
insert into t1 values (99,'hello'),(100,'world');
insert into t1 (x) values (101);
insert into t1 (y) values ('y only');

-- insert test (error expected)
-- Char too long
insert into t1 (x,y) values (101,'looooooooooooooooooooooooooooooooooog');
-- Data type mismatch
insert into t1 (x,y) values ('100','mismatch');
-- Too much argument
insert into t1 (x) values (1,'column mismatch',99);
-- Column count doesn't match value count at row 1
insert into t1 values (1);
-- Unknown column name
insert into t1 (not_exist_name) values (1,'not exist name');

create table t2 (x int,y char(20));

insert into t2 values (1,'a'),(2,'b'),(3,'c');



create table t3 (x int,y char(20));

insert into t3 values (4,'d'),(5,'e'),(6,'f');

create table t4 (x int,z char(20));

insert into t4 values (7,'g'),(8,'h'),(9,'i');

-- no ambiguity
select t2.x,t3.x from t2,t3;

-- ambiguous column name x
select x,y from t2,t3;

-- ambiguous column name y
select t2.x,y from t2,t3;

-- infer the table that z belongs to
select t2.x,z from t2,t4;

-- expression evaluation
select * from t2;

select * from t2 where 1=1;
select * from t2 where 1=x;
select * from t2 where 'a'<y;
select * from t2 where 'a'<y and x>2;
select * from t2 where ('a'<y and x>2)=x-1;

update t2 set x=x*10 where x =3;
select * from t2;

delete from t2 where x=x/x ;
select * from t2;

drop table t1;

-- column constraint ( the comment can be filtered)
create table t5 (id int not null);
insert into t5 values (1),(2);
-- column id can not be null
insert into t5 values (NULL);


-- show
show databases;
show tables;

-- exit
exit;



