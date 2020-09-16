#include "sem.h"

int rider(int capacity)
{
	f=NULL;

	sem_wait(sem_shared);
	shmmr[4]+=1;
	sem_post(sem_shared);
	ridID=shmmr[4];
	//start
	ridWrite("start",0);

	//na zastavku
	sem_wait(sem_zastavka);

	sem_wait(sem_shared);
	shmmr[2]+=1;//zastavka
	sem_post(sem_shared);
	//enter
	ridWrite("enter",1);

	sem_post(sem_zastavka);

	//do busu
	sem_wait(sem_bus);

	sem_wait(sem_shared);
	shmmr[1]-=1;
	shmmr[2]-=1;
	shmmr[3]+=1;
	shmmr[5]+=1;//pocet cestujicich (pro vystup)
	
	sem_post(sem_shared);
	
	//boarding

	ridWrite("boarding",0);
	
	//kapacita
	//pokud je bus plny nebo zastavka prazdna bus odjizdi,
	//jinak se pusti dalsi rider
	if(shmmr[2]>0&&shmmr[3]<capacity)
	{
		sem_post(sem_bus);
	}
	else
	{
		sem_post(sem_nastup);
	}

	//cekani kvuli finish
	sem_wait(sem_finish);

	sem_wait(sem_shared);
	shmmr[5]-=1;
	sem_post(sem_shared);
	if(shmmr[5]>0)
	{
		sem_post(sem_finish);
	}
	else
	{
		//pusteni busu
		sem_post(sem_creator);
	}


	
	//cekani rideru z predchozi jizdy
	sem_wait(sem_odjezd);
	sem_wait(sem_shared);
	shmmr[7]-=1;
	sem_post(sem_shared);
	if(shmmr[7]>0)
	{
		sem_post(sem_odjezd);
	}
	else
	{
		sem_post(sem_busend);
	}

	//finish
	ridWrite("finish",0);
	

	
exit(0);
return 0;
}




void ridWrite(char *stg,int mode)
{

	sem_wait(sem_poradi);
	shmmr[0]+=1;
	f=fopen("proj2.out","a");
	if(f==NULL)
	{
		fprintf(stderr,"Otevereni souboru se nezdarilo");
		clean();
		exit(1);
	}
	
	if(mode==0)
	{
		fprintf(f,"%d \t:RID %d \t: %s\n",shmmr[0],ridID,stg);
	}
	if(mode==1)
	{
		fprintf(f,"%d \t:RID %d \t: %s: %d\n",shmmr[0],ridID,stg,shmmr[2]);
	}
	

	fclose(f);
	f=NULL;	
	sem_post(sem_poradi);

	
	
}