#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <semaphore.h>
#include <iostream>

#define RANGE 1
#define ALLDONE 2
#define GO 3
#define GENDONE 4 // Generation Done
#define EMPTY 5
#define UNCHANGED 6
#define UNCHANGEDEMPTY 7
#define STOP 8

using namespace std;


extern struct message* MAILBOX[11];
extern sem_t* SENDING[11];
extern sem_t* RECIEVING[11];


class message{
	
	public:
		message(int, int, int, int);
		//friend ostream& operator<<(ostream& os, const message& a);
		int getSender();
		int getType();
		int getV1();
		int getV2();
		void Send(int);
		message* Recv(int);

	private:
		int iSender, type, value1, value2;

};

#endif /* MESSAGE_H_ */
