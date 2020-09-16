
/* ******************************* c203.c *********************************** */
/*  P?edm?: Algoritmy (IAL) - FIT VUT v Brn?                                */
/*  ?ol: c203 - Fronta znak? v poli                                          */
/*  Referen??implementace: Petr P?ikryl, 1994                               */
/*  P?epis do jazyka C: V?lav Topinka, z??2005                             */
/*  ?ravy: Kamil Je??ek, z??2018                                          */
/* ************************************************************************** */
/*
** Implementujte frontu znak? v poli. P?esnou definici typ? naleznete
** v hlavi?ov? souboru c203.h (ADT fronta je reprezentov?a strukturou tQueue,
** kter?obsahuje pole 'arr' pro ulo?en?hodnot ve front?a indexy f_index
** a b_index. V?echny implementovan?funkce mus?p?edpokl?at velikost pole
** QUEUE_SIZE, i kdy? ve skute?osti jsou rozm?y statick?o pole definov?y
** MAX_QUEUE. Hodnota QUEUE_SIZE slou??k simulaci fronty v r?zn?velk? poli
** a nastavuje se v testovac? skriptu c203-test.c p?ed testov??
** implementovan?ch funkc? Hodnota QUEUE_SIZE m??e nab?vat hodnot v rozsahu
** 1 a? MAX_QUEUE.
**
** Index f_index ukazuje v?dy na prvn?prvek ve front? Index b_index
** ukazuje na prvn?voln? prvek ve front? Pokud je fronta pr?dn? ukazuj?** oba indexy na stejn?m?to. Po inicializaci ukazuj?na prvn?prvek pole,
** obsahuj?tedy hodnotu 0. Z uveden?ch pravidel vypl?v? ?e v poli je v?dy
** minim?n?jeden prvek nevyu?it?.
**
** P?i libovoln?operaci se ??n? z index? (f_index i b_index) nesni?uje
** vyjma p??adu, kdy index p?es?ne hranici QUEUE_SIZE. V tom p??ad?** se "posunuje" znovu na za?tek pole. Za t?to ??lem budete deklarovat
** pomocnou funkci NextIndex, kter?pro kruhov? pohyb p?es indexy pole
** vyu???operaci "modulo".
**
** Implementujte n?leduj??funkce:
**
**    nextIndex ..... pomocn?funkce - viz popis v??e
**    queueInit ..... inicializace fronty
**    queueEmpty .... test na pr?dnost fronty
**    queueFull ..... test, zda je fronta zapln?a (vy?rp?a kapacita)
**    queueFront .... p?e?e hodnoty prvn?o prvku z fronty
**    queueRemove ... odstran?prvn?prvek fronty
**    queueGet ...... p?e?e a odstran?prvn?prvek fronty
**    queueUp ....... za?azen?prvku na konec fronty
**
** Sv??e?en???ln?komentujte!
**
** Terminologick?pozn?ka: Jazyk C nepou???pojem procedura.
** Proto zde pou???e pojem funkce i pro operace, kter?by byly
** v algoritmick? jazyce Pascalovsk?o typu implemenov?y jako
** procedury (v jazyce C procedur? odpov?aj?funkce vracej??typ void).
**
**/

#include "c203.h"

void queueError (int error_code) {
/*
** Vytiskne upozorn??na to, ?e do?lo k chyb?
** Tato funkce bude vol?a z n?ter?ch d?e implementovan?ch operac?
**
** TUTO FUNKCI, PROS?E, NEUPRAVUJTE!
*/
	static const char* QERR_STRINGS[MAX_QERR+1] = {"Unknown error","Queue error: UP","Queue error: FRONT","Queue error: REMOVE","Queue error: GET","Queue error: INIT"};
	if ( error_code <= 0 || error_code > MAX_QERR )
		error_code = 0;
	printf ( "%s\n", QERR_STRINGS[error_code] );
	err_flag = 1;
}

void queueInit (tQueue* q) {
/*
** Inicializujte frontu n?leduj?? zp?sobem:
** - v?echny hodnoty v poli q->arr nastavte na '*',
** - index na za?tek fronty nastavte na 0,
** - index prvn?o voln?o m?ta nastavte tak?na 0.
**
** V p??ad? ?e funkce dostane jako parametr q == NULL, volejte funkci
** queueError(QERR_INIT).
*/	
	if(q == NULL)
		queueError(QERR_INIT);
	else
	{
		for(int i = 0; i < QUEUE_SIZE; i++)
		{
			q->arr[i] = '*';
		}
		q->f_index = 0;
		q->b_index = 0;
	}
	

	//  solved = FALSE;                  /* V p??ad??e?en? sma?te tento ??ek! */
}

