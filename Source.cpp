#include<FL/Fl_Widget.H>
#include<FL/fl_ask.H>
#include<FL/Fl_Button.H>
#include<Fl/Fl_Box.H>
#include<FL/Fl_Window.H>
#include<FL/Fl.H>
#include <iostream>
#include <time.h> 
#include "boardBase.h"
#include "gameState.h"
using namespace std;

int main()
{
	srand(time(NULL));
	gameState* g = new gameState();
	boardBase* B = new boardBase(0, 1);
	B->initGame(g);

	return Fl::run();
	//return 0;
}