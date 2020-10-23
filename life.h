#ifndef LIFE_H_
#define LIFE_H_

#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <fstream>
#include <string>
#include <vector>
#include "board.h"
#include "message.h"

#define RANGE 1
#define ALLDONE 2
#define GO 3
#define GENDONE 4 // Generation Done
#define EMPTY 5
#define UNCHANGED 6
#define UNCHANGEDEMPTY 7
#define STOP 8

#define DEATH 0
#define SURVIVAL 1
#define BIRTH -1
#define NOTHING -2

using namespace std;

int MAXGRID = 40;
int MAXTHREAD = 10;
int generations = 0;

struct message* MAILBOX[11];
sem_t* SENDING[11];
sem_t* RECIEVING[11];
pthread_t* THREADS[11];

struct Board* board;
struct Board* old_board;

#endif /* LIFE_H_ */
