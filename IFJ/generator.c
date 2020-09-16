/*Implementace překladače imperativního jazyka IFJ18
Jakub Gajdošík xgajdo24
Daniel Bílý xbilyd01
Petr Marek xmarek69
Jan Fridrich xfridr07
*/
#include "generator.h"

//makra pro jednodussi pirdavani instrukci/kodu
#define CAT_INST(inst) \
	do{ \
		if (gen_to_main == 1)\
			lexem_putstr(code_main, (inst "\n"));\
		else if(gen_to_main == 0)\
			lexem_putstr(code_rest, (inst "\n"));\
		else\
			lexem_putstr(code_while, (inst "\n"));\
	}while(0)\


#define CAT_STR(code_to_add) \
	do{ \
		if (gen_to_main == 1)\
			lexem_putstr(code_main, (code_to_add));\
		else if(gen_to_main == 0)\
			lexem_putstr(code_rest, (code_to_add));\
		else\
			lexem_putstr(code_while, (code_to_add));\
	}while(0)\



#define CAT_NUM(num) \
	do{ \
		char buffer[310];	\
		sprintf(buffer, "%d", num);	\
		CAT_STR(buffer);\
	}while(0) \

#define CAT_FLOAT(num) \
	do{ \
		char buffer[310];	\
		sprintf(buffer, "%a", num);	\
		CAT_STR(buffer);\
	}while(0) \

#define CAT_ESC_SEQUENCE(symbol) \
	do{ \
		char buffer[310];	\
		sprintf(buffer, "%03d", symbol);	\
		CAT_STR(buffer);\
	}while(0) \

extern int uvnitr_funkce;
lexem_t *code_main;
lexem_t *code_rest;
lexem_t *code_while;

int gen_to_main;//zatim vse do main 
						//bude urcovat jestli generovat do mainu(jiny lexem) nebo do zbytku

int if_counter = 0;
int if_block_counter = 0;
int while_counter = 0;
int while_block_counter = 0;
int aritm_instr_counter = 0;

void gen_substr()
{
	CAT_INST("LABEL $substr");
	gen_frame_retvar();
	CAT_INST("MOVE LF@!retvar nil@nil");

	CAT_INST("DEFVAR LF@!length");
	//volani length
	CAT_INST("CREATEFRAME");
	//predani argumentu
	CAT_INST("DEFVAR TF@!0");
	CAT_INST("MOVE TF@!0 LF@!0");
	CAT_INST("CALL $length");
	CAT_INST("MOVE LF@!length TF@!retvar");
	
	CAT_INST("DEFVAR LF@!relation");
	// length < 0?
	CAT_INST("LT LF@!relation LF@!length int@0");
	CAT_INST("JUMPIFEQ $substr_end LF@!relation bool@true");
	//length == 0?
	CAT_INST("EQ LF@!relation LF@!length int@0");
	CAT_INST("JUMPIFEQ $substr_end LF@!relation bool@true");
	//length > 0

	// i<0?
	CAT_INST("LT LF@!relation LF@!1 int@0");
	CAT_INST("JUMPIFEQ $substr_end LF@!relation bool@true");
	// i>lengt?
	CAT_INST("GT LF@!relation LF@!1 LF@!length");
	CAT_INST("JUMPIFEQ $substr_end LF@!relation bool@true");

	CAT_INST("LT LF@!relation LF@!2 int@0");
	CAT_INST("JUMPIFEQ $substr_end LF@!relation bool@true");
	CAT_INST("MOVE LF@!retvar string@");

	CAT_INST("EQ LF@!relation LF@!2 int@0");
	CAT_INST("JUMPIFEQ $substr_end LF@!relation bool@true");
	// n > length(s) - i
	CAT_INST("DEFVAR LF@!n");
	CAT_INST("MOVE LF@!n LF@!length");
	CAT_INST("SUB LF@!n LF@!n LF@!1");
	
	CAT_INST("GT LF@!relation LF@!2 LF@!n");
	CAT_INST("JUMPIFEQ $n_is_max LF@!relation bool@true");
	CAT_INST("JUMP $find_substr");
	CAT_INST("LABEL $n_is_max");
	CAT_INST("MOVE LF@!2 LF@!n");//nastaveni na maximalni velikost n
	
	CAT_INST("LABEL $find_substr");
	CAT_INST("DEFVAR LF@!i");
	CAT_INST("MOVE LF@!i LF@!1");
	CAT_INST("DEFVAR LF@!symbol");
	CAT_INST("DEFVAR LF@!cond");
	CAT_INST("LABEL $substr_loop");
	
	CAT_INST("GETCHAR LF@!symbol LF@!0 LF@!i");
	CAT_INST("CONCAT LF@!retvar LF@!retvar LF@!symbol");
	CAT_INST("ADD LF@!i LF@!i int@1");
	CAT_INST("SUB LF@!2 LF@!2 int@1");//celkova delka stringu se zkrati, musime od n odecist 1
	CAT_INST("GT LF@!cond LF@!2 int@0");
	CAT_INST("JUMPIFEQ $substr_loop LF@!cond bool@true");
	CAT_INST("LABEL $substr_end");
	CAT_INST("POPFRAME");
	CAT_INST("RETURN");
	CAT_INST("");

}	


void gen_ord()
{
	CAT_INST("LABEL $ord");
	gen_frame_retvar();
	CAT_INST("MOVE LF@!retvar nil@nil");
	CAT_INST("DEFVAR LF@!relation");
	// i<0?
	CAT_INST("LT LF@!relation LF@!1 int@0");
	CAT_INST("JUMPIFEQ $ord_end LF@!relation bool@true");
	CAT_INST("DEFVAR LF@!length");
	//volani funkce length
	CAT_INST("CREATEFRAME");
	CAT_INST("DEFVAR TF@!0");
	CAT_INST("MOVE TF@!0 LF@!0");
	CAT_INST("CALL $length");
	CAT_INST("MOVE LF@!length TF@!retvar");
	CAT_INST("SUB LF@!length LF@!length int@1");
	// i>length?
	CAT_INST("GT LF@!relation LF@!1 LF@!length");
	CAT_INST("JUMPIFEQ ord_end LF@!relation bool@true");
	CAT_INST("STRI2INT LF@!retvar LF@!0 LF@!1");
	CAT_INST("LABEL $ord_end");
	gen_built_end();
	CAT_INST("");

}

