/*Implementace překladače imperativního jazyka IFJ18
Jakub Gajdošík xgajdo24
Daniel Bílý xbilyd01
Petr Marek xmarek69
Jan Fridrich xfridr07
*/
#include "symtable.h"

int HTSIZE = MAX_HTSIZE;
 
int hashCode ( tKey key ) {
	unsigned int h=0;
	const unsigned char *p;
	for(p=(const unsigned char*)key; *p!='\0'; p++)
		h = 65599*h + *p;
	return (h%HTSIZE);
}

/*
** Inicializace tabulky s explicitně zřetězenými synonymy.  Tato procedura
** se volá pouze před prvním použitím tabulky.
*/

void htInit ( tHTable* ptrht ) {
	for (int i = 0; i < HTSIZE; ++i)
		(*ptrht)[i] = NULL;
}

/* TRP s explicitně zřetězenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíče key.  Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek. Pokud prvek nalezen není, 
** vrací se hodnota NULL.
*/

tHTItem* htSearch ( tHTable* ptrht, tKey key ) {
	int hkey = hashCode(key);
	if((*ptrht)[hkey] != NULL)
	{
		if (!strcmp((*ptrht)[hkey]->key, key))
			return (*ptrht)[hkey];
		else if ((*ptrht)[hkey]->ptrnext != NULL)
		{
			tHTItem* tmp = (*ptrht)[hkey];
			while (tmp->ptrnext != NULL)
			{
				if (!strcmp(tmp->ptrnext->key, key))
					return tmp->ptrnext;
				tmp = tmp->ptrnext;
			}
		}
	}	
	return NULL;
}

/* 
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vkládá do tabulky ptrht položku s klíčem key a s daty
** data.  Protože jde o vyhledávací tabulku, nemůže být prvek se stejným
** klíčem uložen v tabulce více než jedenkrát.  Pokud se vkládá prvek,
** jehož klíč se již v tabulce nachází, aktualizujte jeho datovou část.
**
** Využijte dříve vytvořenou funkci htSearch.  Při vkládání nového
** prvku do seznamu synonym použijte co nejefektivnější způsob,
** tedy proveďte.vložení prvku na začátek seznamu.
**/

void htInsert ( tHTable* ptrht, tKey key, tData *data ) {
	tHTItem* tmp = htSearch(ptrht, key);
	int hkey = hashCode(key);
	if(tmp != NULL)
			tmp->data = data;		
	else
	{
		tmp = malloc(sizeof(tHTItem));
		tmp->key = key;
		tmp->data = data;
		tmp->ptrnext = (*ptrht)[hkey];
		(*ptrht)[hkey] = tmp;
	}
}
/*
** TRP s explicitně zřetězenými synonymy.
** Tato funkce zjišťuje hodnotu datové části položky zadané klíčem.
** Pokud je položka nalezena, vrací funkce ukazatel na položku
** Pokud položka nalezena nebyla, vrací se funkční hodnota NULL
**
** Využijte dříve vytvořenou funkci HTSearch.
*/

tData* htRead ( tHTable* ptrht, tKey key ) {

	tHTItem* tmp = htSearch(ptrht, key);
	if (tmp != NULL)
		return tmp->data;
	return NULL;
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vyjme položku s klíčem key z tabulky
** ptrht.  Uvolněnou položku korektně zrušte.  Pokud položka s uvedeným
** klíčem neexistuje, dělejte, jako kdyby se nic nestalo (tj. nedělejte
** nic).
**
** V tomto případě NEVYUŽÍVEJTE dříve vytvořenou funkci HTSearch.
*/

void htDelete ( tHTable* ptrht, tKey key ) {
	int hkey = hashCode(key);
	tHTItem* tmp = NULL;
	tHTItem* tmp2 = NULL;
	if((*ptrht)[hkey] != NULL)
	{
		if (!strcmp((*ptrht)[hkey]->key, key))
		{
			tmp = (*ptrht)[hkey]->ptrnext;
			free((*ptrht)[hkey]);
			(*ptrht)[hkey] = tmp;
		}
		else if ((*ptrht)[hkey]->ptrnext != NULL)
		{
			tmp = (*ptrht)[hkey];
			while (tmp->ptrnext != NULL)
			{
				if (!strcmp(tmp->ptrnext->key,key))
				{
					tmp2 = tmp->ptrnext->ptrnext;
					free(tmp->ptrnext);
					tmp->ptrnext = tmp2;
				}
				if (tmp->ptrnext != NULL)
					tmp = tmp->ptrnext;
			}
		}
	}	
}

/* TRP s explicitně zřetězenými synonymy.
** Tato procedura zruší všechny položky tabulky, korektně uvolní prostor,
** který tyto položky zabíraly, a uvede tabulku do počátečního stavu.
*/

void htClearAll ( tHTable* ptrht ) {
	tHTItem* tmp = NULL;
	tHTItem* tmp2 = NULL;
	for (int i = 0; i < HTSIZE; ++i)
	{
		tmp = (*ptrht)[i];
		while(tmp != NULL)
		{
			tmp2 = tmp;
			tmp = tmp->ptrnext;
			//free_lexem(tmp2->data->lexem);
			free(tmp2->data);
			free(tmp2);
		}
		(*ptrht)[i] = NULL;
	}
}