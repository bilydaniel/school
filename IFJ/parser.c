/*Implementace překladače imperativního jazyka IFJ18
Jakub Gajdošík xgajdo24
Daniel Bílý xbilyd01
Petr Marek xmarek69
Jan Fridrich xfridr07
*/
#include "parser.h"
  
#define TABULKA uvnitr_funkce == 0 ? h_tabulka:tmp_tabulka

extern int gen_to_main;
extern int while_counter;
extern int if_counter;

int token_type; //aktualni token
token_t *token = NULL;
tStack stack; //Zasobnik pro vyrazy
Token_Stack values; //Zasobnik pro tokeny
tStack operatory; //Zasobnik pro hodnoty
tHTable* h_tabulka; // Hashovaci tabulka
tHTable* tmp_tabulka; // Hashovaci tabulka
int brackets_counter=0; //Leve zavorky ++, prave --
int param_counter; //pocitadlo parametru pri definici funkce
int porovnavani_counter;//pocitadlo >,<,<=,>=,==,!=
bool already_init=false;//podmínka pro inicializaci parseru
int uvnitr_funkce=0;//pro orientaci jestli jsme v definic funkce nebo ne 
int typ_promene=WITHOUT_TYPE;//typ promene
bool uzavorkovana_funkce=false; // pro volani funkce flag na poznani jestli je funkce uzavorkovana nebo ne 
int pocitac_param_u_call=0;//pocitadlo argumentu pro volani funkce
int last_KW=WITHOUT_TYPE;//posledni pouzite klicove slovo ( je to pouze pro kontrolu aby nebyli pod sebou 2x else)
int deep_in_program = 0;//pocitadlo pro hloubku programu ( kvuli while,if apod)
int last_if_counter=0;//pocitadlo pro ify
int remember_token_type;//slouží pro uložení typu předešlého tokenu
bool generator_popl_token = false;
bool int2float=false;
int posledni_pop_index=0;
char *nazev_posledni_ID;
bool pouziti_relacnich_operatoru;
int skipovani; //0-ne 1-připrav se 2-skipni
int remember_gen_place=1;
 // simulace pravidla <program> -> <st-list>