void gen_chr()
{
	CAT_INST("LABEL $chr");
	gen_frame_retvar();
	CAT_INST("DEFVAR LF@!relation");
	CAT_INST("LT LF@!relation LF@!0 int@0");
	CAT_INST("JUMPIFEQ $chr_error LF@!relation bool@true");
	CAT_INST("GT LF@!relation LF@!0 int@255");
	CAT_INST("JUMPIFEQ $chr_error LF@!relation bool@true");
	CAT_INST("INT2CHAR LF@!retvar LF@!0");
	gen_built_end();
	CAT_INST("LABEL $chr_error");
	CAT_INST("CLEARS");
	CAT_INST("EXIT int@6");
	
	CAT_INST("");


}

void gen_length()
{
	CAT_INST("LABEL $length");
	gen_frame_retvar();
	CAT_INST("STRLEN LF@!retvar LF@!0");
	gen_built_end();
	CAT_INST("");

}

void gen_print()
{
	CAT_INST("LABEL $print");
	gen_frame_retvar();
	CAT_INST("WRITE GF@!result");
	CAT_INST("MOVE LF@!retvar nil@nil"); 
	gen_built_end();
	CAT_INST("");

}



void gen_inputi()
{
	CAT_INST("LABEL $inputi");
	gen_frame_retvar();
	CAT_INST("READ LF@!retvar int");
	gen_built_end();
	CAT_INST("");

}

void gen_inputf()
{
	CAT_INST("LABEL $inputf");
	gen_frame_retvar();
	CAT_INST("READ LF@!retvar float");
	gen_built_end();
	CAT_INST("");

}
void gen_inputs()
{
	CAT_INST("LABEL $inputs");
	gen_frame_retvar();
	CAT_INST("READ LF@!retvar string");
	gen_built_end();
	CAT_INST("");


}
void print_output()
{
	fputs(code_rest->word, stdout);
	fputs(code_main->word, stdout);	
}
void gen_zero_division()
{
	CAT_INST("LABEL $zero_division");
	CAT_INST("CLEARS");
	CAT_INST("EXIT int@9");
}
void gen_type_error()
{
	CAT_INST("LABEL $type_error");
	CAT_INST("CLEARS");
	CAT_INST("EXIT int@4");
}
void gen_builtins()
{
	CAT_INST("");
	gen_print();
	gen_substr();
	gen_chr();
	gen_ord();
	gen_length();
	gen_inputs();
	gen_inputi();
	gen_inputf();
	gen_zero_division();
	gen_type_error();

}
void switch_stack(){
	CAT_INST("POPS GF@!tmp1");
	CAT_INST("POPS GF@!tmp2");

	CAT_INST("PUSHS GF@!tmp1");
	CAT_INST("PUSHS GF@!tmp2");
}
void generator_init()
{
	code_rest = malloc(sizeof(lexem_t));
	if(code_rest == NULL)
	{
		free_all();
	}
	lexem_init(code_rest);	

	code_main = malloc(sizeof(lexem_t));
	if(code_main == NULL)
	{
		free_all();
	}
	lexem_init(code_main);	
	code_while = malloc(sizeof(lexem_t));
	if(code_while == NULL)
	{
		free_all();
	}
	lexem_init(code_while);	
	

	gen_header();
	gen_main_start();
	gen_to_main = 0;
	gen_builtins();
	
}

void gen_header()
{
	gen_to_main = 0;
	
	CAT_INST(".IFJcode18");
	
	CAT_INST("DEFVAR GF@!result");
	CAT_INST("DEFVAR GF@!tmp1");//pomocne promenne pro operace
	CAT_INST("DEFVAR GF@!tmp2");
	CAT_INST("DEFVAR GF@!tmp3");
	CAT_INST("DEFVAR GF@!tmp4");
	CAT_INST("DEFVAR GF@!tmp5");
	CAT_INST("DEFVAR GF@!tmp6");
	CAT_INST("DEFVAR GF@!tmp7");
	CAT_INST("DEFVAR GF@!tmp8");
	CAT_INST("DEFVAR GF@!tmp9");

	CAT_INST("JUMP $$main");
	
}

void gen_main_start()
{
	gen_to_main = 1;
	CAT_INST(""); //prazdny radek
	CAT_INST("# main:");
	CAT_INST("LABEL $$main");
	CAT_INST("CREATEFRAME");
	CAT_INST("PUSHFRAME");
	CAT_INST("");
}
//mezi start a end se budou generovat ruzne dalsi instrukce
void gen_main_end()
{
	gen_to_main = 1;
	CAT_INST("");
	CAT_INST("POPFRAME");// je potreba?
	CAT_INST("CLEARS");
	CAT_INST("EXIT int@0");//  je potreba?
	CAT_INST("");
}

void gen_val_from_token(token_t token)
{
	if(token.type == TYPE_IDENTIFIER)
	{
		CAT_STR("LF@!");
		CAT_STR(token.attribute.string->word);
	}
	else if(token.type == TYPE_INT)
	{
		CAT_STR("int@");
		CAT_NUM(token.attribute.integer);
	}
	else if(token.type == TYPE_FLOAT)
	{
		CAT_STR("float@");
		CAT_FLOAT(token.attribute.decimal);
	}
	else if(token.type == NIL)
	{
		CAT_STR("nil@nil");
	}
	else if(token.type == TYPE_STRING)
	{
		CAT_STR("string@");
		int i = 0;
		char symbol = token.attribute.string->word[i]; 
		while(symbol != '\0')
		{
			if (symbol == '#' || symbol == '\\' || symbol < 33 || (!isprint(symbol)))
			{
				CAT_STR("\\");
				CAT_ESC_SEQUENCE(token.attribute.string->word[i]);
			}
			else
			{
				if (gen_to_main == 1)
					lexem_putchar(code_main, (symbol));
				else if(gen_to_main == 0)
					lexem_putchar(code_rest, (symbol));
				else
					lexem_putchar(code_while, (symbol));
			}

			i++;
			symbol = token.attribute.string->word[i];
		}
	}
	else
	{
		//error

	}
}


void gen_var(char *v_name)
{
	CAT_STR("DEFVAR LF@!");
	CAT_STR(v_name);
	CAT_INST("");

	CAT_STR("MOVE LF@!");
	CAT_STR(v_name);
	CAT_STR(" nil@nil");
	CAT_INST("");
}
void push_false()
{
	CAT_INST("PUSHS bool@false");
}
void push_true()
{
	CAT_INST("PUSHS bool@true");
}


void gen_def_start(char *f_name)
{
	gen_to_main = 0;
	CAT_INST("");
	CAT_STR("# zacatek funkce: "); 
	CAT_STR(f_name); 
	CAT_INST("");

	CAT_STR("LABEL $"); 
	CAT_STR(f_name);
	CAT_INST("");
	CAT_INST("PUSHFRAME");
	CAT_INST("DEFVAR LF@!retvar");
	uvnitr_funkce=1;


}

