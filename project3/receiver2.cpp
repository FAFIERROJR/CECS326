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
#include <cstdio>
#include "msgbuf.h"
using namespace std;

//method declarations
void death(bool isReceiver1Alive, buf& msg, int qid, int size);
bool checkForDeath(bool& isSender997Alive, bool& isRcv1Alive, long event);

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
	int size = sizeof(msg)-sizeof(long) * 2;

	do{
		msgrcv(qid, (struct msgbuf *)&msg, size, recMtype, 0); // read mesg
		cout << "receiver2"<< ": gets message" << endl;
		cout << "event: " << msg.event<< endl;
		msgCount++;

		long eventReceived = atol(msg.event);

		checkForDeath(isSender997Alive,isReceiver1Alive, eventReceived);

		//do not count receiver1's death signal as message
		//do not reply to receiver 1
		if(eventReceived == -1){
			msgCount -= 1;
			continue;
		}

		//last iteration or two. Warn receiver(s) of impending death
		if((isSender997Alive && (msgCount == 4999)) || msgCount == 5000){
			cout << "I'm dying" << endl;
			long eventNum = -2;
			sprintf(msg.event, "%ld", eventNum);
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
bool checkForDeath(bool& isSender997Alive, bool& isReceiver1Alive, long event){
	if(event == -3){
		isSender997Alive = false;
		return true;
	}

	if(event == -1){
		isReceiver1Alive= false;
		return true;
	}
	return false;
}

/*death()
handles death process. destroys queue if safe
otherwise informs receiver 1 of death */
void death(bool isReceiver1Alive, buf& msg, int qid, int size){
	if(isReceiver1Alive){
		msg.mtype = 1;
		long eventNum = -2;
		sprintf(msg.event, "%ld", eventNum);
		msgsnd(qid, (struct msgbuf *)&msg, size, 0);
	}
	else{
		// now safe to delete message queue
		msgctl (qid, IPC_RMID, NULL);
	}
}