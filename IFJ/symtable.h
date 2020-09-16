/*Implementace překladače imperativního jazyka IFJ18
Jakub Gajdošík xgajdo24
Daniel Bílý xbilyd01
Petr Marek xmarek69
Jan Fridrich xfridr07
*/
#ifndef SYMTABLE_H
#define SYMTABLE_H
#include <stdbool.h>
#include "scanner.h"
#include "token.h"


/* Maximální velikost pole pro implementaci
   vyhledávací tabulky. Řešené procedury však
   využívají pouze HTSIZE prvků pole (viz deklarace této proměnné).
*/
#define MAX_HTSIZE 101

/* typ klíče (například identifikace zboží) */
typedef char* tKey;

/* typ obsahu (například cena zboží) */
typedef struct tData{
	int type; 	//typ co vrací funkce / proměná
	//lexem_t *lexem; 
	bool definovano; // jestli byla funkce definovana
	int pocet_parametru; 
	bool funkce; // rozhoduje jestli je funkce nebo ne ( když ne tak je proměna)
	int lenght; // delka u stringu

}tData;

/*Datová položka TRP s explicitně řetězenými synonymy*/
 typedef struct tHTItem{
	tKey key;				/* klíč  */
	tData *data;				/* obsah */
	struct tHTItem* ptrnext;	/* ukazatel na další synonymum */
} tHTItem;

/* TRP s explicitně zřetězenými synonymy. */
typedef tHTItem* tHTable[MAX_HTSIZE];

/*    
   POZOR! Pro správnou funkci TRP musí být hodnota této proměnné prvočíslem.
*/
extern int HTSIZE;

/* Hlavičky řešených procedur a funkcí. */

int hashCode ( tKey key );

void htInit ( tHTable* ptrht );

tHTItem* htSearch ( tHTable* ptrht, tKey key );

void htInsert ( tHTable* ptrht, tKey key, tData *data );

tData* htRead ( tHTable* ptrht, tKey key );

void htDelete ( tHTable* ptrht, tKey key );

void htClearAll ( tHTable* ptrht );


#endif