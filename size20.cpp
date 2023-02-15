#include "boardBase.h"
#include "size20.h"

boardBase::coordinates size20::checkNeighbours(char* x)
{
	coordinates result, temp;
	result.row = 9; result.column = 9;
	int counter = 0, highestValue = 0;
	bool safeguard = false;
	cout << x << endl;
	//columns
	for (int i = 0; i < boardDimension; i++)
	{
		for (int j = 0; j < boardDimension; j++)
		{
			if (buttons[i][j]->label() == x) { counter += 1; }
			if (j != 0 && j != boardDimension - 1 && (buttons[i][j - 1]->label() == x || buttons[i][j + 1]->label() == x)) { if (buttons[i][j]->label() == 0) { temp.row = i; temp.column = j; safeguard = true; } }
		}
		if (counter >= highestValue && safeguard) { highestValue = counter; result = temp; }
		counter = 0; safeguard = false;
	}
	//rows
	for (int i = 0; i < boardDimension; i++)
	{
		for (int j = 0; j < boardDimension; j++)
		{
			if (buttons[j][i]->label() == x) { counter += 1; }
			if (j != 0 && j != boardDimension - 1 && (buttons[j - 1][i]->label() == x || buttons[j + 1][i]->label() == x)) { if (buttons[j][i]->label() == 0) { temp.row = j; temp.column = i; safeguard = true; } }
		}
		if (counter > highestValue && safeguard) { highestValue = counter; result = temp; }
		counter = 0; safeguard = false;
	}


	//diagonal 1\2
	for (int j = 0; j < boardDimension; j++)
	{
		for (int i = 0; i < boardDimension; i++) {
			if (i + j < boardDimension) {
				if (buttons[i + j][i]->label() == x) { counter += 1; }
				if (i + j < 19 && i + j > 0 && i < 19 && i>0 && (buttons[i + j + 1][i + 1]->label() == x || buttons[i + j - 1][i - 1]->label() == x)) { if (buttons[i + j][i]->label() == 0) { temp.row = i + j; temp.column = i; safeguard = true; } }
			}
		}
		if (counter > highestValue && safeguard) { highestValue = counter; result = temp; }
		counter = 0; safeguard = false;
	}


	//diagonal 2\2
	for (int j = 0; j < boardDimension; j++)
	{
		for (int i = 0; i < boardDimension; i++) {
			if (i - j >= 0) {
				if (buttons[i - j][i]->label() == x) { counter += 1; }
				if (i - j < 19 && i - j > 0 && i < 19 && i>0 && (buttons[i - j + 1][i + 1]->label() == x || buttons[i - j - 1][i - 1]->label() == x)) { if (buttons[i - j][i]->label() == 0) { temp.row = i - j; temp.column = i; safeguard = true; } }
			}
		}
		if (counter > highestValue && safeguard) { highestValue = counter; result = temp; }
		counter = 0; safeguard = false;
	}
	//diagonal 1/2
	for (int j = 0; j < boardDimension; j++)
	{
		for (int i = 0; i < boardDimension; i++) {
			if (i + j < boardDimension) {
				if (buttons[i + j][boardDimension - 1 - i]->label() == x) { counter += 1; }
				if (i + j < 19 && i + j > 0 && i < 19 && i>0 && (buttons[i + j + 1][boardDimension - 1 - i - 1]->label() == x || buttons[i + j - 1][boardDimension - 1 - i + 1]->label() == x)) { if (buttons[i + j][boardDimension - 1 - i]->label() == 0) { temp.row = i + j; temp.column = boardDimension - 1 - i; safeguard = true; } }
			}
		}
		if (counter > highestValue && safeguard) { highestValue = counter; result = temp; }
		counter = 0; safeguard = false;
	}
	//diagonal 2/2
	for (int j = 0; j < boardDimension; j++)
	{
		for (int i = 0; i < boardDimension; i++) {
			if (i - j >= 0) {
				if (buttons[i - j][boardDimension - 1 - i]->label() == x) { counter += 1; }
				if (i - j < 19 && i - j > 0 && i < 19 && i>0 && (buttons[i - j + 1][boardDimension - 1 - i - 1]->label() == x || buttons[i - j - 1][boardDimension - 1 - i + 1]->label() == x)) { if (buttons[i - j][boardDimension - 1 - i]->label() == 0) { temp.row = i - j; temp.column = boardDimension - 1 - i; safeguard = true; } }
			}
		}
		if (counter > highestValue && safeguard) { highestValue = counter; result = temp; }
		counter = 0; safeguard = false;
	}

	if (highestValue >= 0) return result;
	cout << "fail" << endl;
	result.row = -1; result.column = -1;
	return result;

}

