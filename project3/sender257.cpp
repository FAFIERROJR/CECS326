/* 
sender257.cpp
This is the sender with marker values divisible by 997
must receive ack
terminates when after it sends a num < 100
*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include "msgbuf.h"
using namespace std;

//method declarations
long generateRandomNumber(int marker);

int main() {
	//declare existence flag
	bool isReceiver2Alive = true; //assume receiver 2 is alive

	long eventNum = 0;

	//set mtypes
	long recMtype = 4;
	long sendMtype = 3;
	long sender = 257;

	//seed srand
	srand(time(NULL));
	int marker = 257;

	int qid = msgget(ftok(".",'u'), 0);


	//set msg size
	int size = sizeof(msg)-sizeof(long) * 2;

	while(isReceiver2Alive){
		
		eventNum = generateRandomNumber(marker);

		// sending event msg
		msg.mtype = sendMtype;
		msg.sender = sender;
		sprintf(msg.event, "%ld", eventNum);
		msgsnd(qid, (struct msgbuf *)&msg, size, 0);
		cout << "sender257: " << "event sent" << endl;

		//checking existence
		msgrcv(qid, (struct msgbuf *)&msg, size, recMtype, 0); // reading
  		long eventReceived = atol(msg.event);

		if(eventReceived == -2){
			isReceiver2Alive = false;
		}

	}

	cout << getpid() << ": now exits" << endl;

	exit(0);
}

/*generateRandomNumber()
generate random number until divisible by marker
or exit condition */
long generateRandomNumber(int marker){
	long event;
	while(event % marker != 0){
		event = rand();
	}
	return event;	

}


