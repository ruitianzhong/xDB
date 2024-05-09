%option noyywrap
%option case-insensitive
%{
#include <sstream>
#include <string.h>
#include <stdio.h>
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
CREATE ECHO;
USE
SHOW
TABLE
TABLES
FROM
WHERE
SELECT
DATABASE
INTO
VALUES
UPDATE
SET
DROP
JOIN
INNER
OUTER
LEFT
RIGHT
INT
INTEGER
VARCHAR
AS
BETWEEN
DELETE
IS
NOT
OR
NULL


 /* Operator */

"=="
"!="
"<>"
">="
"<="
"||"



[-+*/(){},.;<>=^%:?[\]|]    { printf("%c\n",yytext[0]); }



 /* Float point number */
[0-9]+"."[0-9]* |
"."[0-9]+ {
	printf("float point:%s\n",yytext);
}



 /* Integer number */

[0-9]+

 /* Identifier (placed after the keyword, lower priority in other words) */

 /*  \"[^\"\n]+\"  { printf("id: %s\n",yytext); } */

[A-Za-z][A-Za-z0-9_]* 




 /* Single quoted string */

\' { BEGIN(single_quoted_string); strbuf.clear();strbuf.str(""); }
<single_quoted_string>\'\' { strbuf<<'\''; }
<single_quoted_string>[^']* { strbuf<<yytext;}
<single_quoted_string>\' { BEGIN(INITIAL); printf("%s\n",strbuf.str().c_str()); }

<single_quoted_string><<EOF>> { fprintf(stderr,"[SQL-Lexer-Error] Unterminated string\n"); return 0;}


 /* Fallback */

. { fprintf(stderr, "[SQL-Lexer-Error] Unknown Character: %c\n", yytext[0]); return 0; }

%%

