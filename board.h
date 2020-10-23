#ifndef BOARD_H_
#define BOARD_H_

#include <vector>
#include <iostream>

#define DEATH 0
#define SURVIVAL 1
#define BIRTH -1
#define NOTHING -2

using namespace std;

extern struct Board* board;
extern struct Board* old_board;

class Board{
	
	public:
		Board();

		int getR();
		int getC();
		void setRC(int, int);
		int get(int r, int c);
		void set(int v, int r, int c);
		void print_board();
		void copy_wipe(Board*);
		bool has_org(int r, int c);
		int action(int r, int c);
		bool same_as(Board* compare, int i);
		bool empty(int i);

	private:
		int nrow, ncol;
		vector<vector <int>> game_board;


};



#endif /* BOARD_H_ */
