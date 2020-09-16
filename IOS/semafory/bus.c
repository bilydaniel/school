#include "sem.h"

int bus(int ABT)//int argc, char *argv[]
{
	
	f=NULL;	
	int cestujici;
	
	//START
	busWrite("start",0);
	
	
	sem_wait(sem_shared);
	cestujici=shmmr[1];//zbyli rideri
	shmmr[6]=shmmr[1];//rideri tuto jizdu
	sem_post(sem_shared);

	//cyklus jizd
	while(cestujici>0)
	{
		sem_wait(sem_shared);
		shmmr[3]=0;//vynulovani busu po prijezdu
		shmmr[5]=0;
		sem_post(sem_shared);

		sem_wait(sem_zastavka);//zamceni zastavky pri prijezdu BUS
		busWrite("arrival",0);

		//pokud jsou na zastace rideri
		if(shmmr[2]>0)
		{
			busWrite("start boarding",1);
			
			sem_post(sem_bus);//otevreeni busu pro nastup
			sem_wait(sem_nastup);//cekani na dokonceni nastupu

			//ukoknceni nastupu
			busWrite("end boarding",1);
			
			//pousteni rideru dal(kvuli finish)
			if(shmmr[3]>0)
			{
				sem_post(sem_finish);//pusteni rideru
				sem_wait(sem_creator);//cekani na pruchod vsech rideru

			}	

		}
		shmmr[7]=shmmr[3];//ulozeni minule jizdy(poctu cestujicich)
		busWrite("depart",0);

		sem_post(sem_zastavka);//otevreni zastavky po odjezdu busu


		//uspacni busu
		srand(time(NULL));
		int r;
		if(ABT!=0)
		{
			r=rand();
			usleep(r%(1000*(ABT+1)));
		}

		//end
		busWrite("end",0);

		
		//propusteni koncicich rideru
		if(shmmr[7]>0)
		{
			sem_post(sem_odjezd);	
			sem_wait(sem_busend);
		}
		
		//aktualizace cestujicich
		sem_wait(sem_shared);
		cestujici=shmmr[1];
		sem_post(sem_shared);
	}
	

	//finish
	busWrite("finish",0);


exit(0);
return 0;
}




void busWrite(char *stg,int mode)
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
			fprintf(f,"%d \t:BUS\t: %s \n",shmmr[0],stg);	
		}
		if(mode==1)
		{
			fprintf(f,"%d \t:BUS\t: %s: %d \n",shmmr[0],stg,shmmr[2]);			
		}
		
		fclose(f);
		f=NULL;
		sem_post(sem_poradi);
}