void gen_built_end(){
	CAT_INST("POPFRAME");
	CAT_INST("RETURN");
}

void gen_def_end()
{
	CAT_INST("MOVE LF@!retvar GF@!result");
	CAT_INST("POPFRAME");
	CAT_INST("RETURN");
	gen_to_main = 1;
	uvnitr_funkce=0;
}
void create_frame()//musi se vyrvorit frame pro parametry, pro lepsi prehlednost pri volani z parseru
{
	CAT_INST("CREATEFRAME");
}

void gen_frame_retvar()
{
	CAT_INST("PUSHFRAME");
	CAT_INST("DEFVAR LF@!retvar");// MUSI BYT NA LF PROTOZE PO KONCI FUNKCI SE DELA POPFRAME,
								// VEDLO BY KE ZTRATE HODNOTY
}

void gen_call(char *fname)
{
	CAT_STR("CALL $");
	CAT_STR(fname);
	CAT_INST("");
}

void gen_assign_from_call(char *v_name)
{
	CAT_STR("MOVE LF@!");
	CAT_STR(v_name);
	CAT_INST(" TF@!retvar");
}

void gen_argument(int order)
{
	CAT_STR("DEFVAR TF@!");
	CAT_NUM(order);
	CAT_INST("");

	CAT_STR("MOVE TF@!");
	CAT_NUM(order);
	CAT_STR(" GF@!result");
	CAT_INST("");	
}

void gen_parametr(char *p_name, int order)
{
	CAT_STR("DEFVAR LF@!");
	CAT_STR(p_name);
	CAT_INST("");

	CAT_STR("MOVE LF@!");
	CAT_STR(p_name);
	CAT_STR(" LF@!");
	CAT_NUM(order);
	CAT_INST("");
}



void gen_stack_push(token_t token)
{
	CAT_STR("PUSHS ");
	gen_val_from_token(token);
	CAT_INST("");
}

void gen_stack_pop(char *frame, char *v_name)
{
	CAT_STR("POPS ");
	CAT_STR(frame);
	CAT_STR("@!");
	CAT_STR(v_name);
	CAT_INST("");

}

void gen_stack_add()
{	
	//skopirovani hodnot
	CAT_INST("POPS GF@!tmp1");
	CAT_INST("POPS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp1");
	//zjisteni typu
	CAT_INST("TYPE GF@!tmp4 GF@!tmp1");
	CAT_INST("TYPE GF@!tmp5 GF@!tmp2");
	CAT_INST("TYPE GF@!tmp6 GF@!tmp1");
	CAT_INST("TYPE GF@!tmp7 GF@!tmp2");
	CAT_INST("TYPE GF@!tmp8 GF@!tmp1");
	CAT_INST("TYPE GF@!tmp9 GF@!tmp2");

	CAT_INST("EQ GF@!tmp3 GF@!tmp4 GF@!tmp5");


	CAT_STR("JUMPIFEQ $opr");
	CAT_NUM(aritm_instr_counter);
	CAT_INST(" GF@!tmp3 bool@true");
	//float int
	CAT_INST("EQ GF@!tmp4 GF@!tmp4 string@float");
	CAT_INST("EQ GF@!tmp5 GF@!tmp5 string@int");
	CAT_INST("AND GF@!tmp3 GF@!tmp4 GF@!tmp5");
	//int float
	CAT_INST("EQ GF@!tmp6 GF@!tmp6 string@int");
	CAT_INST("EQ GF@!tmp7 GF@!tmp7 string@float");
	CAT_INST("AND GF@!tmp4 GF@!tmp6 GF@!tmp7");
	CAT_INST("OR GF@!tmp3 GF@!tmp3 GF@!tmp4");



	CAT_INST("JUMPIFEQ $type_error GF@!tmp3 bool@false");


	CAT_INST("EQ GF@!tmp3 GF@!tmp4 bool@true");
	CAT_STR("JUMPIFEQ $pretyp_1");
	CAT_NUM(aritm_instr_counter);
	CAT_STR(" GF@!tmp3 bool@true");
	CAT_INST("");
	//pretyp2
	CAT_INST("INT2FLOAT GF@!tmp2 GF@!tmp2");
	CAT_STR("JUMP $opr");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");

	//pretyp1
	CAT_STR("LABEL $pretyp_1");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");
	CAT_INST("INT2FLOAT GF@!tmp1 GF@!tmp1");
		

	//jsou stejne, zjistuji operaci(add, cat)
	CAT_STR("LABEL $opr");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");

	CAT_STR("JUMPIFEQ $type_error");
	CAT_INST(" GF@!tmp8 string@bool");

	CAT_STR("JUMPIFEQ $type_error");
	CAT_INST(" GF@!tmp8 string@nil");

	CAT_STR("JUMPIFEQ $concat");
	CAT_NUM(aritm_instr_counter);
	CAT_INST(" GF@!tmp8 string@string");



	CAT_INST("POPS GF@!tmp3");
	CAT_INST("POPS GF@!tmp4");
	CAT_INST("PUSHS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp1");



	CAT_INST("ADDS");
	

	CAT_STR("JUMP $add_end");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");
	//concat
	CAT_STR("LABEL $concat");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");

	CAT_INST("POPS GF@!tmp1");
	CAT_INST("POPS GF@!tmp2");

	CAT_INST("CONCAT GF@!tmp3 GF@!tmp2 GF@!tmp1");
	CAT_INST("PUSHS GF@!tmp3");

	//end
	CAT_STR("LABEL $add_end");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");
	aritm_instr_counter++;
}

