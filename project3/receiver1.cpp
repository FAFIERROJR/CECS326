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

int main() {
	//declare sender existence flags, init to true
	bool isSender997Alive = true;
	bool isSender251Alive = true;

	long mtype = 1;

	// create my msgQ with key value from ftok()
	int qid = msgget(ftok(".",'u'), IPC_EXCL|IPC_CREAT|0600);

	// declare my message buffer
	struct buf {
		long mtype; 
		char event[50];
	};


	buf msg;
	//set this sender's id
	strcpy(msg.id, "receiver1");
	//set this sender's mtype;
	msg.mtype = mtype;

	//set msg size
	int size = sizeof(msg)-sizeof(long);

	do{
		msgrcv(qid, (struct msgbuf *)&msg, size, mtype, 0); // read mesg
		cout << getpid() << ": gets message" << endl;
		cout << "event: " << msg.event << endl;

		msgsnd(qid, (struct msgbuf *)&msg, size, mtype, 0);
	}while (isSender997Alive || isSender251Alive);

	//clear potential remaining 251 message
	msgrcv(qid (struct msgbuf *) &msg), size, 
	// now safe to delete message queue
	msgctl (qid, IPC_RMID, NULL);

	exit(0);
}
