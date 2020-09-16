/*Implementace překladače imperativního jazyka IFJ18
Jakub Gajdošík xgajdo24
Daniel Bílý xbilyd01
Petr Marek xmarek69
Jan Fridrich xfridr07
*/
#include "stack.h"
#include "priority.h"


void SInit (tStack *Stack ) 
/*   ------
** Inicializace zásobníku.
**/
{
	Stack->top = 0;  
}	

void SPush (tStack *Stack, int token_type)
/*   ------
** Vloží hodnotu na vrchol zásobníku.
**/
{ 
                 /* Při implementaci v poli může dojít k přetečení zásobníku. */
  if (Stack->top==MAXSTACK) 
    fprintf(stderr,"Chyba: Došlo k přetečení zásobníku s ukazateli!\n");
  else {  
		Stack->top++;  
		Stack->a[Stack->top]=token_type;
	}
}	

int SPop (tStack *Stack)
/*         --------
** Odstraní prvek z vrcholu zásobníku a současně vrátí jeho hodnotu.
**/
{
                            /* Operace nad prázdným zásobníkem způsobí chybu. */
	if (Stack->top==0)  {
		fprintf(stderr,"Chyba: Došlo k podtečení zásobníku s ukazateli!\n");
		exit(99);
	}	
	else {
		return (Stack->a[Stack->top--]);
	}	
}

int STop (tStack *Stack)
/*         --------
** Vrati prvek z vrcholu zasobniku, ale neodstrani ho jako SPop.
**/
{
                            /* Operace nad prázdným zásobníkem způsobí chybu. */
	if (Stack->top==0)  {
		fprintf(stderr,"Chyba: Došlo k podtečení zásobníku s ukazateli!\n");
		exit(99);
	}	
	else {
		return (Stack->a[Stack->top]);
	}	
}

bool SEmpty (tStack *Stack)
/*   -------
** Je-li zásobník prázdný, vrátí hodnotu true.
**/
{
  return(Stack->top==0);
}	

//Nasleduje stack pro tokeny
//--------------------------------------------------------------------

void TInit (Token_Stack *Stack ) 
/*   ------
** Inicializace zásobníku.
**/
{
	Stack->top = 0;  
}	

void TPush (Token_Stack *Stack, token_t token)
/*   ------
** Vloží hodnotu na vrchol zásobníku.
**/
{ 
                 /* Při implementaci v poli může dojít k přetečení zásobníku. */
  if (Stack->top==MAXSTACK) 
    fprintf(stderr,"Chyba: Došlo k přetečení zásobníku s ukazateli!\n");
  else {  
		Stack->top++;  
		Stack->a[Stack->top] = token;
	}
}	

token_t TPop (Token_Stack *Stack)
/*         --------
** Odstraní prvek z vrcholu zásobníku a současně vrátí jeho hodnotu.
**/
{
                            /* Operace nad prázdným zásobníkem způsobí chybu. */
	if (Stack->top==0)  {
		fprintf(stderr,"Chyba: Došlo k podtečení zásobníku s ukazateli!\n");
		exit(99);
	}	
	else {
		return (Stack->a[Stack->top--]);
	}	
}

token_t TTop (Token_Stack *Stack)
/*         --------
** Vrati prvek z vrcholu zasobniku, ale neodstrani ho jako SPop.
**/
{
                            /* Operace nad prázdným zásobníkem způsobí chybu. */
	if (Stack->top==0)  {
		fprintf(stderr,"Chyba: Došlo k podtečení zásobníku s ukazateli!\n");
		exit(99);
	}	
	else {
		return (Stack->a[Stack->top]);
	}	
}

bool TEmpty (Token_Stack *Stack)
/*   -------
** Je-li zásobník prázdný, vrátí hodnotu true.
**/
{
  return(Stack->top==0);
}	