int nextIndex (int index) {
/*
** Pomocn?funkce, kter?vrac?index n?leduj??o prvku v poli.
** Funkci implementujte jako jedin? prikaz vyu??aj??operace '%'.
** Funkci nextIndex budete vyu??at v dal??h implementovan?ch funkc?h.
*/	
	//pouziti modulo pro pripad kdy hodnota presahne velikost fronty
	return (index +1) % QUEUE_SIZE;



	  //solved = FALSE;                  /* V p??ad??e?en? sma?te tento ??ek! */

}

int queueEmpty (const tQueue* q) {
/*
** Vrac?nenulovou hodnotu, pokud je frona pr?dn? jinak vrac?hodnotu 0. 
** Funkci je vhodn?implementovat jedn? p??azem return.
*/
		//fronta je prazdna pokud se zacatecni a koncovy index rovnaji
		return (q->f_index == q->b_index);

	 // solved = FALSE;                  /* V p??ad??e?en? sma?te tento ??ek! */
}

int queueFull (const tQueue* q) {
/*
** Vrac?nenulovou hodnotu, je-li fronta pln? jinak vrac?hodnotu 0. 
** Funkci je vhodn?implementovat jedn? p??azem return
** s vyu?it? pomocn?funkce nextIndex.
*/
	//fronta je plna pokud se nasledujici hodnota koncoveho indexu rovna pocatecnimu
	//(koncovy index ukazuje na posledni volny index)

	return (q->f_index == nextIndex(q->b_index));

	  //solved = FALSE;                  /* V p??ad??e?en? sma?te tento ??ek! */
}

void queueFront (const tQueue* q, char* c) {
/*
** Prost?ednictv? parametru c vr??znak ze za?tku fronty q.
** Pokud je fronta pr?dn? o?et?ete to vol?? funkce queueError(QERR_FRONT).
** Vol??t?o funkce p?i pr?dn?front?je v?dy nutn?pova?ovat za nekorektn?
** B?v?to toti? d?sledek ?patn?o n?rhu algoritmu, ve kter? je fronta
** pou?ita. O takov?situaci se proto mus?program?or-v?voj? dozv??.
** V opa?? p??ad?je lad??program? obt?n???
**
** P?i implementaci vyu?ijte d??e definovan?funkce queueEmpty.
*/
	if(queueEmpty(q))
		queueError(QERR_FRONT);
	else
	{
		*c = q->arr[q->f_index];
	}

	  //solved = FALSE;                  /* V p??ad??e?en? sma?te tento ??ek! */
}

void queueRemove (tQueue* q) {
/*
** Odstran?znak ze za?tku fronty q. Pokud je fronta pr?dn? o?et?ete
** vzniklou chybu vol?? funkce queueError(QERR_REMOVE).
** Hodnotu na uvoln??pozici ve front?nijak neo?et?ujte (nep?episujte).
** P?i implementaci vyu?ijte d??e definovan?funkce queueEmpty a nextIndex.
*/
	if(queueEmpty(q))
		queueError(QERR_REMOVE);
	else
	{
		q->f_index = nextIndex(q->f_index);
	}


	  //solved = FALSE;                  /* V p??ad??e?en? sma?te tento ??ek! */
}

void queueGet (tQueue* q, char* c) {
/*
** Odstran?znak ze za?tku fronty a vr??ho prost?ednictv? parametru c.
** Pokud je fronta pr?dn? o?et?ete to vol?? funkce queueError(QERR_GET).
**
** P?i implementaci vyu?ijte d??e definovan?ch funkc?queueEmpty,
** queueFront a queueRemove.
*/

	if(queueEmpty(q))
		queueError(QERR_GET);
	else
	{
		queueFront(q, c);
		queueRemove(q);
	}

	  //solved = FALSE;                  /* V p??ad??e?en? sma?te tento ??ek! */
}

void queueUp (tQueue* q, char c) {
/*
** Vlo??znak c do fronty. Pokud je fronta pln? o?et?ete chybu vol??
** funkce queueError(QERR_UP). Vkl???do pln?fronty se pova?uje za
** nekorektn?operaci. Situace by mohla b?t ?e?ena i tak, ?e by operace
** neprov??a nic, ale v p??ad?pou?it?takto definovan?abstrakce by se
** obt?n?odhalovaly chyby v algoritmech, kter?by abstrakci vyu??aly.
**
** P?i implementaci vyu?ijte d??e definovan?ch funkc?queueFull a nextIndex.
*/
	if(queueFull(q))
		queueError(QERR_UP);
	else
	{
		q->arr[q->b_index] = c;
		q->b_index = nextIndex(q->b_index);
	}

	 // solved = FALSE;                  /* V p??ad??e?en? sma?te tento ??ek! */
}
/* Konec p??ladu c203.c */
