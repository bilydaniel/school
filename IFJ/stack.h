/*Implementace překladače imperativního jazyka IFJ18
Jakub Gajdošík xgajdo24
Daniel Bílý xbilyd01
Petr Marek xmarek69
Jan Fridrich xfridr07
*/
#ifndef STACK_H
#define STACK_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "generator.h"
#include "token.h"

#define MAXSTACK 1000


/* pomocné zásobníky */
typedef struct	{                          /* zásobník hodnot typu int */
    int a[MAXSTACK];
    int top;
} tStack;

typedef struct	{                         
    token_t a[MAXSTACK];
    int top;
} Token_Stack;


void SInit (tStack *Stack);
void SPush (tStack *Stack, int token_type);
int SPop (tStack *Stack);
int STop (tStack *Stack);
bool SEmpty (tStack *Stack);


void TInit (Token_Stack *Stack);
void TPush (Token_Stack *Stack, token_t token);
token_t TPop (Token_Stack *Stack);
token_t TTop (Token_Stack *Stack);
bool TEmpty (Token_Stack *Stack);

#endif