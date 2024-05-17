%option noyywrap
%option reentrant
%option nounput
%option never-interactive
%option case-insensitive
%{
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "sql.tab.hpp"

#define TOKEN(name) { return SQL_##name; }

int chars = 0;
int words = 0;
const char * lexer_str = nullptr;
unsigned long slen = 0;
unsigned long cur = 0;


static thread_local std::stringstream strbuf;

%}

%x single_quoted_string
%s comment
DIGIT [0-9]
NumberFirstID [0-9]+[a-z]+[a-z0-9]*
CharFirstID [a-z][a-z0-9]*
ID {CharFirstID}|{NumberFirstID}

/*
 Rules
*/

%%
 /* SQL comment */

-- { BEGIN(comment); }
<comment>[^\n]* /* skipping the comment */
<comment>[\n] { BEGIN(INITIAL); }

[ \t\n]+      /* skipping the whitespace */

 /* SQL keyword */
CREATE TOKEN(CREATE)
USE TOKEN(USE)
SHOW TOKEN(SHOW)
TABLE TOKEN(TABLE)
TABLES TOKEN(TABLES)
FROM TOKEN(FROM)
WHERE TOKEN(WHERE)
SELECT TOKEN(SELECT)
DATABASE TOKEN(DATABASE)
DATABASES TOKEN(DATABASES)
INTO TOKEN(INTO)
VALUES TOKEN(VALUES)
UPDATE TOKEN(UPDATE)
SET TOKEN(SET)
DROP TOKEN(DROP)
JOIN TOKEN(JOIN)
INNER TOKEN(INNER)
OUTER TOKEN(OUTER)
LEFT TOKEN(LEFT)
RIGHT TOKEN(RIGHT)
INT TOKEN(INT)
INTEGER TOKEN(INTEGER)
VARCHAR TOKEN(VARCHAR)
AS TOKEN(AS)
BETWEEN TOKEN(BETWEEN)
DELETE TOKEN(DELETE)
IS TOKEN(IS)
NOT TOKEN(NOT)
OR TOKEN(NOT)
NULL TOKEN(NULL)
CHAR TOKEN(CHAR)
INSERT TOKEN(INSERT)
IF TOKEN(IF)
EXISTS TOKEN(EXISTS)
AND TOKEN(AND)
EXIT TOKEN(EXIT)


 /* Operator */

"==" TOKEN(EQUALS)
"!=" TOKEN(NOTEQUALS)
"<>" TOKEN(NOTEQUALS)
">=" TOKEN(GREATEQ)
"<=" TOKEN(LESSEQ)
"||"



[-+*/(){},.;<>=^%:?[\]|]    { return yytext[0]; }



 /* Float point number */
[0-9]+"."[0-9]* |
"."[0-9]+ {
	yylval.float_val = atof(yytext);
	return SQL_FLOATVAL;
}



 /* Integer number */

[0-9]+ { yylval.int_val = atoi(yytext);return SQL_INTVAL; }

 /* Identifier (placed after the keyword, lower priority in other words) */

 /*  \"[^\"\n]+\"  { printf("id: %s\n",yytext); } */

[A-Za-z][A-Za-z0-9_]* { yylval.str = strdup(yytext); return SQL_IDENTIFIER; }




 /* Single quoted string */

\' { BEGIN(single_quoted_string); strbuf.clear();strbuf.str(""); }
<single_quoted_string>\'\' { strbuf<<'\''; }
<single_quoted_string>[^']* { strbuf<<yytext;}
<single_quoted_string>\' { BEGIN(INITIAL); yylval.str= strdup(strbuf.str().c_str()); return SQL_STRING; }

<single_quoted_string><<EOF>> { fprintf(stderr,"[SQL-Lexer-Error] Unterminated string\n"); return 0;}


 /* Fallback */

. { fprintf(stderr, "[SQL-Lexer-Error] Unknown Character: %c\n", yytext[0]); return 0; }

%%