//check victory conditions
void size20::checkLine()
{
	bool checked = false;
	char* x;

	if (isPlayerOne) { x = (char*)"X"; }
	else x = (char*)"O";

	for (int i = 0; i < boardDimension; i++)
	{
		for (int j = 0; j + 4 < boardDimension; j++) {
			if (buttons[i][j]->label() == x && buttons[i][j + 1]->label() == x && buttons[i][j + 2]->label() == x && buttons[i][j + 3]->label() == x && buttons[i][j + 4]->label() == x) { checked = true; break; }
			if (buttons[j][i]->label() == x && buttons[j + 1][i]->label() == x && buttons[j + 2][i]->label() == x && buttons[j + 3][i]->label() == x && buttons[j + 4][i]->label() == x) { checked = true; break; }
		}
		if (checked)break;
	}

	//diagonal 1
	for (int i = 0; i < boardDimension; i++)
	{
		for (int j = 0; j < boardDimension; j++) {
			if (i + j + 4 < boardDimension) {
				if (buttons[i + j][i]->label() == x && buttons[i + j + 1][i + 1]->label() == x && buttons[i + j + 2][i + 2]->label() == x && buttons[i + j + 3][i + 3]->label() == x && buttons[i + j + 4][i + 4]->label() == x) { checked = true; break; }
			}
			if (i - j - 4 >= 0) {
				if (buttons[i - j][i]->label() == x && buttons[i - j - 1][i - 1]->label() == x && buttons[i - j - 2][i - 2]->label() == x && buttons[i - j - 3][i - 3]->label() == x && buttons[i - j - 4][i - 4]->label() == x) { checked = true; break; }
			}
		}
		if (checked)break;
	}

	//diagonal 2
	for (int i = 0; i < boardDimension / 2; i++)
	{
		for (int k = boardDimension - 1; k >= 0; k--)
		{
			for (int j = 0; j < boardDimension / 2; j++)
			{
				if (i + j + 4 <= boardDimension && k - 4 >= 0) {
					if (buttons[i + j][k]->label() == x && buttons[i + j + 1][k - 1]->label() == x && buttons[i + j + 2][k - 2]->label() == x && buttons[i + j + 3][k - 3]->label() == x && buttons[i + j + 4][k - 4]->label() == x) { checked = true; break; }
				}
				if (i - j >= 0 && k - 4 >= 0) {
					if (buttons[i - j][k]->label() == x && buttons[i - j + 1][k - 1]->label() == x && buttons[i - j + 2][k - 2]->label() == x && buttons[i - j + 3][k - 3]->label() == x && buttons[i - j + 4][k - 4]->label() == x) { checked = true; break; }
				}
			}
		}
		if (checked)break;
	}

	if (checked)
	{
		endGame(1);
	}
}

//perform move as CPU
void size20::cpuMove(Fl_Widget* w, void* data)
{
	if (gameTurn == 0) { w = buttons[rand() % boardDimension][rand() % boardDimension]; }
	while (w->label() != 0)
	{
		if (gameTurn < 2) {
			w = buttons[rand() % boardDimension][rand() % boardDimension];
		}

		coordinates c;
		c = checkNeighbours((char*)"X");
		//block player from winning
		if (c.row != -1) {
			w = buttons[c.row][c.column];
			if (w->label() == 0) { cout << "blocked"; cout << " x: " << c.row << " y: " << c.column << endl;  break; }
		}
		c = checkNeighbours((char*)"O");
		//win with player
		if (c.row != -1) {
			w = buttons[c.row][c.column];
			if (w->label() == 0) { cout << "eliminated"; cout << " x: " << c.row << " y: " << c.column << endl;  break; }
		}

		//choose field at random if failed
		while (w->label() != 0)
		{
			w = buttons[rand() % boardDimension][rand() % boardDimension];
			cout << "chosen at random";
		}
		break;
	}
	if (isPlayerOne) w->label("X");
	else w->label("O");
	w->labelsize(30);
	checkLine();
	isPlayerOne = !isPlayerOne;
	gameTurn++;
	cout << endl << "turn:" << gameTurn << endl;
}
