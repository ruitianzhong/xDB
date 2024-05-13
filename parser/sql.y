%{
#include <stdio.h>
#include "sql_lexer.h"
#include "string.h"
#include "sql/parser_result.h"
int yyerror(yyscan_t scanner,xDB::ParserResult * result,const char * s);
%}
%lex-param   { yyscan_t scanner }

%parse-param { yyscan_t scanner }
%parse-param { ParserResult * result }
%code requires{
#include "sql/stmts.h"
using namespace xDB;
}

%destructor {
    //
    free( ($$) );
}<str>

%define api.token.prefix {SQL_}

%token CREATE USE SHOW TABLE TABLES FROM
%token WHERE SELECT DATABASE INTO VALUES
%token UPDATE SET DROP JOIN INNER OUTER LEFT
%token RIGHT INT INTEGER VARCHAR AS BETWEEN
%token DELETE IS NOT NULL CHAR
%token  INSERT
%token<float_val> FLOATVAL
%token IF EXISTS EQUALS NOTEQUALS GREATEQ LESSEQ AND OR
%token<str> IDENTIFIER STRING
%token EXIT DATABASES
%token<int_val> INTVAL

%type<use_stmt> use_statement
%type<select_stmt> select_statement
%type<insert_stmt> insert_statement
%type<create_stmt> create_statement
%type<drop_stmt> drop_statement
%type<delete_stmt> delete_statement
%type<show_stmt> show_statement
%type<update_stmt> update_statement
%type<stmt> statement
%type<table_name> table_name dbname
%type<definition_list> fields_definition
%type<data_definition> field_type type
%type<insert_value>  insert_value
%type opt_exists
%type<exp> expr operand between_expr logic_expr scalar_expr unary_expr binary_expr comp_expr
%type<exp> literal string_literal integer_literal opt_where
%type<update_assign_list> update_clause_comma_list
%type<update_assign> update_clause
%type<table_list>  table_list
%type<column_name> column_name
%type<column_name_list> column_list opt_column_list select_comma_list_with_star select_comma_list
%type<insert_values_list> insert_values

%left OR
%left AND
%right NOT
%nonassoc '=' EQUALS NOTEQUALS
%nonassoc '<' '>' LESSEQ GREATEQ

%nonassoc IS
%left '+' '-'
%left '*' '/' '%'
%left '^'


/* Unary Operator */
%left '(' ')'
%left '.'
%left '[' ']'
%left JOIN


%type<stmt_vec> statements

%union{
    char * str;
    xDB::DropStmt* drop_stmt;
    std::vector<SQLStmt*> * stmt_vec;
    TableName table_name;
    xDB::SQLStmt* stmt;
    xDB::UseStmt * use_stmt;
    xDB::ColumnName * column_name;
    xDB::ShowStmt * show_stmt;
    double float_val;
    int int_val;
    xDB::InsertStmt * insert_stmt;
    std::vector<ColumnName*> *column_name_list;
    xDB::Parameter *insert_value;
    std::vector<Parameter*>* insert_values_list;
    xDB::DataDefinition* data_definition;
    std::vector<DataDefinition*>*definition_list;
    xDB::CreateStmt * create_stmt;
    xDB::Exp * exp;
    xDB::SelectStmt * select_stmt;
    std::vector<TableName> * table_list;
    xDB::DeleteStmt * delete_stmt;
    UpdateAssign * update_assign;
    std::vector<UpdateAssign*> * update_assign_list;
    UpdateStmt * update_stmt;
}


%%
statements
: statements statement {
     $$ = $1; // synthesized properties
     $$->push_back($2);
     result->addStatement($2);
}
| statement {
    $$ = new std::vector<SQLStmt*>();
    $$->push_back($1);
    result->addStatement($1);
}

statement
: create_statement { $$ = $1 ;}
| insert_statement { $$ =$1;}
| drop_statement { printf("drop stmt\n"); $$ = $1;}
| show_statement { printf("show_statement\n");$$ = $1;}
| use_statement { printf("use_statement\n"); $$ = $1;}
| update_statement { printf("update_statement\n"); $$ = $1;}
| select_statement {  $$ = $1;}
| delete_statement { $$ = $1; }
| EXIT ';' { $$ = new ExitStmt(); }