void gen_stack_sub()
{
	//skopirovani hodnot
	CAT_INST("POPS GF@!tmp1");
	CAT_INST("POPS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp1");
	//zjisteni typu
	CAT_INST("TYPE GF@!tmp4 GF@!tmp1");
	CAT_INST("TYPE GF@!tmp5 GF@!tmp2");
	CAT_INST("TYPE GF@!tmp6 GF@!tmp1");
	CAT_INST("TYPE GF@!tmp7 GF@!tmp2");
	CAT_INST("TYPE GF@!tmp8 GF@!tmp1");
	CAT_INST("TYPE GF@!tmp9 GF@!tmp2");

	CAT_INST("EQ GF@!tmp3 GF@!tmp4 GF@!tmp5");


	CAT_STR("JUMPIFEQ $opr");
	CAT_NUM(aritm_instr_counter);
	CAT_INST(" GF@!tmp3 bool@true");
	//float int
	CAT_INST("EQ GF@!tmp4 GF@!tmp4 string@float");
	CAT_INST("EQ GF@!tmp5 GF@!tmp5 string@int");
	CAT_INST("AND GF@!tmp3 GF@!tmp4 GF@!tmp5");
	//int float
	CAT_INST("EQ GF@!tmp6 GF@!tmp6 string@int");
	CAT_INST("EQ GF@!tmp7 GF@!tmp7 string@float");
	CAT_INST("AND GF@!tmp4 GF@!tmp6 GF@!tmp7");
	CAT_INST("OR GF@!tmp3 GF@!tmp3 GF@!tmp4");



	CAT_INST("JUMPIFEQ $type_error GF@!tmp3 bool@false");


	CAT_INST("EQ GF@!tmp3 GF@!tmp4 bool@true");
	CAT_STR("JUMPIFEQ $pretyp_1");
	CAT_NUM(aritm_instr_counter);
	CAT_STR(" GF@!tmp3 bool@true");
	CAT_INST("");
	//pretyp2
	CAT_INST("INT2FLOAT GF@!tmp2 GF@!tmp2");
	CAT_STR("JUMP $opr");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");

	//pretyp1
	CAT_STR("LABEL $pretyp_1");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");
	CAT_INST("INT2FLOAT GF@!tmp1 GF@!tmp1");
		

	//jsou stejne, zjistuji operaci(add, cat)
	CAT_STR("LABEL $opr");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");
	
	CAT_STR("JUMPIFEQ $type_error");
	CAT_INST(" GF@!tmp8 string@bool");

	CAT_STR("JUMPIFEQ $type_error");
	CAT_INST(" GF@!tmp8 string@nil");

	CAT_STR("JUMPIFEQ $type_error");
	CAT_INST(" GF@!tmp8 string@string");


	CAT_INST("POPS GF@!tmp3");
	CAT_INST("POPS GF@!tmp4");
	CAT_INST("PUSHS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp1");



	CAT_INST("SUBS");
	
	
	aritm_instr_counter++;
}

void gen_stack_mul()
{
	//skopirovani hodnot
	CAT_INST("POPS GF@!tmp1");
	CAT_INST("POPS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp1");
	//zjisteni typu
	CAT_INST("TYPE GF@!tmp4 GF@!tmp1");
	CAT_INST("TYPE GF@!tmp5 GF@!tmp2");
	CAT_INST("TYPE GF@!tmp6 GF@!tmp1");
	CAT_INST("TYPE GF@!tmp7 GF@!tmp2");
	CAT_INST("TYPE GF@!tmp8 GF@!tmp1");
	CAT_INST("TYPE GF@!tmp9 GF@!tmp2");

	CAT_INST("EQ GF@!tmp3 GF@!tmp4 GF@!tmp5");


	CAT_STR("JUMPIFEQ $opr");
	CAT_NUM(aritm_instr_counter);
	CAT_INST(" GF@!tmp3 bool@true");
	//float int
	CAT_INST("EQ GF@!tmp4 GF@!tmp4 string@float");
	CAT_INST("EQ GF@!tmp5 GF@!tmp5 string@int");
	CAT_INST("AND GF@!tmp3 GF@!tmp4 GF@!tmp5");
	//int float
	CAT_INST("EQ GF@!tmp6 GF@!tmp6 string@int");
	CAT_INST("EQ GF@!tmp7 GF@!tmp7 string@float");
	CAT_INST("AND GF@!tmp4 GF@!tmp6 GF@!tmp7");
	CAT_INST("OR GF@!tmp3 GF@!tmp3 GF@!tmp4");



	CAT_INST("JUMPIFEQ $type_error GF@!tmp3 bool@false");


	CAT_INST("EQ GF@!tmp3 GF@!tmp4 bool@true");
	CAT_STR("JUMPIFEQ $pretyp_1");
	CAT_NUM(aritm_instr_counter);
	CAT_STR(" GF@!tmp3 bool@true");
	CAT_INST("");
	//pretyp2
	CAT_INST("INT2FLOAT GF@!tmp2 GF@!tmp2");
	CAT_STR("JUMP $opr");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");

	//pretyp1
	CAT_STR("LABEL $pretyp_1");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");
	CAT_INST("INT2FLOAT GF@!tmp1 GF@!tmp1");
		

	//jsou stejne, zjistuji operaci(add, cat)
	CAT_STR("LABEL $opr");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");
	
	CAT_STR("JUMPIFEQ $type_error");
	CAT_INST(" GF@!tmp8 string@bool");

	CAT_STR("JUMPIFEQ $type_error");
	CAT_INST(" GF@!tmp8 string@nil");

	CAT_STR("JUMPIFEQ $type_error");
	CAT_INST(" GF@!tmp8 string@string");


	CAT_INST("POPS GF@!tmp3");
	CAT_INST("POPS GF@!tmp4");
	CAT_INST("PUSHS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp1");



	CAT_INST("MULS");
	
	
	aritm_instr_counter++;
}

