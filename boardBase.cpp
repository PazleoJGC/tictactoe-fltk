#include<FL/fl_ask.H>
#include<Fl/Fl_Box.H>
#include<FL/Fl_Button.H>
#include<FL/Fl_Window.H>
#include <iostream>
#include "boardBase.h"
#include "gameState.h"
#include "size3.h"
#include "size20.h"
using namespace std;

//Initialize and show mode selection window
void boardBase::initGame(gameState* gs)
{
	game = gs;
	game->gameSelection = new Fl_Window(500, 250, 310, 310, "Main menu");
	Fl_Box* Dbox = new Fl_Box(60, 60, 200, 60, "Choose game mode");
	Dbox->labelsize(25);
	Fl_Button* mode3 = new Fl_Button(100, 130, 100, 30, "3 x 3");
	Fl_Button* mode20 = new Fl_Button(100, 170, 100, 30, "20 x 20");
	Fl_Button* toggleCPU = new Fl_Button(20, 20, 60, 20, "toggle CPU");
	Fl_Button* togglePL2 = new Fl_Button(100, 20, 80, 20, "player2 starts");
	Fl_Button* exit = new Fl_Button(100, 230, 100, 30, "exit");
	mode3->labelsize(20);
	mode3->callback(boardBase::startMode3, this);
	mode20->labelsize(20);
	mode20->callback(boardBase::startMode20, this);
	toggleCPU->labelsize(10);
	toggleCPU->callback(boardBase::toggleCpu, this);
	togglePL2->labelsize(10);
	togglePL2->callback(boardBase::toggleStartingPlayer, this);
	exit->labelsize(20);
	exit->callback(closeGame);
	game->gameSelection->end();
	game->gameSelection->show();
}

void boardBase::startMode3(Fl_Widget* w, void* data)
{
	boardBase* temp = (boardBase*)data;
	boardBase* P = new size3(temp->isCpuEnabled, temp->isPlayerOne);
	P->game = temp->game;
	boardBase::startMode(w, P);
}

void boardBase::startMode20(Fl_Widget* w, void* data)
{
	boardBase* temp = (boardBase*)data;
	boardBase* P = new size20(temp->isCpuEnabled, temp->isPlayerOne);
	P->game = temp->game;
	boardBase::startMode(w, P);
}

void boardBase::startMode(Fl_Widget* w, void* data)
{
	boardBase* P = (boardBase*)data;
	P->game->gameMain = new Fl_Window(500, 100, P->boardSize, P->boardSize + 60, "game");
	P->drawBoard(P->borderSize, P->buttonSize, P->boardDimension);
	P->game->gameMain->end();
	P->game->gameMain->hide();
	P->game->gameSelection->hide();
	P->game->gameMain->show();
}

void boardBase::drawBoard(int x, int y, int dimension)
{
	cout << "player " << isPlayerOne << " starts." << endl;
	int X = x; int Y = x;
	int W = y; int H = y;
	for (int j = 0; j < dimension; j++)
	{
		for (int i = 0; i < dimension; i++)
		{
			buttons[i][j] = new Fl_Button(X, Y, W, H, 0);
			buttons[i][j]->callback(game->fCallback, this);
			X += y;
		}
		X = x; Y += y;
	}
	this->surrenderButton = new Fl_Button(borderSize, boardSize, 100, 30, "return");
	this->toggleCpuButton = new Fl_Button(2 * borderSize + 100, boardSize, 100, 30, "toggle CPU");
	this->surrenderButton->callback(surrender, this);
	this->toggleCpuButton->callback(toggleCpu, this);
	if (!isPlayerOne && isCpuEnabled) buttons[0][0]->do_callback();
}

void boardBase::toggleCpu(Fl_Widget* w, void* data)
{
	Fl_Button* W = (Fl_Button*)w;
	boardBase* P = (boardBase*)data;
	P->isCpuEnabled = !P->isCpuEnabled;
	if (P->isCpuEnabled) { cout << " CPU On "; W->value(1); }
	else { cout << " CPU Off "; W->value(0); }
	if (P->gameTurn > 0 && !P->isPlayerOne) P->buttons[rand() % P->boardDimension][rand() % P->boardDimension]->do_callback();
}

void boardBase::toggleStartingPlayer(Fl_Widget* w, void* data)
{
	Fl_Button* W = (Fl_Button*)w;
	boardBase* P = (boardBase*)data;
	P->isPlayerOne = !P->isPlayerOne;
	if (P->isPlayerOne) { cout << " player 1 starts "; W->value(0); }
	else { cout << " player 2 starts "; W->value(1); }
}

//Select and display winner
void boardBase::endGame(bool hasWinner)
{
	if (hasWinner)
	{
		if (isPlayerOne) fl_message("player 1 won!");
		else fl_message("player 2 won!");
	}
	else fl_message("draw");
	gameEnd = true;
	buttons[0][0]->do_callback(); //refresh gamestate
}

//Exit from the game screen
void boardBase::surrender(Fl_Widget* w, void* data)
{
	boardBase* P = (boardBase*)data;
	if (fl_ask("Do you want to leave the game?"))P->endGame(0);
	else return;

}

//Quit game
void boardBase::closeGame(Fl_Widget* w, void* v)
{
	exit(0);
}
