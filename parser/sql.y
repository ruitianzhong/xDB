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
%token IF EXISTS INTVAL EQUALS NOTEQUALS GREATEQ LESSEQ AND OR

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
%type opt_exists opt_column_list opt_where
%type expr operand between_expr logic_expr
%type scalar_expr unary_expr binary_expr comp_expr
%type literal string_literal integer_literal
%type update_clause_comma_list update_clause
%type select_comma_list table_list select_comma_list_with_star
%start statements

%%
statements
: statements statement { printf("done"); return 0; }
| statement {}

statement
: create_statement {}
| insert_statement {printf("insert\n");}
| drop_statement { printf("drop stmt\n"); }
| show_statement { printf("show_statement\n");}
| use_statement { printf("use_statement\n"); }
| update_statement { printf("update_statement\n"); }
| select_statement { printf("select_statement\n"); }
| delete_statement { printf("delete_statement\n"); }

table_name: IDENTIFIER

opt_exists: IF EXISTS {}
| /* empty */ { printf("opt");}

opt_column_list:
'(' comma_list ')'
|  {}

comma_list:
comma_list ',' IDENTIFIER
| IDENTIFIER

column_name
: IDENTIFIER
| IDENTIFIER '.' IDENTIFIER

opt_where
: WHERE expr {}
| {}

    /****** Expression ******/

expr: operand | between_expr | logic_expr ;
/* And/Or Priority?? */
logic_expr
: expr AND expr
| expr OR expr

between_expr
: operand BETWEEN operand AND operand

operand : '(' expr ')' {}
| scalar_expr | unary_expr | binary_expr {

}

scalar_expr
: column_name | literal

literal
: string_literal | integer_literal

string_literal
: STRING

integer_literal
: INTVAL

unary_expr
: '-' operand {}
| operand IS NULL {}
| operand IS NOT NULL {}

binary_expr
: comp_expr { }
| operand '-' operand {}
| operand '+' operand {}
| operand '-' operand {}
| operand '/' operand {}
| operand '*' operand {}
| operand '%' operand {}
| operand '^' operand {}

logic_expr : expr AND expr {}
| expr OR expr


comp_expr
: operand EQUALS operand
| operand NOTEQUALS operand
| operand GREATEQ operand
| operand LESSEQ operand
| operand '>' operand
| operand '<' operand
| operand '=' operand



 /****** CREATE statement ******/
create_statement
: CREATE TABLE table_name '(' fields_definition ')' ';'

fields_definition
: fields_definition ',' field_type | field_type

field_type: field type

field: IDENTIFIER

type
: CHAR '(' INTVAL ')' {}
| INT {}
| INTEGER

 /****** INSERT statement ******/

insert_statement
: INSERT INTO table_name opt_column_list VALUES '(' insert_values ')' ';'  { }

insert_values
: insert_values ',' insert_value
| insert_value

insert_value
: INTVAL
| FLOAT
| STRING


 /****** DROP statement (example: DROP TABLE students;) ******/
 drop_statement
 : DROP TABLE opt_exists table_name ';'

 /****** SHOW (SHOW TABLES) ******/
 show_statement
 : SHOW TABLES ';' {}
 | SHOW DATABASE ';'

 /****** USE ( USE example; ) ******/

 use_statement
 : USE table_name ';'


   /****** UPDATE ******/
 update_statement
 : UPDATE table_name SET update_clause_comma_list opt_where ';'

 update_clause_comma_list:
 update_clause_comma_list ',' update_clause
| update_clause

 /* can be improved */
update_clause: column_name '=' expr


/****** SELECT (select )******/
select_statement
: SELECT select_comma_list_with_star FROM table_list opt_where ';'

select_comma_list_with_star
:'*'
| '*' ',' select_comma_list

select_comma_list:
select_comma_list ',' column_name
| column_name

table_list:
table_list ',' table_name
| table_name

/****** DELETE ******/
delete_statement:
DELETE FROM table_name opt_where ';'

%%