table_name: IDENTIFIER {
    $$.name = $1;
    $$.schema = nullptr;
}
| IDENTIFIER '.' IDENTIFIER {
    $$.name = $3;
    $$.schema = $1;
}

opt_exists: IF EXISTS {}
| /* empty */ { }

opt_column_list:
'(' column_list ')' { $$ = $2;}
|  { $$ = nullptr;}

column_list:
column_list ',' column_name {
    $$ = $1;
    $$->push_back($3);
 }
| column_name {
    $$ = new std::vector<ColumnName*>();
    $$->push_back($1);
}

column_name
: IDENTIFIER{
    $$ = new ColumnName(nullptr,nullptr,$1);
}
| IDENTIFIER '.' IDENTIFIER {
    $$ = new ColumnName(nullptr,$1,$3);
}
| IDENTIFIER '.' IDENTIFIER '.' IDENTIFIER{
   $$ = new ColumnName($1,$3,$5);
}

opt_where
: WHERE expr { }
| { $$=nullptr; }

    /****** Expression ******/

expr: operand {
    $$ = $1;
 }
| between_expr{
    $$ = $1;
}
| logic_expr { $$ =$1; }
/* And/Or Priority?? */
logic_expr
: expr AND expr {
    $$ = new BinaryExp(BinaryAND,$1,$3);
}
| expr OR expr {
    $$ = new BinaryExp(BinaryOR,$1,$3);
}

between_expr
: operand BETWEEN operand AND operand {
    $$ = new BetweenExpr($1,$3,$5);
}

operand : '(' expr ')' { $$ = $2; }
| scalar_expr { $$ = $1; }
| unary_expr { $$ = $1;}
| binary_expr {
    $$ = $1;
}

scalar_expr
: column_name{ $$ = new ScalarExp($1);}
| literal { $$ = $1; }

literal
: string_literal { $$ = $1;}
| integer_literal { $$ = $1;}
| NULL { $$ = new ScalarExp(); }

string_literal
: STRING {
    $$ = new ScalarExp($1);
}

integer_literal
: INTVAL {
    $$ = new ScalarExp($1);
}

unary_expr
: '-' operand {
    $$ = new UnaryExp(UnaryNegate,$2);
}
| operand IS NULL {
    $$ = new UnaryExp(UnaryIsNull,$1);
}
| operand IS NOT NULL {
    $$ = new UnaryExp(UnaryIsNotNull,$1);
}
| NOT operand {
    $$ = new UnaryExp(UnaryNot,$2);
}

binary_expr
: comp_expr {
    $$ = $1;
}
| operand '-' operand {
    $$ = new BinaryExp(BinarySub,$1,$3);
}
| operand '+' operand {
    $$ = new BinaryExp(BinaryAdd,$1,$3);
}
| operand '/' operand {
    $$ = new BinaryExp(BinaryDiv,$1,$3);
}
| operand '*' operand {
    $$ = new BinaryExp(BinaryMul,$1,$3);
}
| operand '%' operand {
    $$ = new BinaryExp(BinaryModulo,$1,$3);
}
| operand '^' operand {
    $$ = new BinaryExp(BinaryXOR,$1,$3);
}



comp_expr: operand '=' operand{
    $$ = new BinaryExp(BinaryEquals,$1,$3);
}
| operand EQUALS operand{
     $$ = new BinaryExp(BinaryEquals,$1,$3);
}
| operand NOTEQUALS operand {
     $$ = new BinaryExp(BinaryNotEquals,$1,$3);
}
| operand GREATEQ operand {
     $$ = new BinaryExp(BinaryGreatEQ,$1,$3);
}
| operand LESSEQ operand {
     $$ = new BinaryExp(BinaryLessEQ,$1,$3);
}
| operand '>' operand {
     $$ = new BinaryExp(BinaryGreat,$1,$3);
}
| operand '<' operand {
     $$ = new BinaryExp(BinaryLess,$1,$3);
}



 /****** CREATE statement ******/
create_statement
: CREATE TABLE table_name '(' fields_definition ')' ';'{
    $$ = new CreateStmt($3,$5);
}
| CREATE DATABASE dbname ';'{
    $$ = new CreateStmt($3);
}


