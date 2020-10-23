#include "life.h"


// Worker thread code that handles palying of the game
void* worker_thread(void* arg){
	// gets thread name from void* argument
	int thread_number = (long)arg;
	// gets message that indicates the range of rows to manage
	message* range;
	range = range->Recv(thread_number);
	// initalizes the default end of generation message as GENDON
	int type = GENDONE;
	// messages to be sent to thread 0 when a generation is finished
	message* gen_done;
	// message to be sent to thread 0 when this thread is finished 
	message* all_done = new message(thread_number, ALLDONE, -1, -1);
	// flag that indicates whether a all asigned rows are unchanged from one gen to the next
	bool unchanged = true;
	// flag that indicates whether a all asigned rows are empty
	bool empty = true;

	// if the inital message type isn't alldone play game on a specifed set of rows
	if(range->getType() != ALLDONE){
		// gets lower inclusive bound
		int lower_limit = range->getV1();
		// gets upper inclusive bound
		int upper_limit = range->getV2();

		// message to be recieved from thread 0
		message* fromZ;
		// plays game for number of generations set by the user
		for(int i = 1; i<= generations;i++){
			// recieves message from 0, if there are no messages waits until there is one
			fromZ = fromZ->Recv(thread_number);
			// if thread 0 tells the thread to be done it is done
			if(fromZ->getType() == STOP) break;
			// loops through assigned rows
			for(int i = lower_limit; i <= upper_limit;i++){
				// searches each column of designated row
				for(int j = 0; j < old_board->getC(); j++){
					// makes changes necessary to board
					int action = old_board->action(i, j);
					if(action == DEATH) board->set(0, i, j);
					else if(action == SURVIVAL) board->set(1, i, j);
					else if(action == BIRTH) board->set(1,i,j);
					else board->set(old_board->get(i,j), i, j);

				}
				// checks to see if all assigned rows are unchanged or empty
				unchanged = old_board->same_as(board, i) && unchanged;
				empty = board->empty(i) && empty;


				
				
			}
			// if all assigned rows are either unchanged or empty message type is changed
			// to coordinate future of program
			// ie. if all rows are empty the game ends early
			// or
			// if the board is unchanged from one generation to the next
			if(unchanged && empty)type = UNCHANGEDEMPTY;
			else if(unchanged) type = UNCHANGED;
			else if(empty) type = EMPTY;

			// reset flags
			unchanged = true;
			empty = true;

			gen_done = new message(thread_number, type, -1, -1);
			
			gen_done->Send(0);
			type = GENDONE;
			


		}
		// sends message to thread 0 that this thread has completed the game for its rows
		all_done->Send(0);

	} 
	// if an all_done message is recieved initally send an all done message back to thread 0
	else all_done->Send(0);

	
	
}

