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
#include "msgbuf.h"
using namespace std;

//method declarations
void checkForDeath(bool& isSender997Alive, bool& isSender251Alive, bool& isRcv2Alive, long event);
bool areAllDead(bool isSender997Alive, bool isSender251Alive, bool isRcv2Alive);
void death(bool isReceiver1Alive, buf& msg, int qid, int size);

int main() {
	//declare sender existence flags, init to true
	bool isSender997Alive = true;
	bool isSender251Alive = true;
	bool isRcv2Alive = true;

	//receiving mtype
	long mtype = 1;

	// create my msgQ with key value from ftok()
	int qid = msgget(ftok(".",'u'), IPC_EXCL|IPC_CREAT|0600);

	buf msg;

	//set msg size
	int size = sizeof(msg)-sizeof(long);

	do{
		
		msgrcv(qid, (struct msgbuf *)&msg, size, mtype, 0); // read mesg
		cout << getpid() << ": gets message" << endl;
		cout << "event: " << msg.event << endl;

		if(msg.event == -2 || msg.event == -1){
			continue;
		}

		msg.mtype = 2;
		msgsnd(qid, (struct msgbuf *)&msg, size, 0);
	}while (isSender997Alive || isSender251Alive);

	death(isRcv2Alive, msg, qid, size);

	exit(0);
}


/*checkForDeath()
checks to see if one of its senders or receiver 2 died
sets appropriate flag */
void checkForDeath(bool& isSender997Alive, bool& isSender251Alive, bool& isRcv2Alive, long event){
	if(event < 100 && event >= 0){
		isSender997Alive = false;
	}

	if(event == -2){
		isRcv2Alive = false;
	}

	if(event == -1){
		isSender251Alive = false;
	}
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
		msg.event = -1;
		msgsnd(qid, (struct msgbuf *)&msg, size, 0);
	}
	else{
		// now safe to delete message queue
		msgctl (qid, IPC_RMID, NULL);
	}
}