fields_definition
: fields_definition ',' field_type {
    $$ = $1;
    $$->push_back($3);
}
| field_type {
    $$ = new std::vector<DataDefinition*>();
    $$->push_back($1);
}

field_type: IDENTIFIER type {
    $$ = $2;
    $$->setName($1);
}


type
: CHAR '(' INTVAL ')' { $$ = new CharDefinition(CHAR,$3); }
| INT { $$ = new DataDefinition(INTEGER); }
| INTEGER { $$ = new DataDefinition(INTEGER); }

 /****** INSERT statement ******/

insert_statement
: INSERT INTO table_name opt_column_list VALUES '(' insert_values ')' ';'  {
    $$ = new InsertStmt($3,$4,$7);
 }

insert_values
: insert_values ',' insert_value {
    $$ = $1;
    $1->push_back($3);
}
| insert_value {
    $$ = new std::vector<Parameter*>();
    $$->push_back($1);
}

insert_value
: INTVAL {
    $$ = new Parameter($1);
}
| FLOATVAL {
    $$ = new Parameter($1);
}
| STRING {
    $$ = new Parameter($1);
}


 /****** DROP statement (example: DROP TABLE students;) ******/
 drop_statement
 : DROP TABLE opt_exists table_name ';' {
   $$ = new DropStmt(DropTable,$4);

 }
 | DROP DATABASE opt_exists dbname ';' {
    $$ = new DropStmt(DropDatabase,$4);
 }

 dbname:
 IDENTIFIER {
    $$.name = nullptr;
    $$.schema = $1;
 }

 /****** SHOW (SHOW TABLES) ******/
 show_statement
 : SHOW TABLES ';' {
    $$ = new ShowStmt(ShowTables);
 }
 | SHOW DATABASES ';' {
    $$ = new ShowStmt(ShowDatabases);
 }

 /****** USE ( USE example; ) ******/

 use_statement
 : USE dbname ';'{
    $$ = new UseStmt($2);
 }


   /****** UPDATE ******/
 update_statement
 : UPDATE table_name SET update_clause_comma_list opt_where ';'{
    $$ = new UpdateStmt($2,$4,$5);
 }

 update_clause_comma_list:
 update_clause_comma_list ',' update_clause {
    $$ = $1;
    $$->push_back($3);
 }
| update_clause {
    $$ = new std::vector<UpdateAssign*>();
    $$->push_back($1);
}

 /* can be improved */
update_clause: column_name '=' expr {
    $$ = new UpdateAssign($1,$3);
}


/****** SELECT (select )******/
select_statement
: SELECT select_comma_list_with_star FROM table_list opt_where ';'{
    $$ = new SelectStmt($2,$4,$5);
}

select_comma_list_with_star
:'*' {
    char* s = strdup("*");
    auto c = new ColumnName(nullptr,nullptr,s);
    $$ = new std::vector<ColumnName*>();
    $$->push_back(c);
}
| '*' ',' select_comma_list {
    char * s = strdup("*");
    auto c = new ColumnName(nullptr,nullptr,s);
    $3->insert($3->begin(),c);
    $$ = $3;
}
| select_comma_list {
    $$ = $1;
}

select_comma_list:
select_comma_list ',' column_name {
    $$ = $1;
    $$->push_back($3);
}
| column_name {
    $$ = new std::vector<ColumnName*>();
    $$->push_back($1);
}

table_list:
table_list ',' table_name {
    $$ = $1;
    $$->push_back($3);
}
| table_name {
    $$ = new std::vector<TableName>();
    $$->push_back($1);
}

/****** DELETE ******/
delete_statement:
DELETE FROM table_name opt_where ';' {
    $$ = new DeleteStmt($3,$4);
}

%%

int wrapped_parse(const char*text, ParserResult * result){
    yyscan_t scanner;
    YY_BUFFER_STATE state;

    if (yylex_init(&scanner)){
        fprintf(stderr,"SQLParser error");
        return 0;
    }
    state = yy_scan_string(text,scanner);

    int ret = yyparse(scanner,result);

    yy_delete_buffer(state,scanner);
    yylex_destroy(scanner);
    return 1;
}

int yyerror(yyscan_t scanner,ParserResult * result ,const char *msg) {
    fprintf(stderr, "[SQL-Lexer-Error] %s\n",msg);return 0;
}