// arguments: 
// % ./life threads filename generations print input
main(int argc, char* argv[]){

	// number of threads desired by user
	int thread_count = 0;
	
	// name of file
	string filename;
	// whether each generation (including generation 0) should be printed before proceeding to the next generation. The default value is false ("n")
	bool print = false;
	// whether keyboard input should be required before proceeding to the next generation. The default value is false ("n")
	bool input = false;

	// verifies that the minimum number of argumetns is satisified
	// assigns arguments to internal variables and checks each to verify validity
	if(argc > 3){
		thread_count = atoi(argv[1]);
		if(thread_count > MAXTHREAD){
			perror("Invalid Input: Thread Count");
			exit(-1);
		}

		filename = argv[2];
		generations = atoi(argv[3]);
		if(!(generations > 0)){
			perror("Invalid Input: Number of Generations\n");
			exit(-1);
		}

		if(argc >= 5){
			char* p;
			p = argv[4];
			if(p[0] == 'y'){
				print = true;
			}
			if(argc >= 6){
				char* i;
				i = argv[5];
				if(i[0] == 'y'){
					input = true;
				}
			}
		}
		
	}
	else{
		perror("Invalid Input: Argument Count\n");
		exit(-1);
	}

	ifstream file (filename);
	if(!file.is_open()){
		perror("Invalid Input: File\n");
		exit(-1);
	}

	board = new Board();
	old_board = new Board();
	
	int r = 0;
	int c = 0;
	int big_c = 0;
	char line[80];
	for(int i = 0; i < 80; i++){
		line[i] = ' ';
	}
	// reads file and copies its contents onto the program's board accordingly 
	while(file.getline(line, 80)){

		for(int i = 0; i < 80; i++){
			if(line[i] == '1' || line[i] == '0'){
				int put;
				put = atoi(&line[i]);
				old_board->set(put, r, c);
				c++;
			}
			line[i] = ' ';
		}
		if(c > big_c){
			big_c = c;
		}
		c = 0;
		r++;
		
		
	}
	// terminates the program if the biggest column is bigger than 40 the max grid size
	if(big_c > MAXGRID){
		perror("Invalid Input: Board Size\n");
		exit(-1);
	}

	// gives both the current and the last board the correct dimensions
	board->setRC(r, big_c);
	old_board->setRC(r, big_c);

	// closes opened file since it is no longer needed
	file.close();

	sem_t* sem1 = new sem_t();
	sem_t* sem2= new sem_t();
	pthread_t* id;

	// sets up mailbox of thread 0
	// semaphores are used to ensure mutual exclusion among mailboxes
	// ie. prevents multiple threads from accessing it in any way at the same time
	sem_init(sem1, 0, 0);
	RECIEVING[0] = sem1;
	sem_init(sem2, 0, 1);
	SENDING[0] = sem2;

	// if there are more rows than columns than too_many_rows will be true
	// meaning that each thread could be assigned multiple rows
	bool too_many_workers = false;
	if(r > thread_count) too_many_workers = true;

	// calculates the floor of (number of rows) / (number of threads)
	// ie finds a number that distributes the number of rows evenly amongst the threads with
	// some amount of precision lost
	int row_thread_assignment = r / thread_count;

	// regains precision by accounting for values lost in row thread assignment
	int differnce = r - (row_thread_assignment * thread_count);

	// is the next inclusive lower bound of the thread assignment 
	int next =0;

	// the number of threads with rows allocated to them
	int threads_inuse = 0;

	// if the threads name is included then it is not finshed
	// if -1 is included where the threads name should be it is not finished
	// ie. if not_finished(4) = -1; thread 4 is finished
	//     if not_finished(2) = 2; thread 2 is not finished 
	vector<int> not_finished;
	not_finished.push_back(0);

	message* to_send;
	message* done = new message(0, ALLDONE, -1, -1);
	message* go_message = new message(0, GO, -1, -1);
	message* stop = new message(0, STOP, -1, -1);

	for(int i = 1; i <= thread_count; i++){
		sem1 = new sem_t();
		sem2 = new sem_t();
		id = new pthread_t();

		// sets up each thread to have mutual exlcusion
		// ie. prevents multiple threads from accessing their or another threads mailbox at the same time
		sem_init(sem1, 0, 0);
		RECIEVING[i] = sem1;
		sem_init(sem2, 0, 1);
		SENDING[i] = sem2;

		// stores the thread id of newly created threads for later termination
		pthread_create(id, NULL, worker_thread, (void *) i);
		THREADS[i] = id;

		if(too_many_workers){
			// As long as there are more rows to distribute, countinue!
			if(next < r){
				// if precision loss has not been dealt with assign an extra row to the current thread
				if(differnce != 0){
					to_send = new message(0, RANGE, next , next + row_thread_assignment);
					threads_inuse++;
					not_finished.push_back(i);
					next += row_thread_assignment + 1;
					differnce--;
					// sends the thread its range
					to_send->Send(i);
					// tells the thread to go
					go_message->Send(i);
				}
				// other wise assign the thread the orginal calculated row thread assignment	
				else{
					to_send = new message(0, RANGE, next, next + (row_thread_assignment-1) );
					threads_inuse++;
					not_finished.push_back(i);
					next += row_thread_assignment;
					// sends the thread its range
					to_send->Send(i);
					// tells the thread to go
					go_message->Send(i);
				}
			}
			// otherwise the thread is done
			else{
				done->Send(i);
				not_finished.push_back(-1);
			}

		}
		// each thread can be assigned 1 row or some threads are un needed because there are less rows than threads
		else{
			// if there are more rows to be assigned assign them
			if(i <= r){
				to_send = new message(0, RANGE, next, next);
				next++;
				threads_inuse++;
				not_finished.push_back(i);
				// sends the thread its range
				to_send->Send(i);
				// tells the thread to go
				go_message->Send(i);

			}
			// if not tell the thread it is finished
			else{
				done->Send(i);
				not_finished.push_back(-1);

			}


		}


	}

	// prints orginal board
	cout << "Generation: " << 0 << endl;
	old_board->print_board();
	
	// number of threads that are done with a generation
	int threads_finished = 0;

	// number of threads that have terminated and will no longer run code
	int threads_terminated = 0;
	message* a_message;

	// current generation of the game 
	int cur_gen = 0;

	// number of threads where all of their rows are unchanged
	int unchanged = 0;

	// number of threads where all of their rows are empty
	int empty = 0;
	
	// true if the game should be terminated
	bool end = false;
	
	while(true){

		// recieves message from thread 0's mailbox from worker threads
		a_message = a_message->Recv(0);
		// gets the type of message
		int type = a_message->getType();

		// checks to see if the message type is unchanged and empty, empty, or unchanged
		// to determine future of the program
		if(type == UNCHANGEDEMPTY){
			unchanged++;
			empty++;
		}
		else if(type == EMPTY) empty++;
		else if(type == UNCHANGED) unchanged++;

		// if all threads are unchanged or are empty all threads terminate
		if(unchanged == threads_inuse || empty == threads_inuse){
			for(int i = 1; i <= thread_count; i++){
				if(not_finished.at(i) != -1){
					stop->Send(i);
				}
				
			} 
			end = true;
		}
		if( a_message->getType() == ALLDONE){
			threads_terminated++;
			not_finished.at(a_message->getSender()) = -1;

			// if all threads have terminated the game is done
			if(threads_terminated == thread_count){
				break;
			}
		}
		else{
			threads_finished++;
			// if a generation has completed or if the number of generations hasn't exceeded the desired number of generations
			if((threads_finished == threads_inuse) && (cur_gen < generations)){
				unchanged = 0; 
				empty = 0;
				// prompt user for input
				if(input){
					cout << "Press enter to countinue to the next generation...";	
					string garbage;
					getline(cin, garbage);


				}
				cur_gen++;
				// print board and generation number
				if(print){
					cout << "Generation: " << cur_gen << endl;
					board->print_board();
				}
				// prints last generation
				if( (cur_gen == generations && !print) || (end && !print) ){
					cout << "Generation: " << cur_gen << endl;
					board->print_board();

				}

				// if the game has not been terminated 
				if(!end){
					// reset boards
					board->copy_wipe(old_board);
					// tell all worker threads they can go
					for(int i = 1; i <= thread_count; i++){
						if(not_finished.at(i) != -1){
							go_message->Send(i);
						}
						
					} 
					threads_finished = 0;	
				}	

			}
		}	
	}

	cout << "The game has ended after " << cur_gen << " generations" << endl;

	// terminates all threads and semaphores
	for(int i = 1; i <= thread_count; i++){
		pthread_join(*THREADS[i], NULL);
		sem_destroy(RECIEVING[i]);
		sem_destroy(SENDING[i]);
	}

}

