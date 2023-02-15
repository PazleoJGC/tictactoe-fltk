#pragma once
#include <FL/Fl_Button.H>
#include "gameState.h"
#include <iostream>

class boardBase
{
public:
	struct coordinates {
		int row;
		int column;
	};
	//general
	bool isPlayerOne;		//true=player1, false=player2/CPU
	bool isCpuEnabled;
	bool gameEnd;
	int boardSize;			//Total board size
	int	boardDimension;		//Size in one dimension
	int	gameTurn;			//Number of turns since the start of game
	int	borderSize;			//Size of border (in pixels)
	int	buttonSize;			//Size of button (in pixels)
	gameState* game;
	coordinates previousMove;
	//FLTK
	Fl_Button* buttons[20][20]; //array of buttons at max capacity
	Fl_Button* surrenderButton;
	Fl_Button* toggleCpuButton;
	//methods
	void initGame(gameState* game);
	void drawBoard(int x, int y, int pola);
	void endGame(bool hasWinner);
	virtual void printMe() { std::cout << "base class " << std::endl; }

	static void startMode(Fl_Widget* w, void* v);
	static void startMode3(Fl_Widget* w, void* data);
	static void startMode20(Fl_Widget* w, void* data);

	static void toggleCpu(Fl_Widget* w, void* v);
	static void toggleStartingPlayer(Fl_Widget* w, void* data);
	static void closeGame(Fl_Widget* w, void* v);
	static void surrender(Fl_Widget* w, void* data);

	virtual void cpuMove(Fl_Widget* w, void* data) {};
	virtual void checkLine() {};
	friend void fCallback(Fl_Widget* w, void* v);
	virtual coordinates checkNeighbours(char* x)
	{
		coordinates a;
		return a;
	};

	boardBase(bool CPU, bool player)
	{
		gameTurn = 0;
		isPlayerOne = player;
		isCpuEnabled = CPU;
		boardDimension = 0;
		borderSize = 20;
		gameEnd = false;
	}

	~boardBase() {
		for (int i = 0; i < this->boardDimension; i++)
		{
			for (int j = 0; j < this->boardDimension; j++)
			{
				delete this->buttons[i][j];
			}
		}
		delete this->game->gameMain;
	}
};