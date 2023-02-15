#pragma once
#include "boardBase.h"
#include <iostream>;
using namespace std;

class size3 : public boardBase
{
public:
	virtual void printMe() { cout << "size 3 " << endl; }
	virtual void checkLine();
	virtual void cpuMove(Fl_Widget* w, void* data);
	virtual coordinates checkNeighbours(char* x);
	//virtual coords sprawdz_sasiednie(char* x);
	size3(bool CPU, bool player) :boardBase(CPU, player) {
		boardDimension = 3;
		buttonSize = 90;
		boardSize = boardDimension * buttonSize + 2 * borderSize;
	}
};