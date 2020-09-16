#include "sem.h"



int main(int argc, char *argv[])
{	

	//promenne
	int riders, capacity, ART, ABT;
	char *ptr;
	
	//argumenty
	if(argc==5)
	{	
		//RIDERS
		riders=strtol(argv[1],&ptr,10);
		if((ptr[0]!='\0')||(riders<=0))
		{
			fprintf(stderr,"nespravne zadan argument riders\n");
			exit(1);
		}
		
		//CAPACITY
		capacity=strtol(argv[2],&ptr,10);
		if((ptr[0]!='\0')||(capacity<=0))
		{
			fprintf(stderr,"nespravne zadan argument capacity\n");
			exit(1);
		}
		
		//ART
		ART=strtol(argv[3],&ptr,10);
		if((ptr[0]!='\0')||(ART<0)||(ART>1000))
		{
			fprintf(stderr,"nespravne zadan argument ART\n");
			exit(1);
		}
		
		//ABT
		ABT=strtol(argv[4],&ptr,10);
		if((ptr[0]!='\0')||(ABT<0)||(ABT>1000))
		{
			fprintf(stderr,"nespravne zadan argument ART\n");
			exit(1);
		}
	}
	else
	{
		fprintf(stderr,"nespravny pocet argumentu\n");
		exit(1);
	}

//Sdilena pamet
int shmid;
key_t key=ftok("main.c",42);
int siz=8*sizeof(int);
int shmflag=IPC_CREAT;

//tvorba sdilene pameti
if((shmid=shmget(key,siz,shmflag|0666))<0)
{
	fprintf(stderr,"shmget neuspel\n");
	clean();
	exit(1);
} 

//pripojeni sdilene pameti
if((shmmr=shmat(shmid,NULL,0))==(int*)-1)
{
	fprintf(stderr,"shmat neuspel\n");
	clean();
	exit(1);
}

	//tvorba semaforu
	if((sem_poradi=sem_open(SEMAPHORE_MAIN,O_CREAT|O_EXCL,0666,UNLOCKED))==SEM_FAILED)
	{
		fprintf(stderr,"sem_poradi se nevytvoril\n");
		clean();
		exit(1);	
	}
	if((sem_zastavka=sem_open(SEMAPHORE_BUS,O_CREAT|O_EXCL,0666,UNLOCKED))==SEM_FAILED)
	{
		fprintf(stderr,"sem_zastavka se nevytvoril\n");
		clean();
		exit(1);	
	}
	if((sem_creator=sem_open(SEMAPHORE_CREATOR,O_CREAT|O_EXCL,0666,LOCKED))==SEM_FAILED)
	{
		fprintf(stderr,"sem_creator se nevytvoril\n");
		clean();
		exit(1);	
	}
	if((sem_bus=sem_open(SEMAPHORE_RIDER,O_CREAT|O_EXCL,0666,LOCKED))==SEM_FAILED)
	{
		fprintf(stderr,"sem_bus se nevytvoril\n");
		clean();
		exit(1);	
	}
	if((sem_nastup=sem_open(SEMAPHORE_NASTUP,O_CREAT|O_EXCL,0666,LOCKED))==SEM_FAILED)
	{
		fprintf(stderr,"sem_nastup se nevytvoril\n");
		clean();
		exit(1);	
	}
	if((sem_finish=sem_open(SEMAPHORE_FINISH,O_CREAT|O_EXCL,0666,LOCKED))==SEM_FAILED)
	{
		fprintf(stderr,"sem_finish se nevytvoril\n");
		clean();
		exit(1);	
	}
	if((sem_odjezd=sem_open(SEMAPHORE_ODJEZD,O_CREAT|O_EXCL,0666,LOCKED))==SEM_FAILED)
	{
		fprintf(stderr,"sem_odjezd se nevytvoril\n");
		clean();
		exit(1);	
	}
	if((sem_shared=sem_open(SEMAPHORE_SHARED,O_CREAT|O_EXCL,0666,UNLOCKED))==SEM_FAILED)
	{
		fprintf(stderr,"sem_shared se nevytvoril\n");
		clean(); 
		exit(1);	
	}
	if((sem_busend=sem_open(SEMAPHORE_BUSEND,O_CREAT|O_EXCL,0666,LOCKED))==SEM_FAILED)
	{
		fprintf(stderr,"sem_busend se nevytvoril\n");
		clean(); 
		exit(1);	
	}


//inicializace sdilene pameti
shmmr[0]=0;//poradi vypsane akce
shmmr[1]=riders;//celkovy pocet cestujicich(ukonceni BUS)
shmmr[2]=0;//zastavka
shmmr[3]=0;//bus
shmmr[4]=0;//riders ID
shmmr[5]=0;//pocet cestujicich
shmmr[6]=0;//minula jizda (pro dekrementaci)
shmmr[7]=0;//minula jizda

//vycisteni souboru
f=fopen("proj2.out","w");
if(f==NULL)
{
	fprintf(stderr,"Otevereni souboru se nezdarilo");
	clean();
	exit(1);
}	
fclose(f);
f=NULL;

//FORK BUS
int idBus=fork();
if(idBus==0)
{
	bus(ABT);
}


//FORK CREATOR
int idCreator=fork();
if(idCreator==0)
{
	creator(riders,ART,capacity);
}


//CEKANI NA VSECHNY PROCESY
int status;
waitpid(idCreator,&status,0);
waitpid(idBus,&status,0);

clean();

return 0;
}



void clean()
{
	//odstraneni sdilene pameti
	shmdt(&shmid);
	shmctl(shmid,IPC_RMID,NULL);



	// uzavreni semaforu
	sem_close(sem_busend);
	sem_close(sem_finish);
	sem_close(sem_poradi);
	sem_close(sem_zastavka);
	sem_close(sem_creator);
	sem_close(sem_bus);
	sem_close(sem_nastup);
	sem_close(sem_odjezd);
	sem_close(sem_shared);

	// odstraneni semaforu
	sem_unlink(SEMAPHORE_BUSEND);
	sem_unlink(SEMAPHORE_SHARED);
	sem_unlink(SEMAPHORE_FINISH);
	sem_unlink(SEMAPHORE_MAIN);
	sem_unlink(SEMAPHORE_BUS);
	sem_unlink(SEMAPHORE_CREATOR);
	sem_unlink(SEMAPHORE_RIDER);
	sem_unlink(SEMAPHORE_NASTUP);
	sem_unlink(SEMAPHORE_ODJEZD);

}
