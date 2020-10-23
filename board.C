#include "board.h"

// Board constructor
// each board has a
//	nrow: number of rows in the board
//	ncol: number of columns in the board
//	game_board: a 2d vector of integers where 
//			1: represents a living organism
//			0: represents a dead organism
//		       -1: represents an invalid space
Board::Board(){
	// marks number of rows as unset
	nrow = -1;
	// marks  number of columns as unset
	ncol = -1;
	for(int i = 0; i < 40; i++){
		vector<int> hold;

		for(int i = 0; i < 40; i++){
			hold.push_back(-1);
		}

		game_board.push_back(hold);
	}
}

// gets the number of rows in the board
int Board :: getR(){
	return nrow;
}

// gets the number of columns in the board
int Board :: getC(){
	return ncol;
}

// sets the number of rows and columns in a board
void Board :: setRC(int r, int c){
	nrow = r;
	ncol = c;

}

// gets the value at a specified row and column
int Board :: get(int r, int c){
	return game_board.at(r).at(c);
}

// sets the value at a given row and column to a value v
void Board :: set(int v, int r, int c){
	game_board.at(r).at(c) = v;
}


// prints the board representing
//	1: living organisms
//	0: dead organisms
//	~: invalid space
void Board :: print_board(){
	for(int i = 0; i < nrow; i++){

		for(int j = 0; j < ncol; j++){
			if(game_board.at(i).at(j) < 0) cout << '~' << ' ';
			else cout << get(i,j) << ' ';
			
		}
		
		cout << endl;

	}
	cout << endl;

}
// moves the contents of this to paste, while wiping this 
void Board :: copy_wipe(Board* paste){
	for(int i = 0; i < nrow; i++){
		for(int j = 0; j < ncol; j++){
			paste->set(this->get(i,j), i, j);
			this->set(0, i, j);
		}
		
		

	}

}

// checks to see if there is an organism in the given row and column
bool Board :: has_org(int r, int c){
	// verifies row and column is within the range of the board
	if(r > (nrow-1) || r < 0) return false;
	if(c > (ncol-1) || c < 0) return false;
	// if the space is invalid or there is a dead organism it is unoccupied
	if(get(r,c) <= 0) return false;

	// otherwise the space must be occupied
	return true;
	
	

}

// Calculates number of surrounding organisms to return appropriate action 
//|---------------------------|
//|r-1,c-1 | r,c-1  | r+1,c-1 |
//|---------------------------|
//|r-1,c   | r,c    | r+1,c   |
//|---------------------------|
//|r+1,c+1 | r, c+1 | r+1,c+1 |
//|---------------------------|
int Board :: action(int r, int c){
	int org_surround = 0;
	for(int i = 0; i < 8; i++){
		switch(i){
			case 0: 
				if(has_org(r-1, c-1)) org_surround++;
				break;
			case 1: 
				if(has_org(r, c-1)) org_surround++;
				break;
			case 2: 
				if(has_org(r+1, c-1)) org_surround++;
				break;
			case 3: 
				if(has_org(r-1, c)) org_surround++;
				break;
			case 4: 
				if(has_org(r+1, c)) org_surround++;
				break;
			case 5: 
				if(has_org(r-1, c+1)) org_surround++;
				break;
			case 6: 
				if(has_org(r, c+1)) org_surround++;
				break;
			case 7: 
				if(has_org(r+1, c+1)) org_surround++;
				break;
			default:
				perror("Logical Error in action(int,int): 1");
				exit(-1);

		}

	}

	// if there are 0,1,4,5,6,7, or 8 surrounding organisms the current organisms dies
	if( has_org(r,c) && (org_surround == 0 || org_surround == 1 || (org_surround >= 4)) ) return DEATH;
	// if there are 2 or 3 surrounding organisms the current organism survives to the next round
	else if( has_org(r,c) && (org_surround == 2 || org_surround == 3)) return SURVIVAL;
	// if there is no organism in the current square, and is surrounded by 3 organisms a new organism is spawned on the current spot 
	else if( !has_org(r,c) && org_surround == 3) return BIRTH;
	// otherwise the space should remain the same
	else return NOTHING;
	
}

// checks to see if this is the same as compare for a given row i
bool Board :: same_as(Board* compare, int i){
	bool result = true;

	for(int j = 0; j < ncol; j++){
		result = result && (this->get(i,j) == compare->get(i,j));
		if(!result) break;
		
	}
	return result;

}

// checks this to see if a given row i is empty
bool Board :: empty(int i){
	bool result = true;
	for(int j = 0; j < ncol; j++){
		result = result && !(this->get(i,j) == 1);
		if(!result) break;

	}

	return result;

}