int program(){
	if(!already_init){
		init_parser();
		generator_init();
		gen_to_main = 1;
	}
	int res; // vyhodnocovaní chyb pro parser
	
	if (set_token_and_return()!=END){
		znic_zasobniky();
	}
	switch(token_type){	
		case LEFT_BRACKET:
		case TYPE_INT:
		case TYPE_FLOAT:
		case TYPE_STRING:
			res = rule_expresion_pusher();
			if(is_err(res) != NO_ERROR){
				return res;
			}

		break;
		case DEF:
			res = rule_def();
			if(is_err(res) != NO_ERROR){
				return res;
			}
		break; 
		case END_OF_FILE:
			gen_main_end();
			print_output();	
			return NO_ERROR;
		break;
		case END_OF_LINE:
			res = program();
			return res;
		break;
		case TYPE_IDENTIFIER:
			res=rule_identifier();	
			if(is_err(res) != NO_ERROR){
				return res;
			}
		break;
		default: 			
			res = rule_KW();
			if(is_err(res) != NO_ERROR){
				return res;
			}
		break;

	}
	res=program();
	return res;
}
//pomocna funkce na vycisteni zasobniku pred vykonanim noveho vyrazu
void znic_zasobniky(){
	while(!SEmpty(&stack)){
		SPop(&stack);
	}
	while(!SEmpty(&operatory)){
		SPop(&operatory);
	}
	while(!TEmpty(&values)){
		TPop(&values);
	}
}
//pravidlo pro klicova slova
int rule_KW(){
	int result=NO_ERROR;
	switch(token_type){
		case IF:
			deep_in_program++;
			last_if_counter=deep_in_program;
			last_KW=IF;
			set_token_and_return();
			if (token_type!=THEN){
				result=rule_expresion_pusher();
				if (is_err(result)!=NO_ERROR){
					return result;
				}
			}
			else {
				return ERROR_2;
			}
			if(token_type!=THEN){
				return ERROR_2;
			}
			if (set_token_and_return()!=END_OF_LINE){
				return ERROR_2;
			}
			if (typ_promene!=NIL && typ_promene!=WITHOUT_TYPE && pouziti_relacnich_operatoru==false){
				push_true();
				gen_stack_pop("GF","result");
			}
			gen_if_start();
			return NO_ERROR;

		break;
		case ELSE:
			gen_else();
			if (deep_in_program!=last_if_counter && last_KW==ELSE){
				return ERROR_2;
			}
			if (set_token_and_return()!=END_OF_LINE){
				return ERROR_2;
			}
			last_KW=ELSE;

			return NO_ERROR;
		break;
		case WHILE:
			if (gen_to_main != 2)
			{
				remember_gen_place=gen_to_main;	
			}
			gen_to_main=2;//generovani do specialniho stringu
			gen_while_start();

			deep_in_program++;
			last_KW=WHILE;
			if (set_token_and_return()!=END_OF_LINE){//pokud se to rovná end_of_line tak podmínka je nil ??
				result=rule_expresion_pusher();
				if (is_err(result)!=NO_ERROR){
					return result;
				}
			}else{
				return ERROR_2;
			}
			gen_while_condition();
			if (token_type!=DO){
				return ERROR_2;
			}
			if (set_token_and_return()!=END_OF_LINE){
				return ERROR_2;
			}
			return NO_ERROR;
			
		break;
		case END:
			last_KW=END;
			if (deep_in_program==0){
				if(uvnitr_funkce==1){
					result=check_end();
					if (is_err(result)!=NO_ERROR){
						return result;
					}
					htClearAll(TABULKA);
					gen_def_end();
					return NO_ERROR;
				}
				else{
					return ERROR_2;
				}
			}
			else{
				
				if(last_if_counter==deep_in_program && if_counter>=1){
					last_if_counter--;
					deep_in_program--;
					result=check_end();
					if (is_err(result)!=NO_ERROR){
						return result;
					}
					gen_if_end();
					return NO_ERROR;
				}
				else {
					deep_in_program--;
					if (while_counter==1){
						gen_to_main=remember_gen_place;
					}
					gen_while_end();
				}
				
				return NO_ERROR;	
			}
		break;
		default:
			return rule_preset_fuctions();
		break;

	}
	return ERROR_2;
}
//pravidlo pro vestavene funkce
int rule_preset_fuctions(){
	int result=NO_ERROR;
	int temp1=brackets_counter;
	brackets_counter=0;
	switch(token_type){
		case PRINT:
			if (set_token_and_return()==LEFT_BRACKET){
				uzavorkovana_funkce=true;
				brackets_counter++;
				if(set_token_and_return() < TYPE_IDENTIFIER && token_type > TYPE_STRING ){
					return ERROR_2;
				}
			}
			else if(token_type < TYPE_IDENTIFIER && token_type > TYPE_STRING){
				return ERROR_2;
			}

			result=rule_print();
			if (is_err(result)!=NO_ERROR){
				return result;
			}
			if (brackets_counter!=0){
				return ERROR_2;
			}
			brackets_counter=temp1;
			uzavorkovana_funkce=false;
			typ_promene=NIL;
			return NO_ERROR;
		break;
		case LENGHT:
			if (set_token_and_return()==LEFT_BRACKET){
				uzavorkovana_funkce=true;
				brackets_counter++;
				if(set_token_and_return() != TYPE_IDENTIFIER && token_type != TYPE_STRING ){
					return ERROR_2;
				}
			
			}
			else if(token_type != TYPE_IDENTIFIER && token_type != TYPE_STRING){
				return ERROR_2;
			}
			create_frame();	
			result = rule_expresion_pusher();
			if (is_err(result)!=NO_ERROR){
				return result;
			}
			if (typ_promene!=TYPE_STRING){

				return ERROR_5;
			}

			gen_argument(0);
			gen_call("length");
			znic_zasobniky();
			if(brackets_counter==0){
				brackets_counter=temp1;
				uzavorkovana_funkce=false;
				typ_promene=TYPE_INT;
				return NO_ERROR;
			}else{
				return ERROR_2;
			}
		break;
		case SUBSTR:
			if (set_token_and_return()==LEFT_BRACKET){			
				uzavorkovana_funkce=true;
				brackets_counter++;
				if(set_token_and_return() != TYPE_IDENTIFIER && token_type != TYPE_STRING ){
					return ERROR_6;//volani execute funkce
				}
			}
			
			else if(token_type != TYPE_IDENTIFIER && token_type != TYPE_STRING){
				return ERROR_6;
			}
			create_frame();
			result = rule_expresion_pusher();
			if (is_err(result)!=NO_ERROR){
				return result;
			}
			if (typ_promene!=TYPE_STRING){

				return ERROR_6;
			}
			if (token_type!=COMMA){
				return ERROR_6;
			}

			if(set_token_and_return() != TYPE_IDENTIFIER && token_type != TYPE_INT){
				return ERROR_6;
			}

			typ_promene=WITHOUT_TYPE;
			gen_argument(0);
			znic_zasobniky();

			result = rule_expresion_pusher();
			if (is_err(result)!=NO_ERROR){
				return result;
			}
			if (typ_promene!=TYPE_INT){

				return ERROR_6;
			}
			if(set_token_and_return() != TYPE_IDENTIFIER && token_type != TYPE_INT){
				return ERROR_6;
			}
			typ_promene=WITHOUT_TYPE;

			gen_argument(1);
			znic_zasobniky();
			result = rule_expresion_pusher();
			if (is_err(result)!=NO_ERROR){
				return result;
			}
			if (typ_promene!=TYPE_INT){

				return ERROR_6;
			}

			gen_argument(2);
			gen_call("substr");
			znic_zasobniky();
			if(brackets_counter==0){
				brackets_counter=temp1;
				uzavorkovana_funkce=false;
				typ_promene=TYPE_STRING;
				return NO_ERROR;//TADY SE MUSIME DOMLUVIT NA TOM ze budu vract podle toho co mi vrati dan :D bud NIL nebo STRING
			}
			break;
		case ORD:
			if (set_token_and_return()==LEFT_BRACKET){			
				uzavorkovana_funkce=true;
				brackets_counter++;
				if(set_token_and_return() != TYPE_IDENTIFIER && token_type != TYPE_STRING ){
					return ERROR_6;//volani execute funkce
				}
			
			}
			else if(token_type != TYPE_IDENTIFIER && token_type != TYPE_STRING){
				return ERROR_6;
			}
			create_frame();
			result = rule_expresion_pusher();
			if (is_err(result)!=NO_ERROR){
				return result;
			}
			if (typ_promene!=TYPE_STRING){

				return ERROR_6;
			}
			if (token_type!=COMMA){
				return ERROR_6;
			}

			if(set_token_and_return() != TYPE_IDENTIFIER && token_type != TYPE_INT){
				return ERROR_6;
			}
			typ_promene=WITHOUT_TYPE;
			gen_argument(0);
			znic_zasobniky();
			result = rule_expresion_pusher();
			if (is_err(result)!=NO_ERROR){
				return result;
			}
			if (typ_promene!=TYPE_INT){

				return ERROR_6;
			}
			gen_argument(1);
			gen_call("ord");
			znic_zasobniky();
			if(brackets_counter==0){
				brackets_counter=temp1;
				uzavorkovana_funkce=false;
				typ_promene=TYPE_INT;
				return NO_ERROR;
			}else{
				return ERROR_6;
			}
		break;
		case CHR:
			if (set_token_and_return()==LEFT_BRACKET){
				uzavorkovana_funkce=true;
				brackets_counter++;
				if(set_token_and_return() != TYPE_IDENTIFIER && token_type != TYPE_INT ){
					return ERROR_6;//volani execute funkce
				}
						
			}
			else if(token_type != TYPE_IDENTIFIER && token_type != TYPE_INT){
				return ERROR_6;
			}
			create_frame();	
			result = rule_expresion_pusher();
			if (is_err(result)!=NO_ERROR){
				return result;
			}
			if (typ_promene!=TYPE_INT){

				return ERROR_6;
			}

			gen_argument(0);
			gen_call("chr");
			znic_zasobniky();
			if(brackets_counter==0){
				brackets_counter=temp1;
				uzavorkovana_funkce=false;
				typ_promene=TYPE_STRING;
				return NO_ERROR;
			}else{
				return ERROR_6;
			}
		break;
		case INPUTS:
			if (set_token_and_return()==LEFT_BRACKET){
				if(set_token_and_return() != RIGHT_BRACKET){
					return ERROR_6;//volani execute funkce
				}
			
			}
			else if(token_type == RIGHT_BRACKET){
					return ERROR_6;//volani execute funkce
			}
			create_frame();	
			gen_call("inputs");
			brackets_counter=temp1;
			typ_promene=TYPE_STRING;
			return NO_ERROR;
		break;
		case INPUTF:
			if (set_token_and_return()==LEFT_BRACKET){
				if(set_token_and_return() != RIGHT_BRACKET){
					return ERROR_6;//volani execute funkce
				}
			
			}
			else if(token_type == RIGHT_BRACKET){
					return ERROR_6;//volani execute funkce
			}
			create_frame();	
			gen_call("inputf");
			brackets_counter=temp1;
			typ_promene=TYPE_FLOAT;
			return NO_ERROR;
		break;
		case INPUTI:
			if (set_token_and_return()==LEFT_BRACKET){
				if(set_token_and_return() != RIGHT_BRACKET){
					return ERROR_6;//volani execute funkce
				}
			
			}
			else if(token_type == RIGHT_BRACKET){
					return ERROR_6;//volani execute funkce
			}
			create_frame();	
			gen_call("inputi");
			brackets_counter=temp1;
			typ_promene=TYPE_INT;
			return NO_ERROR;
		break;
		default:
			return ERROR_2; // tady je myšleno když to nebude cokoliv co tady máme uvedeno 
		break;
	}
	return ERROR_2;
}
//pomocna funkce pro vestavenou funkci print
int rule_print(){
	int result=NO_ERROR;
	switch(token_type){
		case COMMA:
			if((set_token_and_return() < TYPE_IDENTIFIER && token_type > TYPE_STRING ) && (token_type!=LEFT_BRACKET)){
				return ERROR_6;
			}

			result=rule_print();
			if (is_err(result)!= NO_ERROR){
				return result;
			}
			return NO_ERROR;
		break;
		case TYPE_STRING:
		case TYPE_IDENTIFIER:
		case TYPE_INT:
		case TYPE_FLOAT:
		case LEFT_BRACKET:
			if (token_type==LEFT_BRACKET){
				brackets_counter++;
			}
			result = rule_expresion_pusher();
			if (is_err(result)!=NO_ERROR){
				return result;
			}
			create_frame();
			gen_call("print");
			znic_zasobniky();
			if (token_type==RIGHT_BRACKET && brackets_counter==1){
				brackets_counter--;
				set_token_and_return();
				return NO_ERROR;
			}
			else if (token_type==COMMA){
				result=rule_print();
				if (is_err(result)!=NO_ERROR){
					return result;
				}
			}
			else if ((token_type==END_OF_LINE || token_type== END_OF_FILE) && brackets_counter == 0){
				return NO_ERROR;
			}
			return NO_ERROR;
		break;
		default:
			return ERROR_6;
		break;
	}
}
//pomocna funkce na zjisteni jestli je za danym tokenem EOL
int check_end(){
	if(set_token_and_return()==END_OF_LINE || token_type==END_OF_FILE){
		return NO_ERROR;					
	}
	return ERROR_2;
}
//zpracovani vyrazu precedencni syntaktickou analyzou
int rule_expresion_pusher(){
	typ_promene=WITHOUT_TYPE;
	tStack tmp_s;
	SInit(&tmp_s);
	remember_token_type=token_type;
	int p_tok1;
	int2float=false;
	int top_stack = I_DOLLAR;
	pouziti_relacnich_operatoru = false;
	if (SEmpty(&stack)){
		SPush(&stack, top_stack);	
	}
	generator_popl_token=false;
	p_tok1 = get_prec_table_index(token_type);

			if (token_type!=TYPE_IDENTIFIER && (token_type>=TYPE_INT && token_type<=TYPE_STRING)){
				if (is_err(set_type_promene(token_type))!=NO_ERROR){

					return set_type_promene(token_type);
				}
			}
			else if (token_type==TYPE_IDENTIFIER){

				int IDecko=check_id();
				if (is_err(set_type_promene(IDecko))!=NO_ERROR){
					return set_type_promene(IDecko);
				}
				
	
			}

	while(!(top_stack == I_DOLLAR && p_tok1 == I_DOLLAR)){

		if(STop(&stack)==NONTERM){
			SPop(&stack);
			top_stack = STop(&stack);
			SPush(&stack, NONTERM);
		}else{			
			top_stack = STop(&stack);
		}

		if((top_stack == I_DOLLAR) && (p_tok1 == I_DOLLAR)){
			if (generator_popl_token){
				gen_stack_pop("GF","result");
			}
			break;
		}

		if(prior[top_stack][p_tok1]==N){
			
			if (brackets_counter >0 && generator_popl_token==false && (token_type== RIGHT_BRACKET || token_type==COMMA)){
				gen_stack_push(TPop(&values));
				gen_stack_pop("GF","result");
				znic_zasobniky();
				return NO_ERROR;

			}else if(brackets_counter >0 && generator_popl_token==true && (token_type== RIGHT_BRACKET || token_type==COMMA)){
				gen_stack_pop("GF","result");
				znic_zasobniky();
				return NO_ERROR;
			}
			if (top_stack==7 && p_tok1==5 && uzavorkovana_funkce==true && generator_popl_token==false){
				gen_stack_push(TPop(&values));
				gen_stack_pop("GF","result");
				znic_zasobniky();
				return NO_ERROR;
			}
			if (top_stack==7 && p_tok1==5 && uzavorkovana_funkce==true && generator_popl_token==true){
				gen_stack_pop("GF","result");
				znic_zasobniky();
				return NO_ERROR;
			}
			return ERROR_6; 
		}else if(prior[top_stack][p_tok1]==S){//Shiftuji
			if(STop(&stack) != top_stack){//Na vrcholu zasobiku je NONTERM
				SPop(&stack);
				SPush(&stack, S);
				SPush(&stack, NONTERM);
				top_stack = p_tok1; //Protoze jsme shiftovali, tak na vrcholu zasobniku NEMUZE byt I_DOLLAR
			}else{
				SPush(&stack, S);				
			}		
			if(p_tok1 == I_DATA){
				if(token_type >= TYPE_IDENTIFIER && token_type <= TYPE_STRING){
					TPush(&values, *token);
				}
				SPush(&operatory, 0);
			}
			else if((token_type>=PLUS && token_type <= MUL) || (token_type >= COMPARE&& token_type <= NOTEQUAL)){

				if (pouziti_relacnich_operatoru){
					skipovani++;
					generator_popl_token=false;
					pouziti_relacnich_operatoru=false;
				}
				if (token_type >= COMPARE&& token_type <= NOTEQUAL){
					pouziti_relacnich_operatoru=true;
				}
				if (!pouziti_relacnich_operatoru && skipovani == 0){
					skipovani++;
				}
				SPush(&operatory, token_type);
		  	}
		
			if (brackets_counter==0 && token_type== RIGHT_BRACKET && uzavorkovana_funkce==true){
				brackets_counter++;
			}
			else{
				SPush(&stack, p_tok1);
			}
			if(remember_token_type == I_DOLLAR){
				remember_token_type = END_OF_FILE;
				return ERROR_6;
				break;
			}else if (set_token_and_return() == END_OF_FILE || token_type == THEN ||
				 token_type == DO || token_type== END_OF_LINE || token_type == COMMA || (brackets_counter>0 && token_type== RIGHT_BRACKET)){
				remember_token_type = I_DOLLAR;
			}
			if (token_type==EQUALS){				
				return ERROR_2;
			}
			if (token_type==LEFT_BRACKET){
				brackets_counter++;
			}
			else if (token_type==RIGHT_BRACKET){
				brackets_counter--;
			}
			p_tok1 = get_prec_table_index(token_type);
			if (remember_token_type == LEFT_BRACKET && token_type==RIGHT_BRACKET )
			{
				token_type=NIL;

				push_false();
				gen_stack_pop("GF","result");
				znic_zasobniky();
				if ( typ_promene != WITHOUT_TYPE && typ_promene!=NIL){
					return ERROR_4;
				}
				set_token_and_return();

				return NO_ERROR;
			}
			remember_token_type=token_type;
			if (token_type!=TYPE_IDENTIFIER && (token_type>=TYPE_INT && token_type<=TYPE_STRING)){
				if (is_err(set_type_promene(token_type))!=NO_ERROR){
					return set_type_promene(token_type);
				}
			}
			else if (token_type==TYPE_IDENTIFIER){
				int IDecko=check_id();
				if (is_err(set_type_promene(IDecko))!=NO_ERROR){

					return set_type_promene(IDecko);
				}
			
	
			}

		}else if(prior[top_stack][p_tok1]==R){//Redukuji
			if(top_of_stack_prepared_for_reduction(&stack)){
			}else{
				return ERROR_6;
			}
			
		}else if(prior[top_stack][p_tok1]==E){
			if (brackets_counter==0 && token_type== RIGHT_BRACKET && uzavorkovana_funkce==true){
				brackets_counter++;
			}
			else{

				SPush(&stack, p_tok1);
			}
			

			if(remember_token_type == I_DOLLAR){
				remember_token_type = END_OF_FILE;
				return ERROR_6;
				break;
			}else if (set_token_and_return() == END_OF_FILE || token_type == THEN ||
				 token_type == DO || token_type== END_OF_LINE || token_type == COMMA || (brackets_counter>0 && token_type== RIGHT_BRACKET)){

				remember_token_type = I_DOLLAR;
			}
			if (token_type==EQUALS){
				return ERROR_2;
			}
			if (token_type==LEFT_BRACKET){
				brackets_counter++;
			}
			else if (token_type==RIGHT_BRACKET){
				brackets_counter--;
			}
				p_tok1 = get_prec_table_index(token_type);

			if (remember_token_type == LEFT_BRACKET && token_type==RIGHT_BRACKET )
			{
				token_type=NIL;

				push_false();
				gen_stack_pop("GF","result");
				znic_zasobniky();
				if ( typ_promene != WITHOUT_TYPE && typ_promene!=NIL){
					return ERROR_2;
				}
				set_token_and_return();

				return NO_ERROR;
			}
			remember_token_type=token_type;
			if (token_type!=TYPE_IDENTIFIER && (token_type>=TYPE_INT && token_type<=TYPE_STRING)){
				if (is_err(set_type_promene(token_type))!=NO_ERROR){
					return set_type_promene(token_type);
				}
			}
			else if (token_type==TYPE_IDENTIFIER){
				int IDecko=check_id();
				if (is_err(set_type_promene(IDecko))!=NO_ERROR){
					return set_type_promene(IDecko);
				}
			}
			if(top_of_stack_prepared_for_reduction(&stack)){
			}
			else{
				return ERROR_6;
			}
		}
	}
	if (!generator_popl_token){
		gen_stack_push(TPop(&values));
		gen_stack_pop("GF","result");
	}
	porovnavani_counter=0;
	return NO_ERROR;
}
//pomocna funkce pro kontrolu typove kompability 
int set_type_promene(int typu){

	if (typu==TYPE_STRING && (typ_promene == WITHOUT_TYPE || typ_promene == TYPE_STRING)){
		typ_promene=TYPE_STRING; 
	}
	else if(typu==TYPE_INT && typ_promene!=TYPE_FLOAT && (typ_promene == WITHOUT_TYPE || typ_promene == TYPE_INT )){
		typ_promene=TYPE_INT;
	}
	else if(typu==TYPE_FLOAT && (typ_promene == WITHOUT_TYPE || typ_promene == TYPE_FLOAT || typ_promene == TYPE_INT)){
		typ_promene=TYPE_FLOAT;
	}
	else if(typu==TYPE_INT && typ_promene==TYPE_FLOAT){
		typ_promene=TYPE_FLOAT;
	}
	else if(typu==WITHOUT_TYPE){
		return NO_ERROR;
	}
	else if (typu==NIL && (typ_promene == NIL || typ_promene==WITHOUT_TYPE)){
		typ_promene=NIL;
	}
	else{
		return ERROR_4; // jde o kombinovaní prace se stringy a čísly a nebo spatne zadefinovana promena :)
	}
	return NO_ERROR;
}
//vrati typ promene nebo WITHOUT TYPE kdyz se vola funkce
int check_id(){
	tKey k = ((char*)token->attribute.string->word);
	nazev_posledni_ID=k;
	if(htRead(TABULKA,k) == NULL){
		return WITHOUT_TYPE;
	}
	else{
		tData *data = malloc(sizeof(tData));
		data=htRead(TABULKA,k);
		if(!data->funkce){
			int data_type = data->type;
			return data_type;
		}else{
			return WITHOUT_TYPE;
		}
	}
	return ERROR_2;
}
//
int rule_identifier(){
	int result = NO_ERROR;
			tKey k = ((char*)token->attribute.string->word);
			nazev_posledni_ID=k;
			if(htRead(TABULKA,k) == NULL){//Pokud nebyla nalezena polozka, tak ji vloz				
				if(set_token_and_return() == EQUALS){ 	
					//zde se bude jednat o definici nove promene, musí se nainicializovat na nil, potom se do ní musí přidat hodnota která je za "="
					pouziti_relacnich_operatoru=true;
					tData *data = malloc(sizeof(tData));
					data->type=NIL;
					data->funkce=false;
					htInsert(TABULKA, k, data );//inicializace na NIL
					if (gen_to_main==2){//kvuli vytvareni novych promenych v cyklu while
						int i = gen_to_main;
						gen_to_main=remember_gen_place;
						remember_gen_place=i;
					}
					gen_var(k);	
					if (remember_gen_place==2){//pokud jsme prehodili string na ktery budem psat tak ho vratime
						int i = remember_gen_place;
						remember_gen_place=gen_to_main;
						gen_to_main=i;
					}		
					result=rule_definice_promene();
					if(is_err(result)!=NO_ERROR){
						return result;
					}

					if (typ_promene!=NIL){
						result_to_var(k);
					}
					data->funkce=false;
					data->type=typ_promene;
					htInsert(TABULKA, k, data );
					return NO_ERROR;
				}
				else{	
					tData *data = malloc(sizeof(tData));			
					//zavolani jeste nedeklarovane funkce 
					int temp1 = pocitac_param_u_call;
					int temp2 = brackets_counter;
					bool temp3 = uzavorkovana_funkce;
					pocitac_param_u_call=0;
					create_frame();
					brackets_counter=0;
					uzavorkovana_funkce=false;
					if (token_type==LEFT_BRACKET){
						uzavorkovana_funkce=true;
						brackets_counter++;
						if(set_token_and_return() < TYPE_IDENTIFIER && token_type > TYPE_STRING ){
							return ERROR_2;
						}
					}
					else if(token_type < TYPE_IDENTIFIER && token_type > TYPE_STRING){						
						return ERROR_2;
					}
					if (token_type==LEFT_BRACKET || (token_type>=TYPE_IDENTIFIER && token_type<=TYPE_STRING)){
						
						result = rule_param_counter();//zpocitame jeji argumenty
						if(is_err(result)!=NO_ERROR){	
							return result;//musí se volat error je blbost abychom vraceli error, když chceme vlastně jenom typ
						}
					}
					data->funkce=true;
					data->pocet_parametru=pocitac_param_u_call;
					data->definovano=false;
					pocitac_param_u_call = temp1;
					if (brackets_counter==-1){
						brackets_counter = temp2 - 1;
					}
					else{
						brackets_counter = temp2;
					}
					if (uzavorkovana_funkce == false && temp3 == true){
						uzavorkovana_funkce = false;
					}else{
						uzavorkovana_funkce = temp3;
					}
					gen_call(k);
					htInsert(TABULKA, k, data );//vlozime do hashtabulky jako jeste nedefinovanou
					return NO_ERROR;
				}
			}else{
				tData *data2 = malloc(sizeof(tData));
				data2=htRead(TABULKA,k);
				if(data2->funkce){
				//pokud je to funkce tak muze to byt uz definovana nebo znovu pouzivana funkce, musime zpocitat argumenty
					int temp1 = pocitac_param_u_call;
					int temp2 = brackets_counter;
					bool temp3 = uzavorkovana_funkce;
					pocitac_param_u_call=0;
					create_frame();

					brackets_counter=0;
					uzavorkovana_funkce=false;
					if (set_token_and_return()==LEFT_BRACKET){
						uzavorkovana_funkce=true;
						brackets_counter++;
						if(set_token_and_return() < TYPE_IDENTIFIER && token_type > TYPE_STRING ){
							return ERROR_2;
						}
					}
					else if(token_type < TYPE_IDENTIFIER && token_type > TYPE_STRING){
						return ERROR_2;
					}
					if (token_type==LEFT_BRACKET || (token_type>=TYPE_IDENTIFIER && token_type<=TYPE_STRING)){
						
						result = rule_param_counter();//pocitani jejich argumentu
						if(is_err(result)!=NO_ERROR){
							return result;
						}
					}
					if(pocitac_param_u_call!=data2->pocet_parametru){
						return ERROR_5;//funkce byla zadana ze spatnym poctem parametru nebo parametry byli spatne napsany
						
					}	
					pocitac_param_u_call = temp1;
					if (brackets_counter==-1){
						brackets_counter = temp2 - 1;
					}
					else{
						brackets_counter = temp2;
					}
					if (uzavorkovana_funkce == false && temp3 == true){
						uzavorkovana_funkce = false;
					}
					else{
						uzavorkovana_funkce = temp3;
					}
					gen_call(k);
					return NO_ERROR;
				}
				else{ //jenom pouziti promene
					typ_promene=data2->type;
					SPush(&stack,I_DOLLAR);
					SPush(&stack,NONTERM);
					TPush(&values, *token);

					if (set_token_and_return()==EQUALS){
						pouziti_relacnich_operatoru=true;
						SPop(&stack);
						SPop(&stack);
						TPop(&values);
						result=rule_definice_promene();
						if(is_err(result)!=NO_ERROR){
						
							return result;
						}
						result_to_var(k);
						//priradu do promene vysledek posledního result
						data2->type=typ_promene;
						htInsert(TABULKA, k, data2 );
						return NO_ERROR;
					}
					else if((token_type>=PLUS && token_type <= MUL) || (token_type >= COMPARE&& token_type <= NOTEQUAL)){	
						//generator hod na stack x
						result=rule_expresion_pusher();
						if(is_err(result)!=NO_ERROR){
							return result;
						}
						return NO_ERROR;
					}
					else if (token_type==END_OF_LINE || token_type==END_OF_FILE){
						result=rule_expresion_pusher();
						if(is_err(result)!=NO_ERROR){
							return result;
						}
						return NO_ERROR;
					}
					else{
						return ERROR_6;//muze nastat pokud za promenou bude nasledovat neco co by nemelo
					}
				}
			}

return ERROR_2;//jenom kvuli ukonceni funkce
}
int rule_param_counter(){
	int result=NO_ERROR;
	switch(token_type){
		case TYPE_STRING:
		case TYPE_IDENTIFIER:
		case TYPE_INT:
		case TYPE_FLOAT:
		case LEFT_BRACKET:		
			if (token_type==LEFT_BRACKET){
				brackets_counter++;
				set_token_and_return();

			}
			result = rule_expresion_pusher();
			if (is_err(result)!=NO_ERROR){
				return result;
			}
			gen_argument(pocitac_param_u_call);
			pocitac_param_u_call++;
			znic_zasobniky();
			if (token_type==RIGHT_BRACKET && brackets_counter==1){
				brackets_counter--;
				set_token_and_return();
				return NO_ERROR;
			}
			else if (token_type==COMMA){
				result=rule_param_counter();
				if (is_err(result)!=NO_ERROR){
					return result;
				}
			}
			if ((token_type==END_OF_LINE || token_type== END_OF_FILE || token_type==RIGHT_BRACKET) && brackets_counter == 0){
				return NO_ERROR;
			}
		break;
		case COMMA:
			if((set_token_and_return() < TYPE_IDENTIFIER && token_type > TYPE_STRING ) && (token_type!=LEFT_BRACKET)){
				return ERROR_2;//za carkou nenasleduje argument
			}

			result=rule_param_counter();
			if (is_err(result)!= NO_ERROR){
				return result;
			}
			return NO_ERROR;
		break;
	}	
	return ERROR_2;//jenom kvuli spravne ukonceni funkce
}
int rule_definice_promene(){
	int result=NO_ERROR;
	typ_promene=WITHOUT_TYPE;//není to zatím zadny typ
	set_token_and_return(); 
	switch(token_type){
		case LENGHT:
		case ORD:
		case PRINT:
		case CHR:
		case SUBSTR:
		case INPUTS:
		case INPUTI:
		case INPUTF:
		//prirazeni do promene jedne z vestavenych funkci
			result=rule_preset_fuctions();
			retvar_to_result();
			return result;
		case TYPE_IDENTIFIER:
		{
			tKey k = ((char*)token->attribute.string->word);
			tData *data2 = malloc(sizeof(tData));
			data2=htRead(TABULKA,k);
			if (data2==NULL){
				result = rule_identifier();
				return result;
			}
			else if(data2->type==NIL){
				typ_promene=NIL;
				if(set_token_and_return()!=END_OF_LINE && token_type!=END_OF_FILE){
					return ERROR_4;
				}
				else{
					return NO_ERROR;
				}
			}
			else if(data2->funkce){
				result=rule_identifier();			
				if(is_err(result)!=NO_ERROR){
					return result;
				}
				retvar_to_result();
				return NO_ERROR;
			}
			else{
				result=rule_expresion_pusher();
				return result;
			}

		break;
		}
		case TYPE_INT:
		case TYPE_FLOAT:
		case TYPE_STRING:
			result=rule_expresion_pusher();
			return result;
		break;
		default:
			return ERROR_2;//pokud za promenou neni hodnota, funkce ani promena
		break;
			
	}
return ERROR_2;//spravne ukonceni funkce
}
int rule_def(){
	int result = NO_ERROR;
	switch(token_type){
		case DEF:
			if(set_token_and_return() != TYPE_IDENTIFIER && deep_in_program!=0){
				return ERROR_2;
			}
			else{
				tKey k = ((char*)token->attribute.string->word);
				tData *data2 = malloc(sizeof(tData));
				data2=htRead(TABULKA,k);
				param_counter=0;
				brackets_counter=0;

				if(data2 == NULL || data2->definovano==false){//Pokud nebyla nalezena polozka, tak ji vloz	
					gen_def_start(k);
					set_token_and_return();
					result=rule_def();
					if(is_err(result)!=NO_ERROR){
						return result;
					}
					if (data2!=NULL){
						if (data2->pocet_parametru!=param_counter){
							return ERROR_5;
						}
					}
					tData *data = malloc(sizeof(tData)); //data pro hashovací tabulku
					data->pocet_parametru=param_counter;
					data->definovano=true;
					data->funkce=true;
					htInsert(h_tabulka, k, data );
					return NO_ERROR;
				}else{	
					return ERROR_3;//redefinice funkce
				}
			}
		break;
		case TYPE_IDENTIFIER:
		case LEFT_BRACKET:
			if (token_type==LEFT_BRACKET && brackets_counter==0){
				brackets_counter++;
				set_token_and_return();
				result = rule_def();
				return result;
			}else if (token_type==LEFT_BRACKET && brackets_counter>0){
				return ERROR_2;
			}	
			if (brackets_counter!=1){
				return ERROR_2;
			}
			tKey k = ((char*)token->attribute.string->word);
			gen_parametr(k,param_counter);
			tData *data = malloc(sizeof(tData));
			data->type=WITHOUT_TYPE;
			data->funkce=false;
			htInsert(TABULKA, k, data );
			param_counter++;

			if(set_token_and_return() != COMMA && token_type != RIGHT_BRACKET){
				return ERROR_2;//spatne zadavani parametru
			}
			result=rule_def();
			return result;

		break;
		case RIGHT_BRACKET:
			if (brackets_counter==1){
				brackets_counter--;
				if(set_token_and_return() != END_OF_LINE){
					return ERROR_2;//spatne ukonceni funkce
				}
				return NO_ERROR;
			}				
			return ERROR_2;//spatne zadane zavorky
		break;
		case COMMA:
			if (brackets_counter==1){
				if(set_token_and_return() != TYPE_IDENTIFIER){
					result = ERROR_2;
					return result;
				}
				result=rule_def();
				return result;
			}
			else{
				return ERROR_2;//spatne zadavani parametru
			}
		break;
	}
return ERROR_2;//spravńe ukonceni funkce
}
//funkce na zjisteni jestli je vyraz error
int is_err(int ret){
	switch(ret){
		case ERROR_1:
		case ERROR_2:
		case ERROR_3:
		case ERROR_4:
		case ERROR_5:
		case ERROR_6:
		case ERROR_9:
		case ERROR_99:
			return ret;
		break;
		default:
			return NO_ERROR;
		break;
	}
return ERROR_2;//spravńe ukonceni funkce
}
//inicializace parseru
void init_parser(){
	token = malloc(sizeof(token_t));
	brackets_counter = 0;
	SInit(&stack);
	TInit(&values);
	SInit(&operatory);
	h_tabulka = (tHTable*) malloc ( sizeof(tHTable) );
	htInit(h_tabulka);
	tmp_tabulka = (tHTable*) malloc ( sizeof(tHTable) );
	htInit(tmp_tabulka);
	already_init=true;
}
//nacteni dalsiho tokenu a nastaveni jeho typu jako globalni promene
int set_token_and_return(){
		get_token(token);
		token_type = token->type;
		return token_type;
}
//pomocna funkce na redukci vyrazu
bool top_of_stack_prepared_for_reduction(tStack *stack){
	int i;
	for(i = 0; ;i++){
		if(stack->a[(stack->top)-i] == S){
			break;
		}
	}
	switch(i){
		// E->i
		case 1:

			SPop(stack);
			SPop(stack);
			SPush(stack, NONTERM);
			return true;

		break;

		case 3:

			if(stack->a[(stack->top)] == NONTERM && stack->a[(stack->top)-2] == NONTERM){ 			
				if(stack->a[(stack->top)-1] == I_PLUS_MINUS){
					check_data_type();
					if (posledni_pop_index>((&values)->top)){
						switch_stack();
					}
					posledni_pop_index=((&values)->top);
					if((&operatory)->a[((&operatory)->top)-1] == PLUS){ //E -> E + E 
						if (typ_promene==TYPE_STRING){
							SPop (&operatory);
							SPop (&operatory);	
							gen_stack_concatanate();
						}else{
							SPop (&operatory);
							SPop (&operatory);	
							gen_stack_add();
						}

					}
					else if((&operatory)->a[((&operatory)->top)-1] == MINUS){//E -> E - E
						if (typ_promene==TYPE_STRING){
							return ERROR_4;
						}
						SPop (&operatory);
						SPop (&operatory);	
						gen_stack_sub();
						//gen_stack_pop("GF","result");
					}

					do_E_rule(stack);
					return true;
				}else if(stack->a[(stack->top)-1] == I_MUL_DIV){ 				
					check_data_type();
					if (typ_promene==TYPE_STRING){
						return ERROR_4;
					}
					if (posledni_pop_index>((&values)->top)){
						switch_stack();
					}
					posledni_pop_index=((&values)->top);
					if((&operatory)->a[((&operatory)->top)-1] == MUL){// E -> E * E 	

						SPop (&operatory);
						SPop (&operatory);	
						gen_stack_mul();
						//gen_stack_pop("GF","result");
					}
					else if((&operatory)->a[((&operatory)->top)-1] == DIV){//E -> E / E		
						SPop (&operatory);
						SPop (&operatory);	
						if (int2float){
							gen_stack_div();
						}else{
							gen_stack_idiv();
						}				
						//gen_stack_pop("GF","result");
					}

					do_E_rule(stack);
					return true;
				}else if(stack->a[(stack->top)-1] == I_REL_OP){ // E -> E relacni-operator E 
					if (skipovani==2){
						skipovani=0;
					}else{
						check_data_type();	
					}
					if (posledni_pop_index>((&values)->top)){
						switch_stack();
					}
					posledni_pop_index=((&values)->top);
					if((&operatory)->a[((&operatory)->top)-1] == COMPARE){	
						SPop (&operatory);
						SPop (&operatory);	
						gen_stack_eq();
					}
					if((&operatory)->a[((&operatory)->top)-1] == LOE){	
						SPop (&operatory);
						SPop (&operatory);	
						gen_less_or_equal();
					}
					if((&operatory)->a[((&operatory)->top)-1] == LESSTHAN){	
						SPop (&operatory);
						SPop (&operatory);	
						gen_stack_less_than();
					}
					if((&operatory)->a[((&operatory)->top)-1] == MOE){	
						SPop (&operatory);
						SPop (&operatory);	
						gen_more_or_equal();
					}
					if((&operatory)->a[((&operatory)->top)-1] == MORETHAN){	
						SPop (&operatory);
						SPop (&operatory);	
						gen_stack_more_than();
					}
					if((&operatory)->a[((&operatory)->top)-1] == NOTEQUAL){	
						SPop (&operatory);
						SPop (&operatory);	
						gen_not_equal();
					}
					do_E_rule(stack);
					return true;
				}
			//E->(E)
			}else if(stack->a[(stack->top)] == I_RIGHT_BRACKET && stack->a[(stack->top)-2] == I_LEFT_BRACKET){
				do_E_rule(stack);
			}else{
				return false;
			}
		break;
		default:
			return false;
		break;
	}
	return true;
}
//pomocna funkce ktera urcuje jestli se ma pretypovavat data
void check_data_type(){
	if (!int2float){
		if ((TTop(&values)).type == TYPE_FLOAT && generator_popl_token==true){
			int2float=true;
			gen_int2float();
			gen_stack_push(TPop(&values));
		}
		else if((TTop(&values)).type == TYPE_FLOAT && generator_popl_token==false){
			int2float=true;
			gen_stack_push(TPop(&values));
			generator_popl_token = true;
			posledni_pop_index=((&values)->top);
			if ((TTop(&values)).type == TYPE_INT){
				gen_stack_push(TPop(&values));
				gen_int2float();
			}
			else{
				gen_stack_push(TPop(&values));
			}
		}
		else{
			gen_stack_push(TPop(&values));
			if (!generator_popl_token){
				posledni_pop_index=((&values)->top);
				if((TTop(&values)).type == TYPE_FLOAT && int2float==false ){
					gen_int2float();
					gen_stack_push(TPop(&values));
					int2float=true;
					generator_popl_token=true;
				}
				else{
					gen_stack_push(TPop(&values));
					generator_popl_token=true;
				}
			}
		}
	}
	else if (int2float==true && (TTop(&values)).type == TYPE_INT){
		gen_stack_push(TPop(&values));
		gen_int2float();
	}
	else{
		if((TTop(&values)).type == TYPE_FLOAT){
			int2float=true;
		}
		gen_stack_push(TPop(&values));

		if (!generator_popl_token){
			posledni_pop_index=((&values)->top);
			if((TTop(&values)).type == TYPE_FLOAT && int2float==false ){
				gen_int2float();
				gen_stack_push(TPop(&values));
				int2float=true;
				generator_popl_token=true;
			}
			else{
				gen_stack_push(TPop(&values));
				generator_popl_token=true;
			}
		}
	}	
}
//po provedeni operace, odstrani ze zasobniku pocitany vyraz a nahradi ho NONTERM 
void do_E_rule(tStack *stack){
	SPop(stack);
	SPop(stack);	
	SPop(stack);	
	SPop(stack);	
	SPush(stack, NONTERM);			
}
