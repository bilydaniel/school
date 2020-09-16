//Daniel Bílý(xbilyd01) a Jakub Gajdošík(xgajdo24)
#include<stdio.h>
#include<thread>
#include<mutex>
#include<vector>
#include<iostream>
#include<string.h>
#include<regex>

char *line;
int counter;
int wait;
int end;
int num_regex;
std::vector<std::mutex *> zamky;
std::mutex mutex_1;
std::mutex threads_rdy;

std::mutex mutex_counter;

char *to_cstr(std::string a) 
{
	// prevede retezec v c++ do retezce v "c" (char *)
	char *str;
	str=(char *) malloc(sizeof(char)*(a.length()+1));
	strcpy(str,a.c_str());
	return str;
}

char *read_line(int *res)
{
	std::string line;
	char *str;
	if (std::getline(std::cin, line)) 
	{
		str=to_cstr(line);
		*res=1;
		return str;
	} 
	else 
	{
		*res=0;
		return NULL;
	}
}


void f (char *reg1, char *reg2, int order)
{
	std::regex r1 (reg1);
	std::string output;
	while(!end)//ziot threadu do podminky
	{	
		zamky[order]->lock();//uvodni propust
		if (end == 0)
		{
			output = regex_replace (line, r1, reg2); //paralelni zpracovani

			while(counter != order);	//sekvencni vypsani podminkou

			printf("%s\n", to_cstr(output));

			counter++; //propust dalsiho threadu pro vypis
		
			mutex_1.lock();
			wait++;
			if (wait == num_regex)
			{
				wait = 0;		//reset citadel
				counter =  0;	
				threads_rdy.unlock(); //signal pro propust main
			}
			mutex_1.unlock();
		}
		
	}
}

int main(int argc, char *argv[])
{
	//dvojice argumentu pro regex + './psed' => lichy pocet, presmerovani stdin se jako argument nepocita
	if ((argc < 3) || (argc % 2 == 0))
	{
		printf("spravne pouziti ./psed RE1 REPL1 [ RE2 REPL2 ] [ RE3  REPL3 ] ...\n");
		exit(1);
	}

	int order = 0;//kazdy thread/regex ma sve poradi
	int res;//vysledek zda se nacetl radek
	end = 0; //koncova podminka
	counter = 0;//pricita a pri spravnem poradi provest urceny vypis threadu
	wait =0;//spolecne cekani na konci threadu na main

	num_regex = (argc - 1) / 2; // vypocet threadu podle argumentu

	//mutexy
	threads_rdy.lock();

	zamky.resize(num_regex); //nastavime si velikost pole zamky
	for(int i=0;i<num_regex;i++){	
		std::mutex *new_zamek = new std::mutex();
		zamky[i]=new_zamek;
		zamky[i]->lock(); //zacatek je v zamklem stavu
	}


	//threads
	std::vector <std::thread *> threads;
	threads.resize(num_regex); // nastavime si velikost pole threads
	for(int i = 0;i < num_regex;i++)
	{	
		std::thread *new_thread = new std::thread (f,argv[i * 2+1],argv[i * 2+2], order);
		threads[i]=new_thread;
		order++;
	}
	
	//core
	line=read_line(&res);
	while (res) 
	{
		for (int i = 0; i < num_regex; ++i)
			zamky[i]->unlock();
		threads_rdy.lock();
		free(line);
		line=read_line(&res);
	}

	//zastaveni threadu
	end = 1;
	for (int i = 0; i < num_regex; ++i)
			zamky[i]->unlock();

	//uvolnovani threadu a mutexu
	for(int i=0;i<num_regex;i++)
	{

		(*(threads[i])).join();
		delete threads[i];
		delete zamky[i];

	}
	return 0;
}