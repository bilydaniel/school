/*Implementace překladače imperativního jazyka IFJ18
Jakub Gajdošík xgajdo24
Daniel Bílý xbilyd01
Petr Marek xmarek69
Jan Fridrich xfridr07
*/
#include <stdbool.h>
#include "stack.h"
#include "scanner.h"
#include "token.h" 
#include "symtable.h"
#include "priority.h"

#define TYPE_FUNCTION 85


#define NO_ERROR 500
#define ERROR_1 501
#define ERROR_2 502
#define ERROR_3 503
#define ERROR_4 504
#define ERROR_5 505
#define ERROR_6 506
#define ERROR_9 507
#define ERROR_99 508
#define ERROR_IDK 509

#define ALREADY_DEF 530
#define WITHOUT_TYPE 531


int program();
void znic_zasobniky();
int check_id();
void call_generator(int resu);
int rule_KW();
int rule_preset_fuctions();
int rule_print();
int check_end();
int rule_expresion_pusher();
int set_type_promene();
int rule_identifier();
int rule_param_counter();
int rule_definice_promene();
int rule_def();
int set_token_and_return();
bool top_of_stack_prepared_for_reduction(tStack *stack);
void do_E_rule(tStack *stack);
int rule_expr();
int is_err(int ret);
void init_parser();
void check_data_type();