void gen_stack_div()
{
	//skopirovani hodnot
	CAT_INST("POPS GF@!tmp1");
	CAT_INST("POPS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp1");
	//zjisteni typu
	CAT_INST("TYPE GF@!tmp4 GF@!tmp1");
	CAT_INST("TYPE GF@!tmp5 GF@!tmp2");
	CAT_INST("TYPE GF@!tmp6 GF@!tmp1");
	CAT_INST("TYPE GF@!tmp7 GF@!tmp2");
	CAT_INST("TYPE GF@!tmp8 GF@!tmp1");
	CAT_INST("TYPE GF@!tmp9 GF@!tmp2");

	CAT_INST("EQ GF@!tmp3 GF@!tmp4 GF@!tmp5");


	CAT_STR("JUMPIFEQ $opr");
	CAT_NUM(aritm_instr_counter);
	CAT_INST(" GF@!tmp3 bool@true");
	//float int
	CAT_INST("EQ GF@!tmp4 GF@!tmp4 string@float");
	CAT_INST("EQ GF@!tmp5 GF@!tmp5 string@int");
	CAT_INST("AND GF@!tmp3 GF@!tmp4 GF@!tmp5");
	//int float
	CAT_INST("EQ GF@!tmp6 GF@!tmp6 string@int");
	CAT_INST("EQ GF@!tmp7 GF@!tmp7 string@float");
	CAT_INST("AND GF@!tmp4 GF@!tmp6 GF@!tmp7");
	CAT_INST("OR GF@!tmp3 GF@!tmp3 GF@!tmp4");



	CAT_INST("JUMPIFEQ $type_error GF@!tmp3 bool@false");


	CAT_INST("EQ GF@!tmp3 GF@!tmp4 bool@true");
	CAT_STR("JUMPIFEQ $pretyp_1");
	CAT_NUM(aritm_instr_counter);
	CAT_STR(" GF@!tmp3 bool@true");
	CAT_INST("");
	//pretyp2
	CAT_INST("INT2FLOAT GF@!tmp2 GF@!tmp2");
	CAT_STR("JUMP $opr");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");

	//pretyp1
	CAT_STR("LABEL $pretyp_1");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");
	CAT_INST("INT2FLOAT GF@!tmp1 GF@!tmp1");
		

	//jsou stejne, zjistuji operaci(add, cat)
	CAT_STR("LABEL $opr");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");

	
	CAT_STR("JUMPIFEQ $type_error");
	CAT_INST(" GF@!tmp8 string@bool");

	CAT_STR("JUMPIFEQ $type_error");
	CAT_INST(" GF@!tmp8 string@nil");

	CAT_STR("JUMPIFEQ $type_error");
	CAT_INST(" GF@!tmp8 string@string");


	CAT_INST("POPS GF@!tmp3");
	CAT_INST("POPS GF@!tmp4");
	CAT_INST("PUSHS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp1");



	//deleni nulou
	CAT_INST("POPS GF@!tmp1");
	CAT_INST("PUSHS GF@!tmp1");
	CAT_INST("MOVE GF@!tmp2 float@0x0.0p+0");
	CAT_INST("EQ GF@!tmp3 GF@!tmp1 GF@!tmp2");
	CAT_INST("JUMPIFEQ $zero_division GF@!tmp3 bool@true");



	CAT_INST("DIVS");
	
	aritm_instr_counter++;
}

void gen_stack_idiv()
{
	//skopirovani hodnot
	CAT_INST("POPS GF@!tmp1");
	CAT_INST("POPS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp1");
	//zjisteni typu
	CAT_INST("TYPE GF@!tmp4 GF@!tmp1");
	CAT_INST("TYPE GF@!tmp5 GF@!tmp2");
	CAT_INST("TYPE GF@!tmp6 GF@!tmp1");
	CAT_INST("TYPE GF@!tmp7 GF@!tmp2");
	CAT_INST("TYPE GF@!tmp8 GF@!tmp1");
	CAT_INST("TYPE GF@!tmp9 GF@!tmp2");

	CAT_INST("EQ GF@!tmp3 GF@!tmp4 GF@!tmp5");


	CAT_STR("JUMPIFEQ $opr");
	CAT_NUM(aritm_instr_counter);
	CAT_INST(" GF@!tmp3 bool@true");
	//float int
	CAT_INST("EQ GF@!tmp4 GF@!tmp4 string@float");
	CAT_INST("EQ GF@!tmp5 GF@!tmp5 string@int");
	CAT_INST("AND GF@!tmp3 GF@!tmp4 GF@!tmp5");
	//int float
	CAT_INST("EQ GF@!tmp6 GF@!tmp6 string@int");
	CAT_INST("EQ GF@!tmp7 GF@!tmp7 string@float");
	CAT_INST("AND GF@!tmp4 GF@!tmp6 GF@!tmp7");
	CAT_INST("OR GF@!tmp3 GF@!tmp3 GF@!tmp4");



	CAT_INST("JUMPIFEQ $type_error GF@!tmp3 bool@false");


	CAT_INST("EQ GF@!tmp3 GF@!tmp4 bool@true");
	CAT_STR("JUMPIFEQ $pretyp_1");
	CAT_NUM(aritm_instr_counter);
	CAT_STR(" GF@!tmp3 bool@true");
	CAT_INST("");
	//pretyp2
	CAT_INST("INT2FLOAT GF@!tmp2 GF@!tmp2");
	CAT_STR("JUMP $podel");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");

	//pretyp1
	CAT_STR("LABEL $pretyp_1");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");
	CAT_INST("INT2FLOAT GF@!tmp1 GF@!tmp1");
	CAT_STR("JUMP $podel");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");	

	//jsou stejne, zjistuji operaci(add, cat)
	CAT_STR("LABEL $opr");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");
	
	CAT_STR("JUMPIFEQ $podel");
	CAT_NUM(aritm_instr_counter);
	CAT_INST(" GF@!tmp8 string@float");

	CAT_STR("JUMPIFEQ $type_error");
	CAT_INST(" GF@!tmp8 string@bool");

	CAT_STR("JUMPIFEQ $type_error");
	CAT_INST(" GF@!tmp8 string@nil");

	CAT_STR("JUMPIFEQ $type_error");
	CAT_INST(" GF@!tmp8 string@string");


	CAT_INST("POPS GF@!tmp3");
	CAT_INST("POPS GF@!tmp4");
	CAT_INST("PUSHS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp1");


	//deleni nulou
	CAT_INST("POPS GF@!tmp1");
	CAT_INST("PUSHS GF@!tmp1");
	CAT_INST("MOVE GF@!tmp2 int@0");
	CAT_INST("EQ GF@!tmp3 GF@!tmp1 GF@!tmp2");
	CAT_INST("JUMPIFEQ $zero_division GF@!tmp3 bool@true");



	CAT_INST("IDIVS");
	
	CAT_STR("JUMP $add_end");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");
	//concat
	CAT_STR("LABEL $podel");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");
	CAT_INST("POPS GF@!tmp3");
	CAT_INST("POPS GF@!tmp4");
	CAT_INST("PUSHS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp1");

	//deleni nulou
	CAT_INST("POPS GF@!tmp1");
	CAT_INST("PUSHS GF@!tmp1");
	CAT_INST("MOVE GF@!tmp2 float@0x0.0p+0");
	CAT_INST("EQ GF@!tmp3 GF@!tmp1 GF@!tmp2");
	CAT_INST("JUMPIFEQ $zero_division GF@!tmp3 bool@true");

	CAT_INST("DIVS");

	//end
	CAT_STR("LABEL $add_end");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");
	aritm_instr_counter++;

}

