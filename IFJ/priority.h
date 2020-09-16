/*Implementace překladače imperativního jazyka IFJ18
Jakub Gajdošík xgajdo24
Daniel Bílý xbilyd01
Petr Marek xmarek69
Jan Fridrich xfridr07
*/
#ifndef PRIORITY_H
#define PRIORITY_H
#include "token.h" 

#define TABLE_SIZE 8

#define E 1 /// = EQUAL
#define S 2 // < SHIFT
#define R 3 /// > REDUCE
#define N 4 //ERROR

#define NONTERM 1000 //nonterm


#define I_PLUS_MINUS 0
#define I_MUL_DIV 1
#define I_REL_OP 3
#define I_LEFT_BRACKET 4
#define I_RIGHT_BRACKET 5
#define I_DATA 6
#define I_DOLLAR 7

int prior[TABLE_SIZE][TABLE_SIZE];

int get_prec_table_index(int symbol);

#endif