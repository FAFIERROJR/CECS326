#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "semaphore.h"
#include <cstring>
#include <ctime>

using namespace std;

const long U = 827395609;
const long V = 962094883;
const int BUFFSIZE = 2;

enum{RUN_CHILD, RUN_PARENT};

void childProc(SEMAPHORE &sem, bool *isUVFree);
void parentProc(pid_t PID[4], SEMAPHORE &sem, int shmid);
void parentCleanup(SEMAPHORE &sem, int shmid);

int main(){
	srand(time(NULL));
	pid_t PID[4] = {0, 0, 0, 0};
	int count = 0;
	SEMAPHORE sem(3);
	int shmid;
	bool *shmBUF;

	sem.V(RUN_CHILD);
	sem.V(RUN_CHILD);

	shmid = shmget(IPC_PRIVATE, BUFFSIZE*sizeof(bool), PERMS);
	shmBUF = (bool *)shmat(shmid, 0, SHM_RND);
	shmBUF[0] = true;
	shmBUF[1] = true;

	bool isUVFree[2] = {true, true};

	for(int i = 0; i < 4; i++){
		PID[i] = fork();
		if(PID[i] == 0){
			break;
		}
		cout << getpid() << ": child pid: " << PID[i] << endl;
	}

	while(true){
		if(PID[3] > 0){
			parentProc(PID, sem, shmid);
		}
		else{
			childProc(sem, shmBUF);
		}
	}
}

void childProc(SEMAPHORE &sem, bool *isUVFree){
	int UVChoice = -1;
	long marker = 0;
	long event = 0;

	sem.P(RUN_CHILD);
	cout << getpid() <<": now running" << endl;
	if(isUVFree[0]){
		UVChoice = 0;
		isUVFree[0] = false;
		marker = U;
	}
	else if(isUVFree[1]){
		UVChoice = 1;
		isUVFree[1] = false;
		marker = V;
	}
	cout << getpid() << ": marker = " << marker << endl;
	do{
		event = rand();
	}while(event > 100 && event % marker != 0);


	isUVFree[UVChoice] = true;

	cout << getpid() << ": giving up turn" << endl;

	sem.V(RUN_PARENT);
}

void parentProc(pid_t PID[4], SEMAPHORE &sem, int shmid){
	string input;
	sem.P(RUN_PARENT);
	sem.P(RUN_PARENT);
	cout << "Enter !wq if you'd like to exit" << endl;
	getline(cin, input);
	if(input == "!wq"){
		for(int i = 0; i < 4; i++){
			kill(PID[i], 9);
		}
		parentCleanup(sem, shmid);
		cout << "parent exiting..." << endl;
		exit(0);
	}
}

void parentCleanup (SEMAPHORE &sem, int shmid) {
	cout << "deleting shmem" << endl;
	shmctl(shmid, IPC_RMID, NULL);	/* cleaning up */
	cout << "deleting sem" << endl;
	sem.remove();
} // parent_cleanup
