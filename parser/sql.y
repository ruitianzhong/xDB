%{
#include <stdio.h>
int yylex(void);
void yyerror(const char * s);
%}

%token CREATE
%start top

%%
top
: CREATE { printf("CREATE"); }
%%
