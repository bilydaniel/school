#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>


//nazvy semaforu
#define SEMAPHORE_MAIN "/xbilyd01_sem1"
#define SEMAPHORE_BUS "/xbilyd01_sem2"
#define SEMAPHORE_CREATOR "/xbilyd01_sem3"
#define SEMAPHORE_RIDER "/xbilyd01_sem4"
#define SEMAPHORE_NASTUP "/xbilyd01_sem5"
#define SEMAPHORE_FINISH "/xbilyd01_sem6"
#define SEMAPHORE_ODJEZD "/xbilyd01_sem7"
#define SEMAPHORE_SHARED "/xbilyd01_sem8"
#define SEMAPHORE_BUSEND "/xbilyd01_sem9"


#define LOCKED 0
#define UNLOCKED 1



//promenne
int *shmmr;
int shmid;
FILE *f;
int ridID;
//semafory
sem_t *sem_poradi;
sem_t *sem_zastavka;
sem_t *sem_bus;
sem_t *sem_nastup;
sem_t *sem_creator;
sem_t *sem_finish;
sem_t *sem_odjezd;
sem_t *sem_shared;
sem_t *sem_busend;


//funkce
int bus();
int creator();
int rider();

void clean();
void busWrite(char* stg,int mode);
void ridWrite();