void gen_stack_eq()
{
	//skopirovani hodnot
	CAT_INST("POPS GF@!tmp1");
	CAT_INST("POPS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp1");
	//zjisteni typu
	CAT_INST("TYPE GF@!tmp4 GF@!tmp1");
	CAT_INST("TYPE GF@!tmp5 GF@!tmp2");
	CAT_INST("TYPE GF@!tmp6 GF@!tmp1");
	CAT_INST("TYPE GF@!tmp7 GF@!tmp2");
	CAT_INST("TYPE GF@!tmp8 GF@!tmp1");
	CAT_INST("TYPE GF@!tmp9 GF@!tmp2");

	CAT_INST("EQ GF@!tmp3 GF@!tmp4 GF@!tmp5");


	CAT_STR("JUMPIFEQ $opr");
	CAT_NUM(aritm_instr_counter);
	CAT_INST(" GF@!tmp3 bool@true");
	//float int
	CAT_INST("EQ GF@!tmp4 GF@!tmp4 string@float");
	CAT_INST("EQ GF@!tmp5 GF@!tmp5 string@int");
	CAT_INST("AND GF@!tmp3 GF@!tmp4 GF@!tmp5");
	//int float
	CAT_INST("EQ GF@!tmp6 GF@!tmp6 string@int");
	CAT_INST("EQ GF@!tmp7 GF@!tmp7 string@float");
	CAT_INST("AND GF@!tmp4 GF@!tmp6 GF@!tmp7");
	CAT_INST("OR GF@!tmp3 GF@!tmp3 GF@!tmp4");



	CAT_INST("JUMPIFEQ $type_error GF@!tmp3 bool@false");


	CAT_INST("EQ GF@!tmp3 GF@!tmp4 bool@true");
	CAT_STR("JUMPIFEQ $pretyp_1");
	CAT_NUM(aritm_instr_counter);
	CAT_STR(" GF@!tmp3 bool@true");
	CAT_INST("");
	//pretyp2
	CAT_INST("INT2FLOAT GF@!tmp2 GF@!tmp2");
	CAT_STR("JUMP $opr");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");

	//pretyp1
	CAT_STR("LABEL $pretyp_1");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");
	CAT_INST("INT2FLOAT GF@!tmp1 GF@!tmp1");
		

	//jsou stejne, zjistuji operaci(add, cat)
	CAT_STR("LABEL $opr");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");

	


	CAT_INST("POPS GF@!tmp3");
	CAT_INST("POPS GF@!tmp4");
	CAT_INST("PUSHS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp1");



	CAT_INST("EQS");
	

	aritm_instr_counter++;
}

void gen_stack_less_than()
{
	CAT_INST("POPS GF@!tmp1");
	CAT_INST("POPS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp1");
	//zjisteni typu
	CAT_INST("TYPE GF@!tmp4 GF@!tmp1");
	CAT_INST("TYPE GF@!tmp5 GF@!tmp2");
	CAT_INST("TYPE GF@!tmp6 GF@!tmp1");
	CAT_INST("TYPE GF@!tmp7 GF@!tmp2");
	CAT_INST("TYPE GF@!tmp8 GF@!tmp1");
	CAT_INST("TYPE GF@!tmp9 GF@!tmp2");

	CAT_INST("EQ GF@!tmp3 GF@!tmp4 GF@!tmp5");


	CAT_STR("JUMPIFEQ $opr");
	CAT_NUM(aritm_instr_counter);
	CAT_INST(" GF@!tmp3 bool@true");
	//float int
	CAT_INST("EQ GF@!tmp4 GF@!tmp4 string@float");
	CAT_INST("EQ GF@!tmp5 GF@!tmp5 string@int");
	CAT_INST("AND GF@!tmp3 GF@!tmp4 GF@!tmp5");
	//int float
	CAT_INST("EQ GF@!tmp6 GF@!tmp6 string@int");
	CAT_INST("EQ GF@!tmp7 GF@!tmp7 string@float");
	CAT_INST("AND GF@!tmp4 GF@!tmp6 GF@!tmp7");
	CAT_INST("OR GF@!tmp3 GF@!tmp3 GF@!tmp4");



	CAT_INST("JUMPIFEQ $type_error GF@!tmp3 bool@false");


	CAT_INST("EQ GF@!tmp3 GF@!tmp4 bool@true");
	CAT_STR("JUMPIFEQ $pretyp_1");
	CAT_NUM(aritm_instr_counter);
	CAT_STR(" GF@!tmp3 bool@true");
	CAT_INST("");
	//pretyp2
	CAT_INST("INT2FLOAT GF@!tmp2 GF@!tmp2");
	CAT_STR("JUMP $opr");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");

	//pretyp1
	CAT_STR("LABEL $pretyp_1");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");
	CAT_INST("INT2FLOAT GF@!tmp1 GF@!tmp1");
		

	//jsou stejne, zjistuji operaci(add, cat)
	CAT_STR("LABEL $opr");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");

	CAT_STR("JUMPIFEQ $type_error");
	CAT_INST(" GF@!tmp8 string@bool");

	CAT_STR("JUMPIFEQ $type_error");
	CAT_INST(" GF@!tmp8 string@nil");

	

	CAT_INST("POPS GF@!tmp3");
	CAT_INST("POPS GF@!tmp4");
	CAT_INST("PUSHS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp1");



	CAT_INST("LTS");

	
	aritm_instr_counter++;
}

