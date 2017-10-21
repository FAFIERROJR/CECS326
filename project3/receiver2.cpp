/* 
receiver2.cpp
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

	//set mtype
	long mtype = 3;

	//init message received counter
	int msgCount = 0;

	int qid = msgget(ftok(".",'u'), 0);

	// declare my message buffer
	struct buf {
		long mtype; 
		char event[50];
	};


	buf msg;

	//set msg size
	int size = sizeof(msg)-sizeof(long);

	do{
		msgrcv(qid, (struct msgbuf *)&msg, size, mtype, 0); // read mesg
		cout << getpid() << ": gets message" << endl;
		cout << "event: " << msg.event << endl;
		msgCount++;

		if(atoi(msg.event) < 100){
			isSender997Alive = false;
		}


		if((isSender997Alive && (msgCount == 4999)) || msgCount == 5000){
			cout << "I'm dying" << endl;
			strcpy(msg.event, "iDedNow");
		}

		msg.mtype = 4;
		msgsnd(qid, (struct msgbuf *)&msg, size, 0);
	}while (msgCount < 5000);


	//inform rcv1 of impending death
	msg.mtype = 4;
	strcpy(msg.event, "iDedNow");
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);

	cout << "iDedNow" << endl;
	exit(0);
}
