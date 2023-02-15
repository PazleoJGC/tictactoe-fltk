#include "boardBase.h"
#include "size3.h"

void size3::checkLine()
{
	bool checked = false;
	char* x;
	if (isPlayerOne) { x = (char*)"X"; }
	else x = (char*)"O";

	for (int i = 0; i < boardDimension; i++)
	{
		if (buttons[i][0]->label() == x && buttons[i][1]->label() == x && buttons[i][2]->label() == x) { checked = true; break; }
		if (buttons[0][i]->label() == x && buttons[1][i]->label() == x && buttons[2][i]->label() == x) { checked = true; break; }
	}
	if (buttons[0][0]->label() == x && buttons[1][1]->label() == x && buttons[2][2]->label() == x) { checked = true; }
	if (buttons[0][2]->label() == x && buttons[1][1]->label() == x && buttons[2][0]->label() == x) { checked = true; }

	if (checked)
	{
		endGame(1);
	}
}

void size3::cpuMove(Fl_Widget* w, void* data)
{
	coordinates c; c.row = 0; c.column = 0;
	if (gameTurn == 1) { w = buttons[1][1]; }
	while (w->label() != 0 || gameTurn == 0)
	{
		//capture corners in the 3 first turns
		if (gameTurn == 0 || gameTurn == 1 || gameTurn == 2)
		{
			if (gameTurn == 2 && buttons[1][1]->label() != 0)
			{
				if (previousMove.row == 0)
				{
					if (previousMove.column == 0) { c.row = 2; c.column = 2; }
					else { c.row = 2; c.column = 0; }
				}
				else if (previousMove.row == 2)
				{
					if (previousMove.column == 0) { c.row = 0; c.column = 2; }
					else { c.row = 0; c.column = 0; }
				}
			}
			else do {
				c.row = rand() % boardDimension;
				c.column = rand() % boardDimension;
				w = buttons[c.row][c.column];
			} while (w->label() != 0 || ((c.row + c.column) % 2 != 0) || w == buttons[1][1]);

			w = buttons[c.row][c.column];
			break;
		}


		//win with player
		c = checkNeighbours((char*)"O");
		if (c.row != -1) {
			w = buttons[c.row][c.column];
			if (w->label() == 0) { cout << "eliminated"; cout << " x: " << c.row << " y: " << c.column << endl;  break; }
		}
		//block player from winning
		c = checkNeighbours((char*)"X");
		if (c.row != -1) {
			w = buttons[c.row][c.column];
			if (w->label() == 0) { cout << "blocked"; cout << " x: " << c.row << " y: " << c.column << endl;  break; }
		}

		//capture past turn 3
		if (gameTurn == 3 && buttons[1][1]->label() == (char*)"O")
		{
			do {
				c.row = rand() % boardDimension;
				c.column = rand() % boardDimension;
				w = buttons[c.row][c.column];
			} while (w->label() != 0 || ((c.row + c.column) % 2 == 0) && w != buttons[1][1]);

			w = buttons[c.row][c.column];
			break;
		}

		//choose at random if failed
		do {
			c.row = rand() % boardDimension;
			c.column = rand() % boardDimension;
			w = buttons[c.row][c.column];
		} while (w->label() != 0);

		cout << "random " << c.row << " " << c.column;
		break;
	}
	previousMove.row = c.row;
	previousMove.column = c.column;
	if (isPlayerOne) w->label("X");
	else w->label("O");
	w->labelsize(30);
	checkLine();
	isPlayerOne = !isPlayerOne;
	gameTurn++;
	cout << "turn: " << gameTurn << endl;
}

boardBase::coordinates size3::checkNeighbours(char* x)
{
	coordinates result;
	result.row = -1; result.column = -1;
	for (int i = 0; i <= 2; i++)
	{
		//up -> down
		if (buttons[i][0]->label() == x && buttons[i][1]->label() == x && buttons[i][2]->label() == 0) { result.row = i; result.column = 2; cout << " t1 " << i << endl; return result; }
		if (buttons[0][i]->label() == x && buttons[1][i]->label() == x && buttons[2][i]->label() == 0) { result.row = 2; result.column = i; cout << " t2 " << i << endl; return result; }
		//down -> up
		if (buttons[i][2]->label() == x && buttons[i][1]->label() == x && buttons[i][0]->label() == 0) { result.row = i; result.column = 0;  cout << " t3 " << i << endl; return result; }
		if (buttons[2][i]->label() == x && buttons[1][i]->label() == x && buttons[0][i]->label() == 0) { result.row = 0; result.column = i;  cout << " t3b " << i << endl; return result; }
		//with gap
		if (buttons[i][0]->label() == x && buttons[i][2]->label() == x && buttons[i][1]->label() == 0) { result.row = i; result.column = 1; cout << " t9 " << i << endl; return result; }
		if (buttons[0][i]->label() == x && buttons[2][i]->label() == x && buttons[1][i]->label() == 0) { result.row = 1; result.column = i; cout << " t10 " << i << endl; return result; }
	}

	//diagonal
	if (buttons[0][0]->label() == x && buttons[1][1]->label() == x && buttons[2][2]->label() == 0) { result.row = 2; result.column = 2; cout << " t5 " << endl; return result; }
	if (buttons[0][0]->label() == x && buttons[2][2]->label() == x && buttons[1][1]->label() == 0) { result.row = 1; result.column = 1; cout << " t5b " << endl; return result; }
	if (buttons[0][2]->label() == x && buttons[1][1]->label() == x && buttons[2][0]->label() == 0) { result.row = 2; result.column = 0; cout << " t6 " << endl; return result; }
	if (buttons[0][2]->label() == x && buttons[2][0]->label() == x && buttons[1][1]->label() == 0) { result.row = 1; result.column = 1; cout << " t6b " << endl; return result; }
	if (buttons[1][1]->label() == x && buttons[2][2]->label() == x && buttons[0][0]->label() == 0) { result.row = 0; result.column = 0; cout << " t7 " << endl; return result; }
	if (buttons[1][1]->label() == x && buttons[2][0]->label() == x && buttons[0][2]->label() == 0) { result.row = 0; result.column = 2; cout << " t8 " << endl; return result; }
	//special
	if (buttons[0][1]->label() == x && buttons[1][0]->label() == x && buttons[0][0]->label() == 0) { result.row = 0; result.column = 0; cout << " t11 " << endl; return result; }
	if (buttons[2][1]->label() == x && buttons[1][0]->label() == x && buttons[2][0]->label() == 0) { result.row = 2; result.column = 0; cout << " t12 " << endl; return result; }
	if (buttons[0][1]->label() == x && buttons[1][2]->label() == x && buttons[0][2]->label() == 0) { result.row = 0; result.column = 2; cout << " t13 " << endl; return result; }
	if (buttons[1][2]->label() == x && buttons[2][1]->label() == x && buttons[2][2]->label() == 0) { result.row = 2; result.column = 2; cout << " t14 " << endl; return result; }
	return result;
}
