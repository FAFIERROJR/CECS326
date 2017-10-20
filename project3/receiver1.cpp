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
	bool isSenter251Alive = true;

	// create my msgQ with key value from ftok()
	int qid = msgget(ftok(".",'u'), IPC_EXCL|IPC_CREAT|0600);

	// declare my message buffer
	struct buf {
		long mtype; 
		char id[10];
		char event[50];
	};


	buf msg;
	//set this sender's id
	strcpy(msg.id, "receiver1");
	//set this sender's mtype;
	msg.mtype = 997;

	//set msg size
	int size = sizeof(msg)-sizeof(long) - sizeof(msg.id);

	do{
		if(isSender997Alive){
			msgrcv(qid, (struct msgbuf *)&msg, size, 997, 0); // read mesg
			cout << getpid() << ": gets message" << endl;
			cout << "event: " << msg.event << endl;

		}
							// don't read "fake" mesg
		cout << getpid() << ": gets message" << endl;
		cout << "message: " << msg.greeting << endl;
		
		strcat(msg.greeting, " and Adios.");
		cout << getpid() << ": sends reply" << endl;
		msg.mtype = 314; // only reading mesg with type mtype = 314
		msgsnd(qid, (struct msgbuf *)&msg, size, 0);
		cout << getpid() << ": now exits" << endl;

		msgrcv (qid, (struct msgbuf *)&msg, size, -112, 0);
		msgrcv (qid, (struct msgbuf *)&msg, size, 0, 0);
		msgrcv (qid, (struct msgbuf *)&msg, size, 117, 0);

	//clear potential remaining 251 message
	msgrcv(qid (struct msgbuf *) &msg), size, 
	// now safe to delete message queue
	msgctl (qid, IPC_RMID, NULL);

	exit(0);
}
