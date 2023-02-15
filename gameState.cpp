#include "gameState.h"
#include "boardBase.h"
#include <iostream>
using namespace std;

//controlling the board
void gameState::fCallback(Fl_Widget* w, void* data) {
	boardBase* P = (boardBase*)data;
	if (P->gameEnd) {
		EndGame:
		P->game->gameSelection->show();
		P->game->gameMain->hide();
		delete P;
		return;
	}
	if (w->label() != "X" && w->label() != "O")
	{
		if (P->isPlayerOne || !P->isCpuEnabled)
		{
			if (P->isPlayerOne) w->label("X");
			else w->label("O");
			w->labelsize(30);
			P->gameTurn++;
			if ((P->gameTurn == 9 && P->boardDimension == 3) || (P->gameTurn == 400 && P->boardDimension == 20))
			{
				P->checkLine();
				if (!P->gameEnd) {
					P->endGame(0);
					return;
				}
			}
			cout << "turn: " << P->gameTurn << endl;
			P->checkLine();
			P->isPlayerOne = !P->isPlayerOne;
		}
		//CPU move
		if (P->isCpuEnabled && !P->gameEnd) {
			P->cpuMove(w, data);
			if ((P->gameTurn == 9 && !P->gameEnd && P->boardDimension == 3) || (P->gameTurn == 25 && !P->gameEnd && P->boardDimension == 5)) { P->checkLine(); if (!P->gameEnd) P->endGame(0); }
		}

	}
}