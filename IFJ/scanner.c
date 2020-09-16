/*Implementace překladače imperativního jazyka IFJ18
Jakub Gajdošík xgajdo24
Daniel Bílý xbilyd01
Petr Marek xmarek69
Jan Fridrich xfridr07
*/
#include "scanner.h"


extern tHTable* h_tabulka;
extern tHTable* tmp_tabulka;
extern token_t* token;
extern lexem_t *code_while;
extern lexem_t *code_main;
extern lexem_t *code_rest;


//FILE *stdin = stdin;
int file_begin = 1;


void free_lexem(lexem_t *lexem)
{
	free(lexem->word);		
	free(lexem);

}


void free_all()
{
	free(token);
	free_lexem(code_main);
	free_lexem(code_rest);
	free_lexem(code_while);
	htClearAll(h_tabulka);
	free(h_tabulka);
	htClearAll(tmp_tabulka);
	free(tmp_tabulka);
}


void get_token(token_t *token)
{

	lexem_t *lexem = malloc(sizeof (lexem_t));
	if(lexem == NULL)
	{
		free_all();
		exit(1);
	}
	lexem_init(lexem);
	int state = STATE_START;//vychozi stav automatu
	char symbol; //Zde uchovavame vzdy dalsi nacteny znak z prekladaneho souboru
	char hex_value[2];
	while(TRUE)
	{
		symbol = fgetc(stdin);//nacteni jednoho symbolu(pismene)
		//blokovy komentar
		if (symbol == '\n' || file_begin)
		{
			char tmp_symbol;
			if(file_begin)
			{
				tmp_symbol = symbol;
				symbol = ' ';
			}
			else 
				tmp_symbol = fgetc(stdin); //pokud byl novi radek tak otestujem zda neni dalsi znak blokovy komentar =begin
			file_begin = 0;
			if (tmp_symbol == '=')
			{
				int comment = 1;
				lexem_t *lexem_com = malloc(sizeof (lexem_t));//pomocny lexem
				
				if(lexem_com == NULL)
				{
					free_lexem(lexem);
					free_lexem(lexem_com);
					free_all();
					exit(1);
				}
				
				lexem_init(lexem_com);
				while (!isspace(tmp_symbol) || tmp_symbol == EOF) //nacitej slovo (ocekavame =begin)
				{
					lexem_putchar(lexem_com, tmp_symbol);
					tmp_symbol = fgetc(stdin);
				}
				if(tmp_symbol == EOF)
				{
					free_lexem(lexem);
					free_lexem(lexem_com);
					free_all();
					exit(1);
				}
				
				if (!strcmp(lexem_com->word,"=begin")) //porovnej zda se jedna o =begin
				{
					lexem_del_word(lexem_com);
					while(comment)
					{
						while((tmp_symbol = fgetc(stdin)) != '\n' && tmp_symbol != EOF); //maz vsechno dokud nenastane =end
						

						if(tmp_symbol == EOF)
						{
							free_lexem(lexem);
							free_lexem(lexem_com);
							free_all();
							exit(1);
						}	

						tmp_symbol = fgetc(stdin);

						if (tmp_symbol == '=') //pokud je znak "=" kontrolujeme zda je to =end
						{
							while (!isspace(tmp_symbol) || tmp_symbol == EOF) //nacti slovo
							{
								lexem_putchar(lexem_com, tmp_symbol);
								tmp_symbol = fgetc(stdin);
							}
							if(tmp_symbol == EOF)
							{
								free_lexem(lexem);
								free_lexem(lexem_com);
								free_all();
								exit(1);
							}	
							if (!strcmp(lexem_com->word,"=end")) //porovnej slovo zda je to =end
							{
								comment = 0; //ukonci cyklus
								while((tmp_symbol = fgetc(stdin)) != '\n'); //smaz zbytek radku
							}
							else
							{
								free_lexem(lexem);
								free_lexem(lexem_com);
								free_all();
								exit(1);
							}
						}
					}
				}
				else
				{
					free_lexem(lexem);
					free_lexem(lexem_com);
					free_all();
					exit(1);
				}
			}
			else
				ungetc(tmp_symbol, stdin); //pokud znak na novem radku neby "=" tak ho vrat
		}
		switch(state)
		{
			case STATE_START:	
				if (symbol == EOF)
				{
					token->type = END_OF_FILE;
					free_lexem(lexem);
					return;
				}
				
				else if(symbol == '\n')
				{
					token->type =END_OF_LINE;
					free_lexem(lexem);
					return;
				}
				else if(symbol == '+')
				{
					token->type = PLUS;
					free_lexem(lexem);
					return;
				}
				else if(symbol == '-')
				{
					token->type = MINUS;
					free_lexem(lexem);
					return;
				}
				else if(symbol == '*')
				{
					token->type = MUL;
					free_lexem(lexem);
					return;
				}
				else if(symbol == '/')
				{
					token->type = DIV;
					free_lexem(lexem);
					return;
				}
				else if(symbol == '=')
				{
					state = STATE_EQUALS;
				}
				else if(symbol == '<')
				{
					state = STATE_LESSTHAN;
				}
				else if(symbol == '>')
				{
					state = STATE_MORETHAN;
				}
				else if(symbol == '!')
				{
					state = STATE_EXCLAMATION;
				}
				else if(symbol == ',')
				{
					token->type = COMMA;
					free_lexem(lexem);
					return;
				}				
				else if(symbol == '(')
				{
					token->type = LEFT_BRACKET;
					free_lexem(lexem);
					return;
				}
				else if(symbol == ')')
				{
					token->type = RIGHT_BRACKET;
					free_lexem(lexem);
					return;
				}
				

				else if (symbol == '#')
				{
					state = STATE_COMMENT;
				}
				else if(isspace(symbol)) //hledej dalsi znak
				{	
					state = STATE_START;
				}

				else if (islower(symbol) || symbol == '_') //promena nebo funkce nebo klicove slovo
				{
					state = STATE_ID_KW;
					lexem_putchar(lexem, symbol);
				}
				else if (isdigit(symbol)) //cislo
				{
					state = STATE_NUMBER;
					lexem_putchar(lexem, symbol);
				}
				else if (symbol == '\"') //string
				{
					state = STATE_STRING_LITERAL;
				}
				else //chyba neplatny symbol
				{
					free_lexem(lexem);
					free_all();
					exit(1);
				}
			break;

			case STATE_EQUALS:
				if(symbol == '=') // ==
				{
					token->type = COMPARE;
					free_lexem(lexem);
					return;
				}
				else 
				{
					token->type = EQUALS; //=x
					ungetc(symbol, stdin);
					free_lexem(lexem);
					return;
				}
			break;

			case STATE_LESSTHAN:
				if(symbol == '=') //<=
				{
					token->type = LOE;//less or equal
					free_lexem(lexem);
					return;
				}
				else
				{
					token->type = LESSTHAN; //<x
					ungetc(symbol, stdin);
					free_lexem(lexem);
					return;
				}
			break;

			case STATE_MORETHAN:
				if(symbol == '=') //>=
				{
					token->type = MOE;//more or equal
					free_lexem(lexem);
					return;
				}
				else 
				{
					token->type = MORETHAN; //>x
					ungetc(symbol, stdin);
					free_lexem(lexem);
					return;
				}
			break;

			case STATE_EXCLAMATION: //!=
				if(symbol == '=')
				{
					token->type = NOTEQUAL;
					free_lexem(lexem);
					return;
				}
				else //!x chyba
				{
					free_lexem(lexem);
					free_all();
					exit(1);
					return;
				}
			break;
			
			
			case STATE_ID_KW: //klicove slova promene atd.
				if(isalnum(symbol) || symbol == '_')
				{
					lexem_putchar(lexem, symbol);
				}
				else
				{
					ungetc(symbol, stdin);
					keyword_check(token, lexem);//kontrola zda se nejdna o klicove slovo nebo preddefinovanou fci
					token->attribute.string = lexem;
					return;
				}
			break;


			case STATE_NUMBER: //cisla
				if(isdigit(symbol))
				{
					lexem_putchar(lexem, symbol);
				}
				else if (symbol == '.' )
				{
					lexem_putchar(lexem, symbol);
					state = STATE_DECIMAL_DOT;
				}
				else if(symbol == 'e' || symbol == 'E')
				{
					lexem_putchar(lexem, symbol);
					state = STATE_DECIMAL_E;
				}
				else
				{
					ungetc(symbol, stdin);
					token->type = TYPE_INT;
					token->attribute.integer = strtol(lexem->word, NULL, 10);
					
					free_lexem(lexem);
					return;
				}
			break;

			case STATE_DECIMAL_DOT:// 123.
				if(isdigit(symbol))
				{
					lexem_putchar(lexem, symbol);
					state = STATE_DECIMAL;
				}
				else //123.x
				{
					free_lexem(lexem);
					free_all();
					exit(1);
				}
			break;

			case STATE_DECIMAL://123.123
				if(isdigit(symbol))
				{
					lexem_putchar(lexem, symbol);
				}
				else if(symbol == 'e' || symbol == 'E')// 123.123e
				{
					lexem_putchar(lexem, symbol);
					state = STATE_DECIMAL_E;
				}
				else
				{
					ungetc(symbol, stdin);
					token->type = TYPE_FLOAT;
					
					char *ptr;
					token->attribute.decimal = strtof(lexem->word, &ptr );
					
					free_lexem(lexem);
					return;
				}
				
					
			break;

			case STATE_DECIMAL_E: //123.123e
				if(isdigit(symbol))
				{
					lexem_putchar(lexem, symbol); //123.123e123
					state = STATE_DECIMAL_END;
				}
				else if(symbol == '+' || symbol == '-') //123.123e-123
				{
					lexem_putchar(lexem, symbol);
					state = STATE_DECIMAL_E_SIGN;
				}
				else //123.123ex
				{
					free_lexem(lexem);
					free_all();
					exit(1);
				}
			break;

			case STATE_DECIMAL_E_SIGN: //123.123e-123
				if(isdigit(symbol))
				{
					lexem_putchar(lexem, symbol);
					state = STATE_DECIMAL_END;
				}
				else //123.123e-
				{
					free_lexem(lexem);
					free_all();
					exit(1);
				}


			break;

			case STATE_DECIMAL_END:
				if(isdigit(symbol))
				{
					lexem_putchar(lexem, symbol);
				}
				else
				{
					ungetc(symbol, stdin);
					token->type = TYPE_FLOAT;
					
					token->attribute.decimal = strtof(lexem->word, NULL );
					
					free_lexem(lexem);
					return;
				}
			break;

			case STATE_STRING_LITERAL:// "text"
				if(symbol == '\"')
				{
					token->type = TYPE_STRING;
					token->attribute.string = lexem;
					return;
				}
				else if(symbol == '\\') //"text \x"
				{
					state = STATE_ESCAPE_SEQUENCE;
				}
				else if(symbol < 32)
				{
					free_lexem(lexem);
					free_all();
					exit(1);
				}
				else
				{
					lexem_putchar(lexem, symbol);
				}
			break;

			case STATE_ESCAPE_SEQUENCE: //"text \n \t \\ \" text \" \s "
				if(symbol == '\"')
				{
					lexem_putchar(lexem, symbol);
					state = STATE_STRING_LITERAL;
				}					
				else if(symbol == 'n')
				{
					symbol = '\n';
					lexem_putchar(lexem, symbol);
					state = STATE_STRING_LITERAL;
				}
				else if(symbol == 't')
				{
					symbol = '\t';
					lexem_putchar(lexem, symbol);
					state = STATE_STRING_LITERAL;
				}
				else if(symbol == 's')
				{
					symbol = ' ';
					lexem_putchar(lexem, symbol);
					state = STATE_STRING_LITERAL;
				}
				else if(symbol == '\\')
				{
					lexem_putchar(lexem, symbol);
					state = STATE_STRING_LITERAL;
				}	
				else if(symbol == 'x')
				{
					state = STATE_HEXADECIMAL;
				}
				else
				{
					free_lexem(lexem);
					free_all();
					exit(1);
				}
			break;

			case STATE_HEXADECIMAL:// "\x"
				//48 az 57 0-9, 97 az 102 a-f, 65 az 70 A-F
				if((symbol > 47 && symbol < 58) ||(symbol > 96 && symbol < 103)||(symbol > 64 && symbol < 71)) //"\x9"
				{
					hex_value[0] = symbol;
					state = STATE_HEXADECIMAL_SECOND;
				}
				else
				{
					free_lexem(lexem);
					free_all();
					exit(1);
				}

			break;

			case STATE_HEXADECIMAL_SECOND: // "\x9"
				//48 az 57 0-9, 97 az 102 a-f, 65 az 70 A-F
				if((symbol > 47 && symbol < 58) ||(symbol > 96 && symbol < 103)||(symbol > 64 && symbol < 71)) // "/x9a"
				{
					hex_value[1] = symbol;
					int value = (int) strtol(hex_value, NULL, 16);
					symbol = (char) value;
					lexem_putchar(lexem, symbol);
					state = STATE_STRING_LITERAL;
					return;
				}
				else
				{
					ungetc(symbol, stdin); // "\x9"
					int value = (int) strtol(hex_value, NULL, 16);
					symbol = (char) value;
					lexem_putchar(lexem, symbol);
					state = STATE_STRING_LITERAL;
					return;
				}
			break;

			case STATE_COMMENT: 
				if (symbol == EOF || symbol == '\n')
				{
					state = STATE_START;
					ungetc(symbol, stdin);
				}	
			break;
		}		
	}
}