void gen_less_or_equal()
{
	//skopirovani hodnot
	CAT_INST("POPS GF@!tmp1");
	CAT_INST("POPS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp1");
	//zjisteni typu
	CAT_INST("TYPE GF@!tmp4 GF@!tmp1");
	CAT_INST("TYPE GF@!tmp5 GF@!tmp2");
	CAT_INST("TYPE GF@!tmp6 GF@!tmp1");
	CAT_INST("TYPE GF@!tmp7 GF@!tmp2");
	CAT_INST("TYPE GF@!tmp8 GF@!tmp1");
	CAT_INST("TYPE GF@!tmp9 GF@!tmp2");

	CAT_INST("EQ GF@!tmp3 GF@!tmp4 GF@!tmp5");


	CAT_STR("JUMPIFEQ $opr");
	CAT_NUM(aritm_instr_counter);
	CAT_INST(" GF@!tmp3 bool@true");
	//float int
	CAT_INST("EQ GF@!tmp4 GF@!tmp4 string@float");
	CAT_INST("EQ GF@!tmp5 GF@!tmp5 string@int");
	CAT_INST("AND GF@!tmp3 GF@!tmp4 GF@!tmp5");
	//int float
	CAT_INST("EQ GF@!tmp6 GF@!tmp6 string@int");
	CAT_INST("EQ GF@!tmp7 GF@!tmp7 string@float");
	CAT_INST("AND GF@!tmp4 GF@!tmp6 GF@!tmp7");
	CAT_INST("OR GF@!tmp3 GF@!tmp3 GF@!tmp4");



	CAT_INST("JUMPIFEQ $type_error GF@!tmp3 bool@false");


	CAT_INST("EQ GF@!tmp3 GF@!tmp4 bool@true");
	CAT_STR("JUMPIFEQ $pretyp_1");
	CAT_NUM(aritm_instr_counter);
	CAT_STR(" GF@!tmp3 bool@true");
	CAT_INST("");
	//pretyp2
	CAT_INST("INT2FLOAT GF@!tmp2 GF@!tmp2");
	CAT_STR("JUMP $opr");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");

	//pretyp1
	CAT_STR("LABEL $pretyp_1");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");
	CAT_INST("INT2FLOAT GF@!tmp1 GF@!tmp1");
		

	//jsou stejne, zjistuji operaci(add, cat)
	CAT_STR("LABEL $opr");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");

	CAT_STR("JUMPIFEQ $type_error");
	CAT_INST(" GF@!tmp8 string@bool");

	CAT_STR("JUMPIFEQ $type_error");
	CAT_INST(" GF@!tmp8 string@nil");

	

	CAT_INST("POPS GF@!tmp3");
	CAT_INST("POPS GF@!tmp4");
	CAT_INST("PUSHS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp1");


	CAT_INST("POPS GF@!tmp1");
	CAT_INST("POPS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp1");
	CAT_INST("LTS");
	CAT_INST("PUSHS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp1");
	CAT_INST("EQS");
	CAT_INST("ORS");
	aritm_instr_counter++;

}
void gen_more_or_equal()
{
	//skopirovani hodnot
	CAT_INST("POPS GF@!tmp1");
	CAT_INST("POPS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp1");
	//zjisteni typu
	CAT_INST("TYPE GF@!tmp4 GF@!tmp1");
	CAT_INST("TYPE GF@!tmp5 GF@!tmp2");
	CAT_INST("TYPE GF@!tmp6 GF@!tmp1");
	CAT_INST("TYPE GF@!tmp7 GF@!tmp2");
	CAT_INST("TYPE GF@!tmp8 GF@!tmp1");
	CAT_INST("TYPE GF@!tmp9 GF@!tmp2");

	CAT_INST("EQ GF@!tmp3 GF@!tmp4 GF@!tmp5");


	CAT_STR("JUMPIFEQ $opr");
	CAT_NUM(aritm_instr_counter);
	CAT_INST(" GF@!tmp3 bool@true");
	//float int
	CAT_INST("EQ GF@!tmp4 GF@!tmp4 string@float");
	CAT_INST("EQ GF@!tmp5 GF@!tmp5 string@int");
	CAT_INST("AND GF@!tmp3 GF@!tmp4 GF@!tmp5");
	//int float
	CAT_INST("EQ GF@!tmp6 GF@!tmp6 string@int");
	CAT_INST("EQ GF@!tmp7 GF@!tmp7 string@float");
	CAT_INST("AND GF@!tmp4 GF@!tmp6 GF@!tmp7");
	CAT_INST("OR GF@!tmp3 GF@!tmp3 GF@!tmp4");



	CAT_INST("JUMPIFEQ $type_error GF@!tmp3 bool@false");


	CAT_INST("EQ GF@!tmp3 GF@!tmp4 bool@true");
	CAT_STR("JUMPIFEQ $pretyp_1");
	CAT_NUM(aritm_instr_counter);
	CAT_STR(" GF@!tmp3 bool@true");
	CAT_INST("");
	//pretyp2
	CAT_INST("INT2FLOAT GF@!tmp2 GF@!tmp2");
	CAT_STR("JUMP $opr");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");

	//pretyp1
	CAT_STR("LABEL $pretyp_1");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");
	CAT_INST("INT2FLOAT GF@!tmp1 GF@!tmp1");
		

	//jsou stejne, zjistuji operaci(add, cat)
	CAT_STR("LABEL $opr");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");

	CAT_STR("JUMPIFEQ $type_error");
	CAT_INST(" GF@!tmp8 string@bool");

	CAT_STR("JUMPIFEQ $type_error");
	CAT_INST(" GF@!tmp8 string@nil");

	

	CAT_INST("POPS GF@!tmp3");
	CAT_INST("POPS GF@!tmp4");
	CAT_INST("PUSHS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp1");



	CAT_INST("POPS GF@!tmp1");
	CAT_INST("POPS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp1");
	CAT_INST("GTS");
	CAT_INST("PUSHS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp1");
	CAT_INST("EQS");
	CAT_INST("ORS");
	aritm_instr_counter++;
			
}
void gen_not_equal(){
	//skopirovani hodnot
	CAT_INST("POPS GF@!tmp1");
	CAT_INST("POPS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp1");
	//zjisteni typu
	CAT_INST("TYPE GF@!tmp4 GF@!tmp1");
	CAT_INST("TYPE GF@!tmp5 GF@!tmp2");
	CAT_INST("TYPE GF@!tmp6 GF@!tmp1");
	CAT_INST("TYPE GF@!tmp7 GF@!tmp2");
	CAT_INST("TYPE GF@!tmp8 GF@!tmp1");
	CAT_INST("TYPE GF@!tmp9 GF@!tmp2");

	CAT_INST("EQ GF@!tmp3 GF@!tmp4 GF@!tmp5");


	CAT_STR("JUMPIFEQ $opr");
	CAT_NUM(aritm_instr_counter);
	CAT_INST(" GF@!tmp3 bool@true");
	//float int
	CAT_INST("EQ GF@!tmp4 GF@!tmp4 string@float");
	CAT_INST("EQ GF@!tmp5 GF@!tmp5 string@int");
	CAT_INST("AND GF@!tmp3 GF@!tmp4 GF@!tmp5");
	//int float
	CAT_INST("EQ GF@!tmp6 GF@!tmp6 string@int");
	CAT_INST("EQ GF@!tmp7 GF@!tmp7 string@float");
	CAT_INST("AND GF@!tmp4 GF@!tmp6 GF@!tmp7");
	CAT_INST("OR GF@!tmp3 GF@!tmp3 GF@!tmp4");



	CAT_INST("JUMPIFEQ $type_error GF@!tmp3 bool@false");


	CAT_INST("EQ GF@!tmp3 GF@!tmp4 bool@true");
	CAT_STR("JUMPIFEQ $pretyp_1");
	CAT_NUM(aritm_instr_counter);
	CAT_STR(" GF@!tmp3 bool@true");
	CAT_INST("");
	//pretyp2
	CAT_INST("INT2FLOAT GF@!tmp2 GF@!tmp2");
	CAT_STR("JUMP $opr");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");

	//pretyp1
	CAT_STR("LABEL $pretyp_1");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");
	CAT_INST("INT2FLOAT GF@!tmp1 GF@!tmp1");
		

	//jsou stejne, zjistuji operaci(add, cat)
	CAT_STR("LABEL $opr");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");

	


	CAT_INST("POPS GF@!tmp3");
	CAT_INST("POPS GF@!tmp4");
	CAT_INST("PUSHS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp1");

	CAT_INST("EQS");
	CAT_INST("NOTS");
	aritm_instr_counter++;
}



