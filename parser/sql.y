%{
#include <stdio.h>
int yylex(void);
void yyerror(const char * s);
%}
%define api.token.prefix {SQL_}

%token CREATE USE SHOW TABLE TABLES FROM
%token WHERE SELECT DATABASE INTO VALUES
%token UPDATE SET DROP JOIN INNER OUTER LEFT
%token RIGHT INT INTEGER VARCHAR AS BETWEEN
%token DELETE IS NOT OR NULL CHAR
%token IDENTIFIER FLOAT STRING INSERT
%token IF EXISTS

%type select_statement
%type insert_statement
%type create_statement
%type  drop_statement
%type delete_statement
%type show_statement
%type statement
%type table_name
%type fields_definition field_type field type
%type insert_values insert_value
%type opt_exists

%start statements

%%
statements
: statements statement { printf("done"); return 0; }
| statement {}

statement
: create_statement {}
| insert_statement {printf("insert\n");}
| drop_statement { printf("drop stmt\n"); }

table_name: IDENTIFIER

opt_exists: IF EXISTS {}
| /* empty */ { printf("opt");}


 /****** CREATE statement ******/
create_statement
: CREATE TABLE table_name '(' fields_definition ')' ';'

fields_definition
: fields_definition ',' field_type | field_type

field_type: field type

field: IDENTIFIER

type
: CHAR '(' INTEGER ')' {}
| INT {}

 /****** INSERT statement ******/

insert_statement
: INSERT INTO table_name '(' insert_values ')' ';'  { }

insert_values
: insert_values ',' insert_value
| insert_value

insert_value
: INTEGER
| FLOAT
| STRING


 /****** DROP statement (example: DROP TABLE students;) ******/
 drop_statement
 : DROP TABLE opt_exists table_name ';'


%%
