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
#include "msgbuf.h"
using namespace std;

//method declarations
void death(bool isReceiver1Alive, buf& msg, int qid, int size);
void checkForDeath(bool& isSender997Alive, bool& isRcv1Alive, long event);

int main() {
	//declare sender existence flags, init to true
	bool isSender997Alive = true;
	bool isReceiver1Alive= true;

	//set mtype
	long recMtype = 3;
	long sendMtype = 4;

	//init message received counter
	int msgCount = 0;

	int qid = msgget(ftok(".",'u'), 0);

	buf msg;

	//set msg size
	int size = sizeof(msg)-sizeof(long);

	do{
		msgrcv(qid, (struct msgbuf *)&msg, size, recMtype, 0); // read mesg
		cout << getpid() << ": gets message" << endl;
		cout << "event: " << msg.event<< endl;
		msgCount++;

		checkForDeath(isSender997Alive,isReceiver1Alive, msg.event);

		//last iteration or two. Warn receiver(s) of impending death
		if((isSender997Alive && (msgCount == 4999)) || msgCount == 5000){
			cout << "I'm dying" << endl;
			msg.event = -2;
		}

		msg.mtype = sendMtype;
		msgsnd(qid, (struct msgbuf *)&msg, size, 0);
	}while (msgCount < 5000);


	cout << "iDedNow" << endl;

	death(isReceiver1Alive, msg, qid, size);
	exit(0);
}

/*checkForDeath()
checks to see if one of its senders or receiver 2 died
sets appropriate flag */
void checkForDeath(bool& isSender997Alive, bool& isReceiver1Alive, long event){
	if(event < 100 && event >= 0){
		isSender997Alive = false;
	}

	if(event == -1){
		isReceiver1Alive= false;
	}
}

/*death()
handles death process. destroys queue if safe
otherwise informs receiver 1 of death */
void death(bool isReceiver1Alive, buf& msg, int qid, int size){
	if(isReceiver1Alive){
		msg.mtype = 1;
		msg.event = -2;
		msgsnd(qid, (struct msgbuf *)&msg, size, 0);
	}
	else{
		// now safe to delete message queue
		msgctl (qid, IPC_RMID, NULL);
	}
}