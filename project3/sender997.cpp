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
#include <cstdlib>
using namespace std;

//method declarations
long detMtype(bool isAlive[2], bool didRcv[2]);
int generateRandomNumber(int marker);
bool didAllReceive(bool isAlive[2], bool didRcv[2]);
int setAliveFlag(bool isAlive[2], long mtype);
int setRcvFlag(bool didRcv[2], long mtype);

int main() {
	//declare existence flags
	bool isAlive[2] = {true, true};
	//declare ack flags
	bool didRcv[2];

	char * event;
	long mtype;
	long eventNum = 0;

	//seed srand
	srand(time(NULL));
	int marker = 997;

	int qid = msgget(ftok(".",'u'), 0);

	// declare my message buffer
	struct buf {
		long mtype; 
		char event[50];
	};

	buf msg;

	//set msg size
	int size = sizeof(msg) - sizeof(long);

	while(true){
		//reset flags
		didRcv[0] = false;
		didRcv[1] = false;
		do{
		
			eventNum = generateRandomNumber(marker);
			event = (char *) &eventNum;

			mtype = detMtype(isAlive, didRcv);
			msg.mtype = mtype;

			// sending event msg
			strcpy(msg.event, event);
			msgsnd(qid, (struct msgbuf *)&msg, size, 0);
			cout << "sender997: " << "event sent" << endl;

			//receiving ack
			msgrcv(qid, (struct msgbuf *)&msg, size, 9970, 0); // reading
			cout << "sender997" << ": ack received" << endl;
			cout << "event recipient: " << "receiver " << (int) msg.mtype;

			if(strcmp(msg.event, "death")){
				setAliveFlag(isAlive, mtype);
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
	else{
		return 2;
	}
}

/*generateRandomNumber()
generate random number until divisible by marker
or exit condition */
int generateRandomNumber(int marker){
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
	if((isAlive[0] * didRcv[0] + !isAlive[0]) && (isAlive[1] * didRcv[1] + !isAlive[1])){
		return true;
	}
	return false;
}

/*setAliveFlag()
sets appropriate existence flag to false */
int setAliveFlag(bool isAlive[2], long mtype){
	if(mtype == 1){
		isAlive[0]= true;
	}
	else{
		isAlive[1] = true;
	}
}

/*setRcvFlag()
sets appropriate reception flag to true */
int setRcvFlag(bool didRcv[2], long mtype){
	if(mtype == 1){
		didRcv[0]= true;
	}
	else{
		didRcv[1] = true;
	}

}