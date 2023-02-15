#pragma once
#include "boardBase.h"
#include <iostream>;
using namespace std;

class size20 : public boardBase
{
public:
	virtual void printMe() { cout << "size 20 " << endl; }
	virtual void checkLine();
	virtual void cpuMove(Fl_Widget* w, void* data);
	virtual coordinates checkNeighbours(char* x);

	size20(bool CPU, bool player) :boardBase(CPU, player) {
		boardDimension = 20;
		buttonSize = 30;
		boardSize = boardDimension * buttonSize + 2 * borderSize;
	}
};
