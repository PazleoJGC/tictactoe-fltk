#include<FL/Fl_Widget.H>
#include<FL/fl_ask.H>
#include<FL/Fl_Button.H>
#include<Fl/Fl_Box.H>
#include<FL/Fl_Window.H>
#include<FL/Fl.H>
#include <iostream>
#include <time.h> 
using namespace std;

Fl_Window* gameSelection;
Fl_Window* gameMain;

class boardBase
{
	public:
		//ogolne
		bool isPlayerOne;		//true=player1, false=player2/CPU
		bool isCpuEnabled;
		bool gameEnd;
		int boardSize;			//Total board size
		int	boardDimension;		//Size in one dimension
		int	gameTurn;			//Number of turns since the start of game
		int	borderSize;			//Size of border (in pixels)
		int	buttonSize;			//Size of button (in pixels)
		struct coordinates {
			int row;
			int column;
		};
		coordinates previousMove;
		//FLTK
		Fl_Button* buttons[20][20]; //array of buttons at max capacity
		//metody
		void initGame();
		void drawBoard(int x, int y, int pola);
		void endGame(bool hasWinner);
		virtual void printMe() { cout << "base class " << endl; }

		static void setMode(Fl_Widget* w, void* v);
		static void setMode3(Fl_Widget* w, void* data);
		static void setMode20(Fl_Widget* w, void* data);

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
			borderSize = 20;
			gameEnd = false;
		}
};

//Initialize and show mode selection window
void boardBase::initGame()
{
	gameSelection = new Fl_Window(500, 250, 310, 310, "Main menu");
	Fl_Box* Dbox = new Fl_Box(60, 60, 200, 60, "Choose game mode");
	Dbox->labelsize(25);
	Fl_Button* tryb3 = new Fl_Button(100, 130, 100, 30, "3 x 3");
	Fl_Button* tryb5 = new Fl_Button(100, 170, 100, 30, "20 x 20");
	Fl_Button* toggleCPU = new Fl_Button(20, 20, 60, 20, "toggle CPU");
	Fl_Button* togglePL2 = new Fl_Button(100, 20, 80, 20, "player2 starts");
	Fl_Button* wyjscie = new Fl_Button(100, 230, 100, 30, "exit");
	tryb3->labelsize(20); tryb5->labelsize(20); toggleCPU->labelsize(10); togglePL2->labelsize(10); wyjscie->labelsize(20);
	tryb3->callback(boardBase::setMode3, this); tryb5->callback(boardBase::setMode20, this); toggleCPU->callback(boardBase::toggleCpu, this); togglePL2->callback(boardBase::toggleStartingPlayer, this); wyjscie->callback(closeGame);
	gameSelection->end();
	gameSelection->show();
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
	buttons[0][0]->do_callback();
}

//Exit from the game screen
void boardBase::surrender(Fl_Widget* w, void* data)
{
	boardBase* P = (boardBase*)data;
	if (fl_ask("Do you want to leave the game?"))P->endGame(0);
	else return;

}

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
			boardBase::boardBase(CPU, player);
			boardSize = boardDimension * buttonSize + 2 * borderSize;
		}
};

