/* 
sender997.cpp
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
long detMtype(bool isAlive[2], bool didRcv[2]);
long generateRandomNumber(int marker);
bool didAllReceive(bool isAlive[2], bool didRcv[2]);
int setAliveFlag(bool isAlive[2], long mtype);
int setRcvFlag(bool didRcv[2], long mtype);

int main() {
	//declare existence flags
	bool isAlive[2] = {true, true};
	//declare ack flags
	bool didRcv[2];

	long mtype;

	char * event;
	long eventNum = 0;

	//seed srand
	srand(time(NULL));
	int marker = 997;

	long sender = 997;

	int qid = msgget(ftok(".",'u'), 0);

	buf msg;

	//set msg size
	int size = sizeof(msg) - sizeof(long) * 2;

	while(true){
		//reset flags
		didRcv[0] = false;
		didRcv[1] = false;

		//find new event
		eventNum = generateRandomNumber(marker);
		if(eventNum < 100){
				eventNum = -3;
		}

		do{
		

			//determine whether to send to receiver 1 or receiver2
			mtype = detMtype(isAlive, didRcv);
			//set sender
			msg.sender = sender;
			cout << "determined mtype " << mtype << endl;
			msg.mtype = mtype;

			// sending event msg
			sprintf(msg.event, "%ld", eventNum);
			msgsnd(qid, (struct msgbuf *)&msg, size, 0);
			cout << "sender997: " << "event sent" << endl;

			//receiving ack
			mtype = msg.mtype + 1;
			cout << "mtype " << mtype << endl;
			msgrcv(qid, (struct msgbuf *)&msg, size, mtype, 0); // reading
			cout << "sender997" << ": ack received" << endl;
			cout << "event recipient: " << "receiver " << mtype/2 << endl;
			long eventReceived = atol(msg.event);

			if(eventReceived == -2){
				setAliveFlag(isAlive, mtype);
				cout << "sender2 died" << endl;
			}

			//flipping appropriate flag
			setRcvFlag(didRcv, mtype);

		//until both flags are flipped
		}while(!didAllReceive(isAlive, didRcv));
		//if event was less than 100
		//break loop
		if(eventNum < 100){
			break;
		}
	}

	cout << getpid() << ": now exits" << endl;

	exit(0);
}


/*detMtype()
determines appropriate mtype
depending on which receiver needs to receive the message */
long detMtype(bool isAlive[2], bool didRcv[2]){
	if(isAlive[0] && !didRcv[0]){
		return 1;
	}
	else if(isAlive[1] && !didRcv[1]){
		return 3;
	}
}

/*generateRandomNumber()
generate random number until divisible by marker
or exit condition */
long generateRandomNumber(int marker){
	long event;
	while(event % marker != 0){
		event = rand();

		//exit condition met
		if(event < 100){
			break;
		}

	}
	return event;	

}

/*didAllReceive()
returns true if all existing receivers have received msg
else returns false */
bool didAllReceive(bool isAlive[2], bool didRcv[2]){
	if((isAlive[0] && didRcv[0]) || !isAlive[0]){
		if((isAlive[1] && didRcv[1]) || !isAlive[1]){
			cout << "all received" << endl;
			return true;
		}
	}
	return false;
}

/*setAliveFlag()
sets appropriate existence flag to false */
int setAliveFlag(bool isAlive[2], long mtype){
	if(mtype == 2){
		isAlive[0]= false;
	}
	else if (mtype == 4){
		isAlive[1] = false;
	}
}

/*setRcvFlag()
sets appropriate reception flag to true */
int setRcvFlag(bool didRcv[2], long mtype){
	if(mtype == 2){
		didRcv[0]= true;
	}
	else{
		didRcv[1] = true;
	}

}