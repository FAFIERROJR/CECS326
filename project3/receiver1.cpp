/* 
receiver1.cpp
receives multiples of 251 or 997
terminates when both senders have terminated
*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <cstdio>
#include "msgbuf.h"
using namespace std;

//method declarations
bool checkForDeath(bool& isSender997Alive, bool& isSender251Alive, bool& isReceiver2Alive, long event);
bool areAllDead(bool isSender997Alive, bool isSender251Alive, bool isRcv2Alive);
void death(bool isReceiver1Alive, buf& msg, int qid, int size);

int main() {
	//declare sender existence flags, init to true
	bool isSender997Alive = true;
	bool isSender251Alive = true;
	bool isReceiver2Alive = true;

	//receiving mtype
	long mtype = 1;

	// create my msgQ with key value from ftok()
	int qid = msgget(ftok(".",'u'), IPC_EXCL|IPC_CREAT|0600);

	buf msg;

	//set msg size
	int size = sizeof(msg)-sizeof(long);

	do{
		
		msgrcv(qid, (struct msgbuf *)&msg, size, mtype, 0); // read mesg
		cout << "receiver1"<< ": gets message" << endl;
		cout << "event: " << msg.event << endl;

		long eventReceived = atol(msg.event);

		checkForDeath(isSender997Alive, isSender251Alive, isReceiver2Alive, eventReceived);

		//do not send message if receiving from 251
		//or receiving death signal from receiver 2
		if(eventReceived % 251 == 0 || eventReceived == -2){
			continue;
		}

		msg.mtype = 2;
		msgsnd(qid, (struct msgbuf *)&msg, size, 0);
	}while (isSender997Alive || isSender251Alive);

	death(isReceiver2Alive, msg, qid, size);

	exit(0);
}

/*checkForDeath()
checks to see if one of its senders or receiver 2 died
sets appropriate flag 
returns true if death occured */
bool checkForDeath(bool& isSender997Alive, bool& isSender251Alive, bool& isReceiver2Alive, long event){
	if(event == -3){
		isSender997Alive = false;
		return true;
	}

	if(event == -2){
		isReceiver2Alive = false;
		return true;
	}

	if(event == -1){
		isSender251Alive = false;
		return true;
	}
	return false;
}

/* areAllDead()
returns true if both senders and receiver 2 are terminated */
bool areAllDead(bool isSender997Alive, bool isSender251Alive, bool isRcv2Alive){
	if(!(isSender997Alive || isSender251Alive || isRcv2Alive)){
		cout << "all others have died" << endl;
		return true;
	}
	return false;
}

/*death()
handles death process. destroys queue if safe
otherwise informs receiver 2 of death */
void death(bool isReceiver2Alive, buf& msg, int qid, int size){
	if(isReceiver2Alive){
		msg.mtype = 3;
		long eventNum = -1;
		sprintf(msg.event, "%ld", eventNum);
		msgsnd(qid, (struct msgbuf *)&msg, size, 0);
	}
	else{
		// now safe to delete message queue
		msgctl (qid, IPC_RMID, NULL);
	}
}
