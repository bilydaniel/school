#include "sem.h"

int creator(int riders,int ART,int capacity)
{
	
	
	//uspacni a vytvareni rideru
	srand(time(NULL));
	int r;
	int id;
	for(int i=1;i<=riders;i++)
	{
		if(ART!=0)
		{
			r=rand();
			usleep(r%(1000*(ART+1)));
		}
		
		id=fork();
		if(id==0)
		{
			
			rider(capacity);

		}
		
	}
	
exit(0);
return 0;
}