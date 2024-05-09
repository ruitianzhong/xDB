# Notes on parser

## Interfacing with yacc (from flex's docs)

One of the main uses of 'flex' is as a companion to the 'yacc'
parser-generator.  'yacc' parsers expect to call a routine named
'yylex()' to find the next input token. The routine is supposed to
return the type of the next token as well as putting any associated
value in the global 'yylval'. To use 'flex' with 'yacc', one specifies
the '-d' option to 'yacc' to instruct it to generate the file 'y.tab.h'
containing definitions of all the '%tokens' appearing in the 'yacc'
input. This file is then included in the 'flex' scanner. For example,
if one of the tokens is 'TOK_NUMBER', part of the scanner might look
like:

```lex
         %{
         #include "y.tab.h"
         %}
     
         %%
     
         [0-9]+        yylval = atoi( yytext ); return TOK_NUMBER;
```

## Action in flex

Actions can include arbitrary C code, including 'return' statements
to return a value to whatever routine called `yylex()`. Each time
`yylex()` is called it continues processing tokens from where it last
left off until it either reaches the end of the file or executes a
return.

Actions are free to modify `yytext` except for lengthening it (adding
characters to its end-these will overwrite later characters in the input
stream).

## Start Condition

'flex' provides a mechanism for conditionally activating rules. Any
rule whose pattern is prefixed with '<sc>' will only be active when the
scanner is in the "start condition" named 'sc'. For example,

```
         <STRING>[^"]*        { /* eat up the string body ... */
                     ...
                     }
```

will be active only when the scanner is in the 'STRING' start
condition, and

```
         <INITIAL,STRING,QUOTE>\.        { /* handle an escape ... */
                     ...
                     } 
```

will be active only when the current start condition is either
'INITIAL', 'STRING', or 'QUOTE'.

Example for detecting multiline comment in C:

```lex
 %x comment
         %%
                 int line_num = 1;
     
         "/*"         BEGIN(comment);
         <comment>[^*\n]*        /* eat anything that's not a '*' */
         <comment>"*"+[^*/\n]*   /* eat up '*'s not followed by '/'s */
         <comment>\n             ++line_num;
         <comment>"*"+"/"        BEGIN(INITIAL);
```