#include "message.h"


// message constructor
// each message has a 
//	iSender: who sent the message
// 	type: can either be 
//		RANGE which indicates a thread has been given a range to sum
//		ALLDONE indicates the thread is now alldone and ready to be summed
//	value1: inclusive lower bound of sum
//	value2: inclusive upper bound of sum
message :: message(int s, int t, int v1, int v2){
	iSender = s;
	type = t;
	value1 = v1;
	value2 = v2;
}


// returns the message's sender
int message :: getSender(){
	return iSender;
}

// returns the message's type
int message :: getType(){
	return type;
}

// returns message's value1
int message :: getV1(){
	return value1;
}

// returns the message's value2
int message :: getV2(){
	return value2;
}

// places a message into thread number iTo's mailbox
// uses mutual exclusion to prevent multiple threads sending a message at the
// same time
// General Idea: once a message arives in a threads mailbox it can now recieve
void message :: Send(int iTo){
	sem_wait(SENDING[iTo]);
	MAILBOX[iTo] = this;
	sem_post(RECIEVING[iTo]);

}

// Recieves a message from iFrom's mailbox and returns that message
// Follows the same general idea as Send(int)
message* message :: Recv(int iFrom){
	sem_wait(RECIEVING[iFrom]);
	message* payload;
	payload = MAILBOX[iFrom];
	MAILBOX[iFrom] = NULL;
	sem_post(SENDING[iFrom]);
	return payload;

}

/*
// overloads << operator to make printing messages possible
ostream& message :: operator<<(ostream& os, const message& a){
	string sType = "RANGE";
	if(a.type == 2) sType = "ALLDONE";
	os << "This message is from " << a.iSender << "\nType: "
			<< sType << "\nValue 1: " << a.value1 << "\nValue 2: "
			<< a.value2 << endl;
	return os;

}
*/

