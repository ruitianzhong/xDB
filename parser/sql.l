%option noyywrap
%option case-insensitive
%{
#include <sstream>
#include <string.h>
#include <stdio.h>
#include "sql.tab.hpp"

#define TOKEN(name) { return SQL_##name; }

int chars = 0;
int words = 0;
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
	printf("float point:%s\n",yytext);
	return SQL_FLOAT;
}



 /* Integer number */

[0-9]+ { return SQL_INTVAL; }

 /* Identifier (placed after the keyword, lower priority in other words) */

 /*  \"[^\"\n]+\"  { printf("id: %s\n",yytext); } */

[A-Za-z][A-Za-z0-9_]* { return SQL_IDENTIFIER; }




 /* Single quoted string */

\' { BEGIN(single_quoted_string); strbuf.clear();strbuf.str(""); }
<single_quoted_string>\'\' { strbuf<<'\''; }
<single_quoted_string>[^']* { strbuf<<yytext;}
<single_quoted_string>\' { BEGIN(INITIAL); printf("%s\n",strbuf.str().c_str()); return SQL_STRING; }

<single_quoted_string><<EOF>> { fprintf(stderr,"[SQL-Lexer-Error] Unterminated string\n"); return 0;}


 /* Fallback */

. { fprintf(stderr, "[SQL-Lexer-Error] Unknown Character: %c\n", yytext[0]); return 0; }

%%

int yyerror(const char *msg) {
    fprintf(stderr, "[SQL-Lexer-Error] %s\n",msg); return 0;
}

