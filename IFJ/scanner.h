/*Implementace překladače imperativního jazyka IFJ18
Jakub Gajdošík xgajdo24
Daniel Bílý xbilyd01
Petr Marek xmarek69
Jan Fridrich xfridr07
*/
#ifndef SCANNER_H
#define SCANNER_H
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

#include "token.h"
#include "symtable.h"

//konstanty pro jednotlive lexemy
//1 az 9 a 99 jsou chybove konstany, pouzit cokoliv krome nich
//def, do, else, end, if, not, nil, then, while

void get_token(token_t *token);
void free_lexem(lexem_t *lexem);
void free_all();

#endif