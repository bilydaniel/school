/*
 * Soubor:  proj2.c
 * Datum:   2017/11/19
 * Autor:   Daniel Bílý, xbilyd01@stud.fit.vutbr.cz
 * Projekt: Iterační výpočty, projekt č. 2 pro předmět IZP
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_VYSKA_MERAKU 100
#define MAX_POCET_ITERACI 14
#define MAX_UHEL 1.4

void vytiskniNapovedu()		//vytiskne napovedu
{
	printf("--tan A N M =>vypocita tangens uhlu alfa (A) zadaneho v radianech. Uhel se pocita pomoci matematicke knihovny, Taylorova polynomu a zretezenych zlomku. Prikaz vypise chybu vypoctu oproti matematicke knihovne. Vypocty se provadi v iteracich N az M.\n0 < N <= M < 14 \n"
			"\n"
		    "[-c X] -m A [B] =>Vypocita vzdalenost mereneho objketu od meraku pomoci uhlu alfa(A), pokud je zadany i uhel beta(B) vypocita se i vyska objektu. Je mozne upravit vysku meraku pomoci argumentu -c. Argumenty v hranatych zavorkach [] jsou nepovinne.0 < A <= 1.4\n0 < B <= 1.4\n0 < X <= 100");
}

double taylor_tan(double x, unsigned int n)		//vypocita tangens zadaneho uhlu pomoci taylorova polynomu
{
	double vysledek=0.;
	double mocnina=x;
	double citatel[13]={1, 1, 2, 17, 62, 1382, 21844, 929569, 6404582,		//hodnoty citatelu zlomku pro vypocet 
	443861162, 18888466084, 113927491862, 58870668456604};
	double jmenovatel[13]={1, 3, 15, 315, 2835, 155925, 6081075,			//hodnoty jmenovatelu zlomku pro vypocet 
	638512875, 10854718875, 1856156927625, 194896477400625,
	49308808782358125, 3698160658676859375};	
	
	for(int i=0;i<(int)n;i++)		//provede urceny pocet iteraci, vysledky jednotlivych iteraci scita
	{
		vysledek+=(citatel[i]*mocnina/jmenovatel[i]);
		mocnina=mocnina*x*x;
	}
	return vysledek;
}

double cfrac_tan(double x, unsigned int n)//vypocita tangens pomoci zretezeneho zlomku
{
	double cf=0.;
	double a;
	double b=x*x;
	
	a=(2.*n)-1.;			//pocatecni hodnota a
	cf=a-(b/(a+2.-cf)); 		//pocatecni hodnota cf
	
	for(;n>0;n--)
	{
		a=(2.*n)-1.;
		cf=a-(b/cf);
	} 
	return (x/cf);
}

void porovnej (double a, int n, int m)		//zavola funkce pro vypocet tangens, vysledky porovna a vypise
{
	double knihovna, polynom, zlomek;		//vysledky jednotlivych funkci		
	
	knihovna=tan(a);		//je potreba zavolat pouze jednou, vysledky se nelisi
	
	for(;n<=m;n++)			// provede urceny pocet iteraci, zavola funkce a vypise jejich vysledky
	{
		polynom=taylor_tan(a,n);
		zlomek=cfrac_tan(a,n);
		
		printf("%d %e %e %e %e %e \n",n,knihovna,polynom,fabs(polynom-knihovna),zlomek,fabs(zlomek-knihovna));
	}
	
}

double vypocitejVzdalenost(double vyska, double tanAlfa)		//vypocita vzdalenost meraku od mereneho objektu 
{
	double vzdalenost;
	
	vzdalenost=vyska/tanAlfa;
	
	return vzdalenost;
}

double vypocitejVysku(double vzdalenost,double tanBeta)		//vypocita vysku mereneho objektu
{
	double vyska;
	
	vyska=tanBeta*vzdalenost;
	
	return vyska;
}

void chyba(int typ)
{
	switch (typ){
		case 1:printf("nezadal jsi argument(zkus --help)\n");
		exit(EXIT_FAILURE);
		break;
		
		case 2:printf("nesparvne zadane parametry");
		exit(EXIT_FAILURE);
		break;
	}
}

int main (int argc, char *argv[])
{	
	double vyskaMeraku=1.5;			//vychozi hodnota vysky meraku
	double vyskaObjektu=0.;
	double alfa;
	double beta=0.;
	double vzdalenost;
	char *c;
	
	if (argc<2)		//v pripade nezadaneho argumentu vypise chybu
	{
		chyba(1);
	}
	
	if (strcmp(argv[1],"--help")==0)		//vypise napovedu
	{
		vytiskniNapovedu();
		exit(0);
	}
	
	if (strcmp(argv[1],"--tan")==0)		//overuje spranost zadanych argumentu, vola funkci porovnej
	{
		double uhel,n,m;
		
		n=strtod(argv[3],&c);
		m=strtod(argv[4],&c);
		
		if (n>0&&m>=n&&m<MAX_POCET_ITERACI)
		{	
			uhel=strtod(argv[2],&c);
			if (c[0]=='\0')
			porovnej(uhel,n,m);
			else chyba(2);
		}
		else chyba(2);
		exit(0);
	}
	
	for(int i=0;i<argc;i++)
	{
		if(strcmp(argv[i],"-c")==0)
		{
			vyskaMeraku=strtod(argv[i+1],&c);;
			
			if (!(vyskaMeraku>0&&vyskaMeraku<=MAX_VYSKA_MERAKU))
			{
				chyba(2);
			}
		}
		
		if (strcmp(argv[i],"-m")==0)
		{
			alfa=strtod(argv[i+1],&c);;
			
			if (!(alfa>0&&alfa<=MAX_UHEL))
			{
				chyba(2);
			}
			if(argv[i+2]!=NULL)
			{	
				beta=strtod(argv[i+2],&c);;
				
				if (!(beta>0&&beta<=MAX_UHEL))
				{
					chyba(2);
				}
			}	
		}
	}
	
	vzdalenost=vypocitejVzdalenost(vyskaMeraku,cfrac_tan(alfa,8));		//staci 8 iteraci, vice iteraci jiz nema vliv na presnost vypoctu na 10 desetinych mist
	
	if(beta>0)
		vyskaObjektu=vypocitejVysku(vzdalenost,cfrac_tan(beta,8))+vyskaMeraku;
	
	printf("%.10e\n%.10e\n", vzdalenost,vyskaObjektu);
	
	return 0;
}