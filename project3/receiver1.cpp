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
using namespace std;

//method declarations
bool areAllDead(bool isSender997Alive, bool isSender251Alive, bool isRcv2Alive);

int main() {
	//declare sender existence flags, init to true
	bool isSender997Alive = true;
	bool isSender251Alive = true;
	bool isRcv2Alive = true;

	//receiving mtype
	long mtype = 1;

	// create my msgQ with key value from ftok()
	int qid = msgget(ftok(".",'u'), IPC_EXCL|IPC_CREAT|0600);

	// declare my message buffer
	struct buf {
		long mtype; 
		long event;
	};


	buf msg;

	//set msg size
	int size = sizeof(msg)-sizeof(long);

	do{
		
		msgrcv(qid, (struct msgbuf *)&msg, size, mtype, 0); // read mesg
		cout << getpid() << ": gets message" << endl;
		cout << "event: " << msg.event << endl;

		if(msg.event < 100){
			isSender997Alive = false;
		}

		if(msg.event == -2){
			isRcv2Alive = false;
		}

		if(areAllDead(isSender997Alive, isSender251Alive, isRcv2Alive)){
			break;
		}

		msg.mtype = 2;
		msgsnd(qid, (struct msgbuf *)&msg, size, 0);
	}while (isSender997Alive || isSender251Alive);

	// now safe to delete message queue
	msgctl (qid, IPC_RMID, NULL);

	exit(0);
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