void size3::checkLine()
{
	bool sprawdzenie = false;
	char* x;
	if (isPlayerOne) { x = (char*)"X"; }
	else x = (char*)"O";

	for (int i = 0; i < boardDimension; i++)
	{
		if (buttons[i][0]->label() == x && buttons[i][1]->label() == x && buttons[i][2]->label() == x) { sprawdzenie = true; break; }
		if (buttons[0][i]->label() == x && buttons[1][i]->label() == x && buttons[2][i]->label() == x) { sprawdzenie = true; break; }
	}
	if (buttons[0][0]->label() == x && buttons[1][1]->label() == x && buttons[2][2]->label() == x) { sprawdzenie = true; }
	if (buttons[0][2]->label() == x && buttons[1][1]->label() == x && buttons[2][0]->label() == x) { sprawdzenie = true; }

	if (sprawdzenie)
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

class size20 : public boardBase
{
public:
	virtual void printMe() { std::cout << "size 20 " << endl; }
	virtual void checkLine();
	virtual void cpuMove(Fl_Widget* w, void* data);
	virtual coordinates checkNeighbours(char* x);

	size20(bool CPU, bool player) :boardBase(CPU, player) {
		boardDimension = 20;
		buttonSize = 30;
		boardBase::boardBase(CPU, player);
		boardSize = boardDimension * buttonSize + 2 * borderSize;
	}
};

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
		if (counter >= highestValue && safeguard) { highestValue = counter; result = temp;}
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
		if (counter > highestValue && safeguard) { highestValue = counter; result = temp;}
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
		if (counter > highestValue && safeguard) { highestValue = counter; result = temp;}
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
		if (counter > highestValue && safeguard) { highestValue = counter; result = temp;}
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
		if (counter > highestValue && safeguard) { highestValue = counter; result = temp;}
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
		if (counter > highestValue && safeguard) { highestValue = counter; result = temp;}
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

//controlling the board
void fCallback(Fl_Widget* w, void* data) {
	boardBase* P = (boardBase*)data;
	if (P->gameEnd) {
		//cleanup
		for (int i = 0; i < P->boardSize - 1; i++)
		{
			for (int j = 0; j < P->boardSize - 1; j++)
			{
				P->buttons[1][1]->label("");
			}
		}
		P->gameTurn = 0;
		gameSelection->show();
		gameMain->hide();
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
			if ((P->gameTurn == 9 && P->boardDimension == 3) || (P->gameTurn == 400 && P->boardDimension == 20)) { P->checkLine(); if (!P->gameEnd) P->endGame(0); }
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

//rysowanie planszy / ustawienia trybów
void boardBase::drawBoard(int x, int y, int wymiar)
{
	cout << " current player" << isPlayerOne << endl;
	int X = x; int Y = x;
	int W = y; int H = y;
	for (int j = 0; j < wymiar; j++)
	{
		for (int i = 0; i < wymiar; i++)
		{
			buttons[i][j] = new Fl_Button(X, Y, W, H, 0);
			buttons[i][j]->callback(fCallback, this);
			X += y;
		}
		X = x; Y += y; //reset x, zwiekszenie y
	}
	Fl_Button* surrenderBtn = new Fl_Button(borderSize, boardSize, 100, 30, "return");
	Fl_Button* toggleCpuBtn = new Fl_Button(2 * borderSize + 100, boardSize, 100, 30, "toggle CPU");
	surrenderBtn->callback(surrender, this);
	toggleCpuBtn->callback(toggleCpu, this);
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
void boardBase::setMode3(Fl_Widget* w, void* data)
{
	boardBase* P = (boardBase*)data;
	P = new size3(P->isCpuEnabled, P->isPlayerOne);
	boardBase::setMode(w, P);
}
void boardBase::setMode20(Fl_Widget* w, void* data)
{
	boardBase* P = (boardBase*)data;
	P = new size20(P->isCpuEnabled, P->isPlayerOne);
	boardBase::setMode(w, P);
}
void boardBase::setMode(Fl_Widget* w, void* data) //wszystko dziala, moze by tak CPU on/off?
{
	boardBase* P = (boardBase*)data;

	gameMain = new Fl_Window(500, 100, P->boardSize, P->boardSize + 60, "game");
	P->drawBoard(P->borderSize, P->buttonSize, P->boardDimension); //rysuje 20[j] od rogu okna
	gameMain->end();
	gameMain->hide();
	gameSelection->hide(); gameMain->show();
}

//Quit game
void boardBase::closeGame(Fl_Widget* w, void* v)
{
	exit(0);
}


int main()
{
	srand(time(NULL));
	boardBase* B = new boardBase(0, 1);
	B->initGame();

	return Fl::run();
	//return 0;
}