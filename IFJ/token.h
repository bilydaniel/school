/*Implementace překladače imperativního jazyka IFJ18
Jakub Gajdošík xgajdo24
Daniel Bílý xbilyd01
Petr Marek xmarek69
Jan Fridrich xfridr07
*/
#ifndef TOKEN_H
#define TOKEN_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

   


typedef struct Lexem {
	int lenght;
	char *word;	
} lexem_t;


typedef union //union smi mit pouze jednu z hodnot
{
	int integer;//hodnota kdyy je int
	double decimal;
	lexem_t *string;//hodnota kdyz je string
	int keyword;//konstanta pro keyword
	int nil;//bool hodnota(je nebo neni to nil)
	
} attribute_t;


typedef struct Token {
   	int type;
  	attribute_t attribute;
  	
} token_t;

void token_init(token_t *token);
void lexem_init(lexem_t *lexem);
void lexem_putchar(lexem_t *lexem, char symbol);
void set_token(int type, char *attribute);
void lexem_init(lexem_t *lexem);
void lexem_putchar(lexem_t *lexem, char symbol);
void set_type(token_t *token, int type);
void keyword_check(token_t *token, lexem_t *lexem);
void lexem_putstr(lexem_t *lexem, const char *str);
void lexem_del_word(lexem_t *lexem);




#define FALSE 0
#define TRUE 1


#define END_OF_FILE 10
#define END_OF_LINE 11
#define NIL 13


#define DEF 20
#define DO 21
#define END 22

#define INPUTS 23
#define INPUTI 24
#define INPUTF 25
#define PRINT 26
#define LENGHT 27
#define SUBSTR 28
#define ORD 29
#define CHR 30

#define ELSE 31
#define THEN 32
#define NOT 33
#define WHILE 34
#define IF 35


#define STATE_START 40
#define STATE_ID_KW 41
#define STATE_NUMBER 42
#define STATE_COMMENT 43 // #
#define STATE_EQUALS 44 // = nevime jestli bude pokracovat druhe rovna se
#define STATE_LESSTHAN 45 // <
#define STATE_MORETHAN 46 // >
#define STATE_EXCLAMATION 47 // !
#define STATE_STRING_LITERAL 48
#define STATE_DECIMAL 49
#define STATE_DECIMAL_DOT 50
#define STATE_DECIMAL_E 51
#define STATE_DECIMAL_END 52
#define STATE_DECIMAL_SIGN 53
#define STATE_DECIMAL_E_SIGN 54
#define STATE_ESCAPE_SEQUENCE 55
#define STATE_HEXADECIMAL 56
#define STATE_HEXADECIMAL_SECOND 57


#define PLUS 60 // +
#define MINUS 61 // -
#define DIV 62 // /
#define MUL 63 // *
#define EQUALS 64 // = 
#define COMMA 65 // ,
#define LEFT_BRACKET 66 // (
#define RIGHT_BRACKET 67 // )
#define COMPARE 68 // ==
#define LOE 69 // <=
#define LESSTHAN 70 // <
#define MOE 71 // >=
#define MORETHAN 72 // >
#define NOTEQUAL 73 // !=


#define TYPE_KEYWORD 80
#define TYPE_IDENTIFIER 81
#define TYPE_INT 82
#define TYPE_FLOAT 83
#define TYPE_STRING 84


#endif