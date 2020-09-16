/*Implementace překladače imperativního jazyka IFJ18
Jakub Gajdošík xgajdo24
Daniel Bílý xbilyd01
Petr Marek xmarek69
Jan Fridrich xfridr07
*/
#include "priority.h"

int prior[TABLE_SIZE][TABLE_SIZE] =
{
//	|+- | */| \ | r | ( | ) | i | $ |
	{ R , S , S , R , S , R , S , R }, /// +-
	{ R , R , R , R , S , R , S , R }, /// */
	{ R , S , R , R , S , R , S , R }, /// \ /
	{ S , S , S , N , S , R , S , R }, /// r (realtion operators) = <> <= < >= >
	{ S , S , S , S , S , E , S , N }, /// (
	{ R , R , R , R , N , R , N , R }, /// )
	{ R , R , R , R , N , R , N , R }, /// i (id, int, double, string)
	{ S , S , S , S , S , N , S , N }  /// $
};

int get_prec_table_index(int symbol)
{
	switch (symbol)
	{
	case PLUS:
	case MINUS:
		return I_PLUS_MINUS;

	case MUL:
	case DIV:
		return I_MUL_DIV;

	case COMPARE:
	case LOE:
	case LESSTHAN:
	case MOE:
	case MORETHAN:
	case NOTEQUAL:
		return I_REL_OP;

	case LEFT_BRACKET:
		return I_LEFT_BRACKET;

	case RIGHT_BRACKET:
		return I_RIGHT_BRACKET;

	case TYPE_FLOAT:
	case TYPE_INT:
	case TYPE_IDENTIFIER:
	case TYPE_STRING:
		return I_DATA;

	default:
		return I_DOLLAR;
	}
}


//------------------------------------
