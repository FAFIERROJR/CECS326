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
long generateRandomNumber(int marker);

int main() {
	//declare existence flag
	bool isAlive;

	char * event;
	long eventNum = 0;

	//receiving mtype
	long mtype = 4;

	//seed srand
	srand(time(NULL));
	int marker = 257;

	int qid = msgget(ftok(".",'u'), 0);

	// declare my message buffer
	struct buf {
		long mtype; 
		long event;
	};

	buf msg;

	//set msg size
	int size = sizeof(msg)-sizeof(long);

	while(true){
		do{
		
			eventNum = generateRandomNumber(marker);
			event = (char *) &eventNum;

			// sending event msg
			msg.mtype = 3;
			msg.event = eventNum;
			msgsnd(qid, (struct msgbuf *)&msg, size, 0);
			cout << "sender257: " << "event sent" << endl;

			//checking existence
			msgrcv(qid, (struct msgbuf *)&msg, size, mtype, 0); // reading
  

			if(msg.event == -2){
				break;
			}

		}while(true);

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


