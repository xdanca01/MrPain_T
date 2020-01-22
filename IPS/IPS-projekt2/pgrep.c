#include <stdio.h>
#include<unistd.h>
#include<thread>
#include<queue>
#include<mutex>
#include<vector>
#include <iostream>
#include<string.h>
#include <regex>


int MIN_SCORE;
int SCORE;
std::vector<std::mutex *> zamky; /* pole zamku promenne velikosti */
std::vector<std::mutex *> zamky3;
std::vector<std::mutex *> shared;
char *line;

char *read_line(int *res) {
	std::string line;
	char *str;
	if (std::getline(std::cin, line)) {
		str=(char *) malloc(sizeof(char)*(line.length()+1));
		strcpy(str,line.c_str());
		*res=1;
		return str;
	} else {
		*res=0;
		return NULL;
	}

}


void f(int ID, char *regex, int my_score) {
	std::regex re(regex);
	while(!(*(shared[1])).try_lock())
	{
		(*(zamky[ID])).lock();
		int neco;
		if(line)
		{
			int count = std::regex_match(line,re);
			neco = count*my_score;
		}
		else break;
		//lock SCORE
		(*(shared[0])).lock();
	//	printf("process %i, neco:%i line:%s regex:%s\n",ID,neco,line,regex);
		SCORE += neco;
		(*(shared[0])).unlock();
		if((*(shared[1])).try_lock() == 1) break;
		(*(zamky3[ID])).unlock();
	}
	
	//(*(shared[1])).unlock();
	(*(zamky3[ID])).unlock();
}

int main(int argc, char *argv[]) {

	if(argc % 2 == 1 || argc < 4)
	{
		printf("spatny pocet argumentu\n");
		return 1;
	}
	//posledni 2 budou pro globalni promenne
	int num = (argc-2)/2;
	int num_zamky = num;
	int num_zamky2 = 3;
	MIN_SCORE = atoi(argv[1]);
	std::vector <std::thread *> threads; /* pole threadu promenne velikosti */
	/* vytvorime zamky */
	zamky.resize(num_zamky); /* nastavime si velikost pole zamky */
	for(int i=0;i<num_zamky;i++){	
		std::mutex *new_zamek = new std::mutex();
		zamky[i]=new_zamek;
		/* Pokud je treba, tak vytvoreny zamek muzeme rovnou zamknout */
		(*(zamky[i])).lock();
	}
	zamky3.resize(num_zamky); /* nastavime si velikost pole zamky */
	for(int i=0;i<num_zamky;i++){	
		std::mutex *new_zamek = new std::mutex();
		zamky3[i]=new_zamek;
		/* Pokud je treba, tak vytvoreny zamek muzeme rovnou zamknout */
		(*(zamky3[i])).lock();
	}
	/* vytvorime zamky */
	shared.resize(num_zamky2); /* nastavime si velikost pole zamky */
	for(int i=0;i<num_zamky2;i++){	
		std::mutex *new_zamek2 = new std::mutex();
		shared[i]=new_zamek2;
		
		/* Pokud je treba, tak vytvoreny zamek muzeme rovnou zamknout */
	}
	(*(shared[1])).lock();
	/* vytvorime thready */
	threads.resize(num); /* nastavime si velikost pole threads */
	for(int i=0;i<num;i++){	
		std::thread *new_thread = new std::thread (f,i,argv[(i+1)*2],atoi(argv[i*2+3]));
		threads[i]=new_thread;
	}	
	
	/**********************************
	 * Vlastni vypocet pgrep
	 * ********************************/
	int res;
	line=read_line(&res);
	while (res) {
		for(int i = 0; i < num_zamky;i++) (*(zamky[i])).unlock();
		for(int i = 0; i < num_zamky;i++) (*(zamky3[i])).lock();
		if(SCORE >= MIN_SCORE) printf("%s\n",line);
		SCORE = 0;
		free(line); /* uvolnim pamet */
		line=read_line(&res);
	}
	for(int i = 0; i < num_zamky;i++) (*(shared[1])).unlock();
	for(int i = 0; i < num_zamky;i++) (*(zamky[i])).unlock();

	/**********************************
	 * Uvolneni pameti
	 * ********************************/

	/* provedeme join a uvolnime pamet threads */
	for(int i=0;i<num;i++){
		(*(threads[i])).join();
		delete threads[i];
	}
	/* uvolnime pamet zamku */
	for(int i=0;i<num_zamky;i++){
		delete zamky[i];
	}

	
}
