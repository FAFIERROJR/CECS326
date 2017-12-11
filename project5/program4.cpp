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

enum{RUN_CHILD, GET_TICKET, REDEEM_TICKET, SET_U, SET_V};
enum{NOW_SERVING, DISPENSER};
enum{IS_U_FREE, IS_V_FREE};

void childProc(SEMAPHORE &sem, bool *isUVFree, int * ticketSystem, int ticket);
void parentProc(pid_t PID[4], SEMAPHORE &sem, int shmid1, int shmid2);
void parentCleanup(SEMAPHORE &sem, int shmid1, int shmid2);
void getTicket(int &ticket, int* ticketSystem);
bool redeemTicket(int &ticket, int *ticketSystem);

int main(){
	srand(time(NULL));
	pid_t PID[4] = {0, 0, 0, 0};
	int count = 0;
	SEMAPHORE sem(5);
	int boolshmid;
	bool *boolshmBUF;
	int * ticketSystem;
	int ticketshmid;
	int ticket = -1;

	sem.V(RUN_CHILD);
	sem.V(RUN_CHILD);
	sem.V(GET_TICKET);
	sem.V(REDEEM_TICKET);
	sem.V(SET_U);
	sem.V(SET_V);

	boolshmid = shmget(IPC_PRIVATE, BUFFSIZE*sizeof(bool), PERMS);
	boolshmBUF = (bool *)shmat(boolshmid, 0, SHM_RND);
	boolshmBUF[IS_U_FREE] = true;
	boolshmBUF[IS_V_FREE] = true;

	ticketshmid = shmget(IPC_PRIVATE, BUFFSIZE * sizeof(int), PERMS);
	ticketSystem = (int *)shmat(ticketshmid, 0, SHM_RND);
	ticketSystem[NOW_SERVING] = 0;
	cout << "NOW SERVING: " << ticketSystem[NOW_SERVING] << endl;
	ticketSystem[DISPENSER] = 0;

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
			parentProc(PID, sem, boolshmid, ticketshmid);
		}
		else{
			childProc(sem, boolshmBUF, ticketSystem, ticket);
		}
	}
}

void childProc(SEMAPHORE &sem, bool *isUVFree, int * ticketSystem, int ticket){
	int UVChoice = -1;
	long marker = 0;
	long event = 0;
	bool isMyTurn = false;

	sem.P(GET_TICKET);
	getTicket(ticket, ticketSystem);
	sem.V(GET_TICKET);

	sem.P(RUN_CHILD);
	sem.P(REDEEM_TICKET);
	isMyTurn = redeemTicket(ticket, ticketSystem);
	sem.V(REDEEM_TICKET);
	if(isMyTurn){
		cout << getpid() <<": now running" << endl;
		if(isUVFree[IS_U_FREE]){
			UVChoice = 0;
			sem.P(SET_U);
			isUVFree[IS_U_FREE] = false;
			sem.V(SET_U);
			marker = U;
		}

		else if(isUVFree[IS_V_FREE]){
			UVChoice = 1;
			sem.P(SET_V);
			isUVFree[IS_V_FREE] = false;
			sem.V(SET_V);
			marker = V;
		}

		cout << getpid() << ": marker = " << marker << endl;

		do{
			event = rand();
		}while(event > 100 && event % marker != 0);


		if(UVChoice == 0){
			sem.P(SET_U);
			isUVFree[UVChoice] = true;
			sem.V(SET_U);
		}else if(UVChoice == 1){
			sem.P(SET_V);
			isUVFree[UVChoice] = true;
			sem.V(SET_V);
		}

		cout << getpid() << ": giving up turn" << endl;
	}
	sem.V(RUN_CHILD);
}

void parentProc(pid_t PID[4], SEMAPHORE &sem, int shmid1, int shmid2){
	string input;
	cin >> input;
	cin.clear();
	if(input == "!wq"){
		for(int i = 0; i < 4; i++){
			kill(PID[i], 9);
		}
		parentCleanup(sem, shmid1, shmid2);
		cout << "parent exiting..." << endl;
		exit(0);
	}
}

void parentCleanup (SEMAPHORE &sem, int shmid1, int shmid2) {
	cout << "deleting shmem" << endl;
	shmctl(shmid1, IPC_RMID, NULL);	/* cleaning up */
	shmctl(shmid2, IPC_RMID, NULL);	/* cleaning up */
	cout << "deleting sem" << endl;
	sem.remove();
} // parent_cleanup


//getTicket will assign ticket to a process
void getTicket(int &ticket, int* ticketSystem){
	if(ticket == -1){
		ticket = ticketSystem[DISPENSER];
		cout << getpid() << ": MYTICKET = " << ticket << endl;;
		ticketSystem[DISPENSER] = (ticketSystem[DISPENSER] + 1) % 4;
	}
}

//redeemTicket will allow child process to run
//if their ticket is next
//otherwise it will send them back to the line
//but will let them keep their ticket
bool redeemTicket(int &ticket, int* ticketSystem){
	if(ticket == ticketSystem[NOW_SERVING]){
		ticket = -1;
		ticketSystem[NOW_SERVING] = (ticketSystem[NOW_SERVING] + 1) % 4;
		cout << "NOW SERVING: " << ticketSystem[NOW_SERVING] << endl;
		return true;
	}
	else{
		return false;
	}
}