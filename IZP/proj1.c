/*
 * Soubor:  proj1.c
 * Datum:   2017/11/04
 * Autor:   Daniel BĂ­lĂ˝, xbilyd01@stud.fit.vutbr.cz
 * Projekt: PrĂĄce s textem, projekt Ä. 1 pro pĹedmÄt IZP
 */


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define VELIKOSTMESTA 101
#define VELIKOSTABECEDY 27

int duplicitni(char pismena[],char pismeno)//zjistuje duplicitu ulozenych pismen
{
	for(int i=0;pismena[i]!='\0';i++)//proiteruje vsechny jiz ulozena pismena a porovna s novym pismenem	
	{
		if(pismena[i]==pismeno)//pri rovnosti(duplicite) vraci 1 kterou dale vyhodnocuji
		return 1;
	}
	return 0;
} 

void naVelke(char *pole)//meni pismena ulozena v poli na velka
{
	for(int i=0;*(pole+i)!='\0';i++)
	{
		*(pole+i)=toupper(*(pole+i));
	}	
}

int zjistiIndex(char mesto[], char arg[])//porovna argument s nazvem mesta, vrati index dalsiho pismene z nazvu mesta
 {
	int i=0;
	
	while(mesto[i]==arg[i])
	{
		i++;
	}
	return i;
 }
 
 void setrid (char *pismena)//tridi podle abecedy
 {
	 for(int i=0;pismena[i]!='\0';i++)	//tridici algoritmus
	{
		for(int j=0;pismena[j]!='\0';j++)
		{
			char temp;
			
			if(pismena[j]>pismena[i])
			{
			temp=pismena[j];
			pismena[j]=pismena[i];
			pismena[i]=temp;
			}
		}
	}
 }

int zkontrolujVstup(char argv[])//kontroluje argumenty, prijma pouze pismena
{
	for(int i=0;argv[i]!='\0';i++)
	{
		if(!((argv[i]>='a'&&argv[i]<='z')||(argv[i]>='A'&&argv[i]<='Z')))
		{
		printf("Invalid input");
		return -1;	
		}
	}
	return 0;
}	
 
int main(int argc, char *argv[])		
{	
    char mesto[VELIKOSTMESTA];
	char arg[VELIKOSTMESTA]="";
	char pismena[VELIKOSTABECEDY]="";
	int poradi;
	int index=0;
	int found=0;
	char FoundCity[VELIKOSTMESTA];
	int printed=0;
	
	if (argc==2)//nemuzu pouzit primo argv, v pripade nezadani argumentu null pointer
	{
		if(zkontrolujVstup(argv[1])==-1)
		exit(0);
			
		strcpy(arg,argv[1]);		
		naVelke(arg);
	}
	
	while(scanf("%100s",mesto)==1)     //nacte mesto (scanf("%100s",mesto)==1) 
    {
		naVelke(mesto);//prevede nazev mesta na velka (case insensitive)
		
		if (strcmp(arg,mesto)==0)//v pripade rovnosti argumentu s mestem prislusne mesto vypise
		{
		printf("Found: %s\n",mesto);
		printed=1;
		}
		
		index=zjistiIndex(mesto,arg);//zjisti index dalsiho pismene 
		
		if (index==(int)strlen(arg))
		{
			if (argc==2)//v pripade found==1 vim ze argument muze vest pouze na jedno mesto, mesto ulozim do promenne
			{
				found+=1;
				strcpy(FoundCity,mesto);
			}
			
			if(!duplicitni(pismena,mesto[index]))//v pripade neduplicitniho noveho pismene jej ulozi
			{
				pismena[poradi]=mesto[index];
				poradi++;
			}
		}
		
		setrid(pismena);//abecedne setridi pismena 
	} 
	if (found==1&&printed==0)
	printf("Found: %s\n", FoundCity);
	else
	{
	if(strlen(pismena)==0&&printed==0)
	printf("Not found\n");
	else if (strlen(pismena)>0)
	printf("ENABLE: %s", pismena);
	}
	
	return 0;
}