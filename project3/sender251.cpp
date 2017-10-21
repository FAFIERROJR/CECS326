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
int generateRandomNumber(int marker);

int main() {
	//declare existence flag
	bool isAlive;

	char * event;
	long eventNum = 0;

	//seed srand
	srand(time(NULL));
	int marker = 251;

	int qid = msgget(ftok(".",'u'), 0);

	// declare my message buffer
	struct buf {
		long mtype; 
		char event[50];
	};

	buf msg;

	//set this sender's mtype;
	msg.mtype = 1;

	//set msg size
	int size = sizeof(msg)-sizeof(long);

	while(true){
		do{
		
			eventNum = generateRandomNumber(marker);
			event = (char *) &eventNum;

			// sending event msg
			strcpy(msg.event, event);
			msgsnd(qid, (struct msgbuf *)&msg, size, 0);
			cout << "sender251: " << "event sent" << endl;

 
		}while(true);

	}

	cout << getpid() << ": now exits" << endl;

	exit(0);
}

/*generateRandomNumber()
generate random number until divisible by marker
or exit condition */
int generateRandomNumber(int marker){
	long event;
	while(event % marker != 0){
		event = rand();
	}
	return event;	

}


