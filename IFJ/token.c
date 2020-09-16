/*Implementace překladače imperativního jazyka IFJ18
Jakub Gajdošík xgajdo24
Daniel Bílý xbilyd01
Petr Marek xmarek69
Jan Fridrich xfridr07
*/
#include "token.h"

 

void lexem_init(lexem_t *lexem)
{
	if (lexem == NULL)
	{
		fprintf(stderr, "nepodarilo se alokovat lexem");
		exit(99);
	}
	lexem->word = malloc(sizeof(char));
	if (lexem->word == NULL)
	{
		fprintf(stderr, "nepodarilo se alokovat lexem->word");
		exit(99);
	}
	lexem->lenght = 0;
	lexem->word[0] = '\0';

}

void lexem_putchar(lexem_t *lexem, char symbol)
{
	lexem->word[lexem->lenght] = symbol;
	lexem->lenght++;
	lexem->word = realloc(lexem->word, (lexem->lenght+1));
	if (lexem->word == NULL)
	{
		fprintf(stderr, "nepodarilo se realokovat lexem");
		exit(99);
	}
	lexem->word[lexem->lenght] = '\0';
}

void lexem_putstr(lexem_t *lexem, const char *str)
{
	int new_lenght = lexem->lenght + strlen(str);

	lexem->word = realloc(lexem->word, (new_lenght+1)*sizeof(char));
	if(lexem->word == NULL)
	{
		fprintf(stderr, "nepodarilo se realokovat lexem");
		exit(99);
	}

	lexem->lenght = new_lenght;
	strcat(lexem->word, str);
}

void lexem_del_word(lexem_t *lexem)
{
	lexem->lenght = 0;
	lexem->word = realloc(lexem->word, sizeof(char));
	if (lexem->word == NULL)
	{
		fprintf(stderr, "nepodarilo se realokovat lexem");
		exit(99);
	}
	lexem->word[lexem->lenght] = '\0';
}



void token_init(token_t *token)
{
	token->type = 0;
}

void set_type(token_t *token, int type)
{
	token->type = type;
}

void keyword_check(token_t *token, lexem_t *lexem)
{
	if (!strcmp(lexem->word, "def"))
		token->type = DEF;
	else if (!strcmp(lexem->word, "do"))
		token->type = DO;
	else if (!strcmp(lexem->word, "else"))
		token->type = ELSE;
	else if (!strcmp(lexem->word, "end"))
		token->type = END;
	else if (!strcmp(lexem->word, "if"))
		token->type = IF;
	else if (!strcmp(lexem->word, "not"))
		token->type = NOT;
	else if (!strcmp(lexem->word, "nil"))
		token->type = NIL;
	else if (!strcmp(lexem->word, "then"))
		token->type = THEN;
	else if (!strcmp(lexem->word, "while"))
		token->type = WHILE;
	else if (!strcmp(lexem->word, "inputs"))
		token->type = INPUTS;
	else if (!strcmp(lexem->word, "inputi"))
		token->type = INPUTI;
	else if (!strcmp(lexem->word, "inputf"))
		token->type = INPUTF;
	else if (!strcmp(lexem->word, "print"))
		token->type = PRINT;
	else if (!strcmp(lexem->word, "length"))
		token->type = LENGHT;
	else if (!strcmp(lexem->word, "substr"))
		token->type = SUBSTR;
	else if (!strcmp(lexem->word, "ord"))
		token->type = ORD;
	else if (!strcmp(lexem->word, "chr"))
		token->type = CHR;
	else
		token->type = TYPE_IDENTIFIER;
}