void gen_stack_more_than()
{
	CAT_INST("POPS GF@!tmp1");
	CAT_INST("POPS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp1");
	//zjisteni typu
	CAT_INST("TYPE GF@!tmp4 GF@!tmp1");
	CAT_INST("TYPE GF@!tmp5 GF@!tmp2");
	CAT_INST("TYPE GF@!tmp6 GF@!tmp1");
	CAT_INST("TYPE GF@!tmp7 GF@!tmp2");
	CAT_INST("TYPE GF@!tmp8 GF@!tmp1");
	CAT_INST("TYPE GF@!tmp9 GF@!tmp2");

	CAT_INST("EQ GF@!tmp3 GF@!tmp4 GF@!tmp5");


	CAT_STR("JUMPIFEQ $opr");
	CAT_NUM(aritm_instr_counter);
	CAT_INST(" GF@!tmp3 bool@true");
	//float int
	CAT_INST("EQ GF@!tmp4 GF@!tmp4 string@float");
	CAT_INST("EQ GF@!tmp5 GF@!tmp5 string@int");
	CAT_INST("AND GF@!tmp3 GF@!tmp4 GF@!tmp5");
	//int float
	CAT_INST("EQ GF@!tmp6 GF@!tmp6 string@int");
	CAT_INST("EQ GF@!tmp7 GF@!tmp7 string@float");
	CAT_INST("AND GF@!tmp4 GF@!tmp6 GF@!tmp7");
	CAT_INST("OR GF@!tmp3 GF@!tmp3 GF@!tmp4");



	CAT_INST("JUMPIFEQ $type_error GF@!tmp3 bool@false");


	CAT_INST("EQ GF@!tmp3 GF@!tmp4 bool@true");
	CAT_STR("JUMPIFEQ $pretyp_1");
	CAT_NUM(aritm_instr_counter);
	CAT_STR(" GF@!tmp3 bool@true");
	CAT_INST("");
	//pretyp2
	CAT_INST("INT2FLOAT GF@!tmp2 GF@!tmp2");
	CAT_STR("JUMP $opr");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");

	//pretyp1
	CAT_STR("LABEL $pretyp_1");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");
	CAT_INST("INT2FLOAT GF@!tmp1 GF@!tmp1");
		

	//jsou stejne, zjistuji operaci(add, cat)
	CAT_STR("LABEL $opr");
	CAT_NUM(aritm_instr_counter);
	CAT_INST("");

	CAT_STR("JUMPIFEQ $type_error");
	CAT_INST(" GF@!tmp8 string@bool");

	CAT_STR("JUMPIFEQ $type_error");
	CAT_INST(" GF@!tmp8 string@nil");

	

	CAT_INST("POPS GF@!tmp3");
	CAT_INST("POPS GF@!tmp4");
	CAT_INST("PUSHS GF@!tmp2");
	CAT_INST("PUSHS GF@!tmp1");



	CAT_INST("GTS");

	
	aritm_instr_counter++;
}

void gen_stack_and()
{
	CAT_INST("ANDS");
}

void gen_stack_or()
{
	CAT_INST("ORS");
}

void gen_stack_not()
{
	CAT_INST("NOTS");
}

void gen_int2float()
{
	CAT_INST("INT2FLOATS");
}
void gen_clear()
{
	CAT_INST("CLEARS");
}

void gen_stack_concatanate()//konkatenace nelze na zasobniku, poziti pomocnych promennych
{
	CAT_INST("POPS GF@!tmp1");
	CAT_INST("POPS GF@!tmp2");

	CAT_INST("CONCAT GF@!tmp3 GF@!tmp2 GF@!tmp1");
	CAT_INST("PUSHS GF@!tmp3");
}

void gen_if_start()
{
	if_counter++;
	CAT_INST(" # if");
	CAT_STR("JUMPIFEQ $");
	CAT_STR("else");
	CAT_NUM(if_counter);
	CAT_STR("!");
	CAT_NUM(if_block_counter);


	CAT_INST(" GF@!result bool@false");
}

void gen_else()
{
	CAT_INST(" # else");
	CAT_STR("JUMP $");
	
	CAT_STR("if_end");
	CAT_NUM(if_counter);
	CAT_STR("!");
	CAT_NUM(if_block_counter);
	CAT_INST("");


	
	CAT_STR("LABEL $");
	CAT_STR("else");
	CAT_NUM(if_counter);
	CAT_STR("!");
	CAT_NUM(if_block_counter);
	CAT_INST("");
	

}

void gen_if_end()
{
	CAT_STR("LABEL $");
	CAT_STR("if_end");
	CAT_NUM(if_counter);
	CAT_STR("!");
	CAT_NUM(if_block_counter);
	CAT_INST("");
	if_counter--;
	if(if_counter == 0)
		if_block_counter++;
}

void result_to_var(char *v_name)
{
	CAT_STR("MOVE LF@!");
	CAT_STR(v_name);
	CAT_STR(" GF@!result");
	CAT_INST("");
}
void retvar_to_result()
{
	CAT_STR("MOVE GF@!result TF@!retvar");
	CAT_INST("");
}


void gen_while_start()
{
	while_counter++;
	CAT_STR(" # while");
	CAT_INST("");
	CAT_STR("LABEL $");
	CAT_STR("while");
	CAT_NUM(while_counter);
	CAT_STR("!");
	CAT_NUM(while_block_counter);
	CAT_INST("");
	
}

void gen_while_condition()
{
	CAT_STR("JUMPIFEQ $");
	CAT_STR("while_end");
	CAT_NUM(while_counter);
	CAT_STR("!");
	CAT_NUM(while_block_counter);
	CAT_INST(" GF@!result bool@false");

}

void gen_while_end()
{
	if (while_counter == 1){
		CAT_STR(code_while->word);
		lexem_del_word(code_while);
	}
	CAT_STR("JUMP $");
	CAT_STR("while");
	CAT_NUM(while_counter);
	CAT_STR("!");
	CAT_NUM(while_block_counter);
	CAT_INST("");

	CAT_STR("LABEL $");
	CAT_STR("while_end");
	CAT_NUM(while_counter);
	CAT_STR("!");
	CAT_NUM(while_block_counter);
	CAT_INST("");

	while_counter--;
	if(while_counter == 0)
	{
		while_block_counter++;
	}	

}