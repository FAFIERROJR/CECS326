/* 
sender251.cpp
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
#include "get_info.h"
#include "msgbuf.h"
using namespace std;

//method declarations
long generateRandomNumber(int marker);

int main() {
	//declare existence flag
	bool isAlive;

	char * event;
	long eventNum = 0;

	//receiving mtype
	long sendMtype = 1;
	long recMtype = 2;

	//seed srand
	srand(time(NULL));
	int marker = 251;

	int qid = msgget(ftok(".",'u'), 0);

	buf msg;

	//set msg size
	int size = sizeof(msg)-sizeof(long);

	//send death signal
	msg.mtype = 1;
	msg.event = -1;
	get_info(qid, (struct msgbuf *)&msg, size, 0);

	while(true){
		
		eventNum = generateRandomNumber(marker);
		event = (char *) &eventNum;

		// sending event msg
		msg.mtype = sendMtype;
		msg.event = eventNum;
		msgsnd(qid, (struct msgbuf *)&msg, size, 0);
		cout << "sender251: " << "event sent" << endl;

		//checking existence
		msgrcv(qid, (struct msgbuf *)&msg, size, recMtype, 0); // reading
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


