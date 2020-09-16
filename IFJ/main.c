/*Implementace překladače imperativního jazyka IFJ18
Jakub Gajdošík xgajdo24
Daniel Bílý xbilyd01
Petr Marek xmarek69
Jan Fridrich xfridr07
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "token.h"
#include "scanner.h"
#include "parser.h" 
#include "symtable.h"
#include "generator.h"
#include "stack.h"
#include "priority.h"



//extern FILE* stdin;

void exit_error(int exit_code)
{
	if (exit_code == ERROR_1)
	{
		free_all();
		exit(1);
	}
	else if (exit_code == ERROR_2)
	{
		free_all();
		exit(2);
	}
	else if (exit_code == ERROR_3)
	{
		free_all();
		exit(3);
	}
	else if (exit_code == ERROR_4)
	{
		free_all();
		exit(4);
	}
	else if (exit_code == ERROR_5)
	{
		free_all();
		exit(5);
	}
	else if (exit_code == ERROR_6)
	{
		free_all();
		exit(6);
	}
	else if (exit_code == ERROR_9)
	{
		free_all();
		exit(9);
	}
	else if (exit_code == ERROR_99)
	{
		free_all();
		exit(99);
	}
}


int main(int argc, char *argv[])
{
	
	if (argc != 1)
	{
		fprintf(stderr, "Nesparvny pocet argumentu.\n");
		free_all();
		exit(99);
	}
	

	int result =program();
	exit_error(result);
	free_all();
	fclose(stdin);

	return 0;
}