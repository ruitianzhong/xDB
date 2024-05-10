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

## Bison

### Definition

+ %type -> non-terminal types
+ %token -> terminal types (upper-case conventionally)
+ %left (+-*/) & %right (=) & %nonassoc

Example:

```bison
%left AA BB
%nonassoc CC
%right DD
```

+ %union 定义了语法符号的语义值类型的集合。在 Bison 中，每个符号，包括记号和非终结符，都有一个不同数据类型的语义值，并且这些值通过
  yylval 变量在移进和归约操作中传递。默认情况下，YYSTYPE（宏定义）为 yylval 的类型，通常为 int。但通过使用
  %union，你可以重新定义符号的类型。使用 union 是因为不同节点可能需要不同类型的语义值，比如下面的定义，希望 ADDOP 的 值是
  char 类型，而 NUMBER 应该是 double 类型。

Example:

```bison
%token <num> NUMBER
%token <op> ADDOP MULOP LPAREN RPAREN
%union {
 char op;
 double num;
}

# 注意：一旦 %union 被定义，需要指定 Bison 每种符号使用的值类型，值类型通过放在尖括号中的 union 类型对应的成员名称确定，如 %token <num>。
```

+ 用 %start 指定文法的开始符号，表示最终需要规约成的符号，例如 %start program。如果不使用 %start 定义文法开始符
  号，则默认在第二部分规则段中定义的第一条生产式规则的左部非终结符为开始符号。

### Grammar Rules

+ 语法规则部分由归约规则和动作组成。
  规则基本按照巴科斯范式（BNF）描述。
  规则中目标或非终端符放在左边，后跟一个冒号：然后是 产生式的右边，之后是对应的动作（用 {} 包含）。
  Bison 的语法树是按自下而上的归约方式进行构建的。

Example:

```bison
%%
 program: program expr '\n' { printf("%d\n", $2); }
 ;
 expr: expr '+' expr { $$ = $1 + $3; }
 | expr '-' expr { $$ = $1 - $3}
 ;
%%
/* 动作中“$1”表示右边的第一个标记的值，“$2”表示右边的第二个标记的值，依
次类推。“$$”表示归约后的值。以“expr: expr '+' expr { $$ = $1 + $3; }”，说明“$$”表示从左
向右第一个 expr，即规约的结果，“$1”表示从左向右第二个 expr，“$2”表示“+”加号，“$3”
表示从左向右第三个 expr。“|”符号表示其他的规约规则。 */
```

A simple example(get everything together)

```bison
/* file name : bison_demo.y */
%{
#include <stdio.h>
/* 定义部分 */
/* 这部分代码会被原样拷贝到生成的 .c 文件的开头 */
int yylex(void);
void yyerror(const char *s);
%}

/* 定义部分 */
/* 对语法的终结符和非终结符进行声明 */
%start reimu
%token REIMU

/* 从这里开始，下面是解析规则 */
%%
reimu : marisa { /* 这里写与该规则对应的处理代码 */ puts("rule1"); }
      | REIMU  { /* 这里写与该规则对应的处理代码 */ puts("rule2"); }
      ; /* 规则最后不要忘了用分号结束哦～ */

 /* 这种写法表示 ε —— 空输入 */
marisa : { puts("Hello!"); }
%%


/* 以下是 C 代码部分 */
/* 在这个 Bison 程序中，我们没有联动 Flex，所以手写一个 yylex 函数 */
int yylex(void)
{
    int c = getchar(); // 从 stdin 获取下一个字符
    switch (c) {
    case EOF: return YYEOF;
    case 'R': return REIMU;
    default:  return YYUNDEF;     // 报告 token 未定义，迫使 bison 报错。
    // 由于 bison 不同版本有不同的定义。如果这里 YYUNDEF 未定义，请尝试 YYUNDEFTOK 或使用一个随意的整数。
    }
}

void yyerror(const char *s)
{
    fprintf(stderr, "%s\n", s);
}

int main(void)
{
    yyparse(); // 启动解析
    return 0;
}
```

### Compile Bison

```shell
# -d generate bison_demo.tab.h
bison -d bison_demo.y
```
