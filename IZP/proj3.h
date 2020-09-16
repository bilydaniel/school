/*
 * Kostra hlavickoveho souboru 3. projekt IZP 2017/18
 * a pro dokumentaci Javadoc.
 */

 ///@file proj3.h
	
	/** @defgroup Struktury Skupina struktur
  *  @{
  */
	///Struktura typu objekt. 
 struct obj_t {
	///Identifikacni cislo daneho objektu.
	int id;
	///Souradnice objektu na ose X.
    float x;
	///Souradnice objektu na ose Y.
    float y;
};

	///Struktura typu cluster(shluk).
struct cluster_t {
	///Aktualni velikost shluku (pocet objektu ve shluku).
    int size;
	///Celkova alokovana kapacita shluku.
    int capacity;
	///Ukazatel na prvni objekt ve shluku.
    struct obj_t *obj;
};
 /** @} */
 
 
 
 /**
  *
  *@brief Inicializuje shluk kteremu alokuje pamet urcenou parametrem cap.
  *
  *@param c Ukazatel na shluk, ktery je inicializovan.
  *@param cap Kapacita na kterou ma byt shluk inicializovan.
  *@pre	Shluk "c" jiz musi byt alokovan.
  *@pre Parametr "cap" musi byt vetsi nebo roven nule.
  *@post Velikost shluku je nastava na nulu.
  *@post Kapacita shluku urcena parametrem "cap".
  *@post Pokud je nektery z parametru nesparvny, program konci.
  *@returns Void.
  */
void init_cluster(struct cluster_t *c, int cap);

/**
  *Odsstrani vsechny objekty shluku a inicializuje jej jako prazdny.
  *
  *@param c Ukazatel na shluk, jehoz objekty maji byt odstraneny.
  *@pre Ukazatel musi byt alokovan.
  *@post Velikost i kapacita shluku jsou nastaveny na nulu.
  *@returns Void.
  */
void clear_cluster(struct cluster_t *c);



/**
  *Zvetsuje kapacitu shluku.
  *
  *@param c Ukazatel na shluk, ktery ma byt zvetsen.
  *@param new_cap Kapacita na kterou se ma shluk zvetsit.
  *@pre Shluk "c" musi byt alokovan.
  *@pre Parametr "new_cap" musi byt vetsi nebo roven nule.
  *@post Pokud je "new_cap" mensi nez puvodni kapacita, je vracen shluk s puvodni kapacitou.
  *@post Pokud se realokace nezdarila, vraci se NULL.
  *@returns Shluk "c" s novou kapacitou.
  */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
  *Prida objekt "obj" do shluku "c". Pokud se do nej nevejde, shluk je rozsiren.
  *
  *@param c Shluk do ktereho ma byt vlozen objekt "obj".
  *@param obj Objekt ktery ma byt vlozeb do shluku "c".
  *@returns Void.
  */
void append_cluster(struct cluster_t *c, struct obj_t obj);

/**
  *Do shluku "c1" prida objekty ze shluku "c2". Shluk "c1" bude v pripade nutnosti rozsiren.
  *Objekty ve shluku "c1" se seradi vzestupne podle hodnoty identifikacniho cisla.
  *Shluk "c2" zustane nezmenen.
  *
  *@param c1 Shluk do ktereho je kopirovano.
  *@param c2 Shluk ze ktereho je kopirovano. 
  *@pre Shluky "c1" i "c2" musi byt alokovany.
  *@post Shluk "c1" je vzestupne serazen podle hodnot id.
  *@returns Void.
  */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

/**
  *Odstrani shluk z pole shluku.
  *
  *@param carr Ukazatel na prvni shluk v poli.
  *@param narr Pocet prvku v poli.
  *@param idx Index shluku, ktery se ma odstranit.
  *@pre Parametr "idx" musi byt mensi nez velikost pole.
  *@pre Velikost pole musi byt vetsi nez nula.
  *@post Velikost pole se snizi o jedna.
  *@returns Novy pocet shluku v poli.
  */
int remove_cluster(struct cluster_t *carr, int narr, int idx);

/**
  *Pocita Euklidovskou vzdalenost dvou objektu.
  *
  *@param o1 Prvi objekt.
  *@param o2 Druhy objekt.
  *@pre Oba objekty musi byt alokovany.
  *@post Vzdalenost ma kladnou, nenulovou hodnotu.
  *@returns Vzdalenost mezi dvema objekty.
  */
float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
  *Pocita vzdalenost dvou shluku.
  *
  *@param c1 Prvni shluk.
  *@param c2 Druhy shluk.
  *@pre Oba shluky musi byt alokovany a mit size>0(mit alespon jeden objekt).
  *@post Vzdalenost ma kladnou, nenulovou hodnotu.
  *@returns Vzdalenost mezi dvema shluky.
  */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
  *Hleda dva nejblizsi shluky v poli shluku.
  *
  *@param carr Ukazetel na prvni prvek pole shluku.
  *@param narr Velikost pole shluku.
  *@param c1 Ukazatel na prvni ze dvou nejblizsich prvku.
  *@param c2 Ukazatel na druhy ze dvou nejblizsich prvku.
  *@pre Velikost pole musi byt vetsi nez nula.
  *@post Do parametru "c1" a "c2" se zapisi indexy nejblizsich shluku.
  *@returns Void.
  */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/**
  *Vytiskne shluk "c" na vystup stdout.
  *
  *@param c Shluk ktery ma byt vytisknut.
  *@pre Shluk "c" musi byt alokovan a obsahovat objekt.
  *@returns Void.
  */
void print_cluster(struct cluster_t *c);

/**
  *Nacte objekty ze souboru, kazdy objekt ulozi do jineho shluku. Shluky jsou pote ulozeny do pole shluku.
  *
  *@param filename Nazev souboru ze ktereho se cte.
  *@param arr Ukazatel na prvni prvek pole shluku.
  *@pre Ukazatel arr nesmi mit hodnotu NULL.
  *@post Soubor se po cteni zavre.
  *@returns Pocet nactenych objektu.
  */
int load_clusters(char *filename, struct cluster_t **arr);

/**
  *Vytiskne pole shluku.
  *
  *@param carr Ukazatel na prvni polozku pole.
  *@param narr Pocet prvku ktery se vytiskne.
  *@returns Void.
  */
void print_clusters(struct cluster_t *carr, int narr);


///Doporuceny pocet objektu o ktery se ma rozsirit kapacita shluku pri realokaci.
extern const int CLUSTER_CHUNK;