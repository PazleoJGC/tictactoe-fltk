#include<FL/Fl_Widget.H>
#include<FL/fl_ask.H>
#include<FL/Fl_Button.H>
#include<Fl/Fl_Box.H>
#include<FL/Fl_Window.H>
#include<FL/Fl.H>
#include <iostream>
#include <time.h> 
using namespace std;

Fl_Window* plansza;
Fl_Window* tryb;

class pole
{
public:
	//ogolne
	bool gracz, komputer, end; //true=gracz1, false=gracz2/CPU
	int rozmiar, tura, wymiar, ramka, przycisk;
	struct coords {
		int row;
		int collumn;
		int max;
	};
	coords ostatniRuch;
	//FLTK
	Fl_Button* przyciski[20][20]; //niezaleznie od trybu
	//metody
	void szkielet_gry();
	void rysuj_plansze(int x, int y, int pola);
	void KoniecGry(bool zwyciezca);
	virtual void wypiszMnie() { cout << "klasa bazowa " << endl; }

	static void ustawtryb(Fl_Widget* w, void* v);
	static void ustawtryb3(Fl_Widget* w, void* data);
	static void ustawtryb5(Fl_Widget* w, void* data);

	static void skynetOn(Fl_Widget* w, void* v);
	static void gracz2pierwszy(Fl_Widget* w, void* data);
	static void close(Fl_Widget* w, void* v);
	static void poddajSie(Fl_Widget* w, void* data);

	virtual void skynet(Fl_Widget* w, void* data) {};
	friend void fCallback(Fl_Widget* w, void* v);
	virtual void sprawdz_linie() {};
	virtual coords sprawdz_sasiednie(char* x)
	{
		coords a;
		return a;
	};
	pole(bool CPU, bool player)
	{
		tura = 0;
		gracz = player;
		komputer = CPU;
		ramka = 20;
		end = false;
	}
};

class na3 : public pole
{
public:
	virtual void wypiszMnie() { cout << "klasa na3 " << endl; }
	virtual void sprawdz_linie();
	virtual void skynet(Fl_Widget* w, void* data);
	virtual coords sprawdz_sasiednie(char* x);
	//virtual coords sprawdz_sasiednie(char* x);
	na3(bool CPU, bool player) :pole(CPU, player) {
		wymiar = 3;
		przycisk = 90;
		pole::pole(CPU, player);
		rozmiar = wymiar * przycisk + 2 * ramka;
	}
};

class na5 : public pole
{
public:
	virtual void wypiszMnie() { std::cout << "klasa na5 " << endl; }
	virtual void sprawdz_linie();
	virtual void skynet(Fl_Widget* w, void* data);
	virtual coords sprawdz_sasiednie(char* x);
	//virtual coords sprawdz_sasiednie(char* x);
	na5(bool CPU, bool player) :pole(CPU, player) {
		wymiar = 20;
		przycisk = 30;
		pole::pole(CPU, player);
		rozmiar = wymiar * przycisk + 2 * ramka;
	}
};
//start menu glownego
void pole::szkielet_gry()
{
	tryb = new Fl_Window(500, 250, 310, 310, "Menu Glowne");
	Fl_Box* Dbox = new Fl_Box(60, 60, 200, 60, "Wybierz tryb gry");
	Dbox->labelsize(25);
	Fl_Button* tryb3 = new Fl_Button(100, 130, 100, 30, "3 x 3");
	Fl_Button* tryb5 = new Fl_Button(100, 170, 100, 30, "20 x 20");
	Fl_Button* toggleCPU = new Fl_Button(20, 20, 60, 20, "komputer");
	Fl_Button* togglePL2 = new Fl_Button(100, 20, 80, 20, "gracz 2 zaczyna");
	Fl_Button* wyjscie = new Fl_Button(100, 230, 100, 30, "wyjscie");
	tryb3->labelsize(20); tryb5->labelsize(20); toggleCPU->labelsize(10); togglePL2->labelsize(10); wyjscie->labelsize(20);
	tryb3->callback(pole::ustawtryb3, this); tryb5->callback(pole::ustawtryb5, this); toggleCPU->callback(pole::skynetOn, this); togglePL2->callback(pole::gracz2pierwszy, this); wyjscie->callback(close);
	tryb->end();
	tryb->show();
}
//informacja/restart
void pole::KoniecGry(bool zwyciezca)
{
	if (zwyciezca)
	{
		if (gracz) fl_message("wygral czlowiek");
		else fl_message("wygral gracz2/komputer");
	}
	else fl_message("remis");
	end = true;
	przyciski[0][0]->do_callback();
}
//wyjscie z poziomu planszy
void pole::poddajSie(Fl_Widget* w, void* data)
{
	pole* P = (pole*)data;
	if (fl_ask("Czy na pewno chcesz zakonczyc gre?"))P->KoniecGry(0);
	else return;

}
//ruchy komputera
pole::coords na3::sprawdz_sasiednie(char* x)
{
	coords a;
	a.row = -1; a.collumn = -1;
	for (int i = 0; i <= 2; i++)
	{
		//z gory na dol
		if (przyciski[i][0]->label() == x && przyciski[i][1]->label() == x && przyciski[i][2]->label() == 0) { a.row = i; a.collumn = 2; cout << " t1 " << i << endl; return a; }
		if (przyciski[0][i]->label() == x && przyciski[1][i]->label() == x && przyciski[2][i]->label() == 0) { a.row = 2; a.collumn = i; cout << " t2 " << i << endl; return a; }
		//z dolu na gore
		if (przyciski[i][2]->label() == x && przyciski[i][1]->label() == x && przyciski[i][0]->label() == 0) { a.row = i; a.collumn = 0;  cout << " t3 " << i << endl; return a; }
		if (przyciski[2][i]->label() == x && przyciski[1][i]->label() == x && przyciski[0][i]->label() == 0) { a.row = 0; a.collumn = i;  cout << " t3b " << i << endl; return a; }
		//z przerwa pomiedzy
		if (przyciski[i][0]->label() == x && przyciski[i][2]->label() == x && przyciski[i][1]->label() == 0) { a.row = i; a.collumn = 1; cout << " t9 " << i << endl; return a; }
		if (przyciski[0][i]->label() == x && przyciski[2][i]->label() == x && przyciski[1][i]->label() == 0) { a.row = 1; a.collumn = i; cout << " t10 " << i << endl; return a; }
	}

	//ukosne
	if (przyciski[0][0]->label() == x && przyciski[1][1]->label() == x && przyciski[2][2]->label() == 0) { a.row = 2; a.collumn = 2; cout << " t5 " << endl; return a; }
	if (przyciski[0][0]->label() == x && przyciski[2][2]->label() == x && przyciski[1][1]->label() == 0) { a.row = 1; a.collumn = 1; cout << " t5b " << endl; return a; }
	if (przyciski[0][2]->label() == x && przyciski[1][1]->label() == x && przyciski[2][0]->label() == 0) { a.row = 2; a.collumn = 0; cout << " t6 " << endl; return a; }
	if (przyciski[0][2]->label() == x && przyciski[2][0]->label() == x && przyciski[1][1]->label() == 0) { a.row = 1; a.collumn = 1; cout << " t6b " << endl; return a; }
	if (przyciski[1][1]->label() == x && przyciski[2][2]->label() == x && przyciski[0][0]->label() == 0) { a.row = 0; a.collumn = 0; cout << " t7 " << endl; return a; }
	if (przyciski[1][1]->label() == x && przyciski[2][0]->label() == x && przyciski[0][2]->label() == 0) { a.row = 0; a.collumn = 2; cout << " t8 " << endl; return a; }
	//specjalne
	if (przyciski[0][1]->label() == x && przyciski[1][0]->label() == x && przyciski[0][0]->label() == 0) { a.row = 0; a.collumn = 0; cout << " t11 " << endl; return a; }
	if (przyciski[2][1]->label() == x && przyciski[1][0]->label() == x && przyciski[2][0]->label() == 0) { a.row = 2; a.collumn = 0; cout << " t12 " << endl; return a; }
	if (przyciski[0][1]->label() == x && przyciski[1][2]->label() == x && przyciski[0][2]->label() == 0) { a.row = 0; a.collumn = 2; cout << " t13 " << endl; return a; }
	if (przyciski[1][2]->label() == x && przyciski[2][1]->label() == x && przyciski[2][2]->label() == 0) { a.row = 2; a.collumn = 2; cout << " t14 " << endl; return a; }
	return a;
}
pole::coords na5::sprawdz_sasiednie(char* x)
{	//Juz DZIAŁA RZĄD 0 UwU
	coords a, temp;
	a.row = 9; a.collumn = 9; a.max = 0;
	int licznik = 0, licznikmax = 0;
	bool bezpiecznik = false;
	cout << x << endl;
	//kolumny
	for (int i = 0; i < wymiar; i++)
	{
		for (int j = 0; j < wymiar; j++)
		{
			if (przyciski[i][j]->label() == x) { licznik += 1; }
			if (j != 0 && j != wymiar - 1 && (przyciski[i][j - 1]->label() == x || przyciski[i][j + 1]->label() == x)) { if (przyciski[i][j]->label() == 0) { temp.row = i; temp.collumn = j; bezpiecznik = true; } }
		}
		if (licznik >= licznikmax && bezpiecznik) { licznikmax = licznik; a = temp; a.max = licznikmax; }
		licznik = 0; bezpiecznik = false;
	}
	//rzedy
	for (int i = 0; i < wymiar; i++)
	{
		for (int j = 0; j < wymiar; j++)
		{
			if (przyciski[j][i]->label() == x) { licznik += 1; }
			if (j != 0 && j != wymiar - 1 && (przyciski[j - 1][i]->label() == x || przyciski[j + 1][i]->label() == x)) { if (przyciski[j][i]->label() == 0) { temp.row = j; temp.collumn = i; bezpiecznik = true; } }
		}
		if (licznik > licznikmax && bezpiecznik) { licznikmax = licznik; a = temp; a.max = licznikmax; }
		licznik = 0; bezpiecznik = false;
	}


	//przekatna 1\2
	for (int j = 0; j < wymiar; j++)
	{
		for (int i = 0; i < wymiar; i++) {
			if (i + j < wymiar) {
				if (przyciski[i + j][i]->label() == x) { licznik += 1; }
				if (i + j < 19 && i + j > 0 && i < 19 && i>0 && (przyciski[i + j + 1][i + 1]->label() == x || przyciski[i + j - 1][i - 1]->label() == x)) { if (przyciski[i + j][i]->label() == 0) { temp.row = i + j; temp.collumn = i; bezpiecznik = true; } }
			}
		}
		if (licznik > licznikmax && bezpiecznik) { licznikmax = licznik; a = temp; a.max = licznikmax; }
		licznik = 0; bezpiecznik = false;
	}


	//przekatna 2\2
	for (int j = 0; j < wymiar; j++)
	{
		for (int i = 0; i < wymiar; i++) {
			if (i - j >= 0) { //dziala
				if (przyciski[i - j][i]->label() == x) { licznik += 1; }
				if (i - j < 19 && i - j > 0 && i < 19 && i>0 && (przyciski[i - j + 1][i + 1]->label() == x || przyciski[i - j - 1][i - 1]->label() == x)) { if (przyciski[i - j][i]->label() == 0) { temp.row = i - j; temp.collumn = i; bezpiecznik = true; } }
			}
		}
		if (licznik > licznikmax && bezpiecznik) { licznikmax = licznik; a = temp; a.max = licznikmax; }
		licznik = 0; bezpiecznik = false;
	}
	//przekatna 1/2
	for (int j = 0; j < wymiar; j++)
	{
		for (int i = 0; i < wymiar; i++) {
			if (i + j < wymiar) {
				if (przyciski[i + j][wymiar - 1 - i]->label() == x) { licznik += 1; }
				if (i + j < 19 && i + j > 0 && i < 19 && i>0 && (przyciski[i + j + 1][wymiar - 1 - i - 1]->label() == x || przyciski[i + j - 1][wymiar - 1 - i + 1]->label() == x)) { if (przyciski[i + j][wymiar - 1 - i]->label() == 0) { temp.row = i + j; temp.collumn = wymiar - 1 - i; bezpiecznik = true; } }
			}
		}
		if (licznik > licznikmax && bezpiecznik) { licznikmax = licznik; a = temp; a.max = licznikmax; }
		licznik = 0; bezpiecznik = false;
	}
	//przekatna 2/2
	for (int j = 0; j < wymiar; j++)
	{
		for (int i = 0; i < wymiar; i++) {
			if (i - j >= 0) {
				if (przyciski[i - j][wymiar - 1 - i]->label() == x) { licznik += 1; }
				if (i - j < 19 && i - j > 0 && i < 19 && i>0 && (przyciski[i - j + 1][wymiar - 1 - i - 1]->label() == x || przyciski[i - j - 1][wymiar - 1 - i + 1]->label() == x)) { if (przyciski[i - j][wymiar - 1 - i]->label() == 0) { temp.row = i - j; temp.collumn = wymiar - 1 - i; bezpiecznik = true; } }
			}
		}
		if (licznik > licznikmax && bezpiecznik) { licznikmax = licznik; a = temp; a.max = licznikmax; }
		licznik = 0; bezpiecznik = false;
	}



	if (licznikmax >= 0)return a;
	cout << "fail" << endl;
	a.row = -1; a.collumn = -1;
	return a;

}
//sprawdzanie wygranej
void na3::sprawdz_linie()
{
	bool sprawdzenie = false;
	char* x;
	if (gracz) { x = (char*)"X"; }
	else x = (char*)"O";

	for (int i = 0; i < wymiar; i++)
	{
		if (przyciski[i][0]->label() == x && przyciski[i][1]->label() == x && przyciski[i][2]->label() == x) { sprawdzenie = true; break; }
		if (przyciski[0][i]->label() == x && przyciski[1][i]->label() == x && przyciski[2][i]->label() == x) { sprawdzenie = true; break; }
	}
	if (przyciski[0][0]->label() == x && przyciski[1][1]->label() == x && przyciski[2][2]->label() == x) { sprawdzenie = true; }
	if (przyciski[0][2]->label() == x && przyciski[1][1]->label() == x && przyciski[2][0]->label() == x) { sprawdzenie = true; }

	if (sprawdzenie)
	{
		KoniecGry(1);
	}
}
void na5::sprawdz_linie()
{
	bool sprawdzenie = false;
	char* x;

	if (gracz) { x = (char*)"X"; }
	else x = (char*)"O";

	for (int i = 0; i < wymiar; i++)
	{
		for (int j = 0; j + 4 < wymiar; j++) {
			if (przyciski[i][j]->label() == x && przyciski[i][j + 1]->label() == x && przyciski[i][j + 2]->label() == x && przyciski[i][j + 3]->label() == x && przyciski[i][j + 4]->label() == x) { sprawdzenie = true; break; }
			if (przyciski[j][i]->label() == x && przyciski[j + 1][i]->label() == x && przyciski[j + 2][i]->label() == x && przyciski[j + 3][i]->label() == x && przyciski[j + 4][i]->label() == x) { sprawdzenie = true; break; }
		}
		if (sprawdzenie)break;
	}

	//przekatna 1
	for (int i = 0; i < wymiar; i++)
	{
		for (int j = 0; j < wymiar; j++) {
			if (i + j + 4 < wymiar) {
				if (przyciski[i + j][i]->label() == x && przyciski[i + j + 1][i + 1]->label() == x && przyciski[i + j + 2][i + 2]->label() == x && przyciski[i + j + 3][i + 3]->label() == x && przyciski[i + j + 4][i + 4]->label() == x) { sprawdzenie = true; break; }
			}
			if (i - j - 4 >= 0) { //dziala
				if (przyciski[i - j][i]->label() == x && przyciski[i - j - 1][i - 1]->label() == x && przyciski[i - j - 2][i - 2]->label() == x && przyciski[i - j - 3][i - 3]->label() == x && przyciski[i - j - 4][i - 4]->label() == x) { sprawdzenie = true; break; }
			}
		}
		if (sprawdzenie)break;
	}
	//przekatna 2
	for (int i = 0; i < wymiar / 2; i++)
	{
		for (int k = wymiar - 1; k >= 0; k--)
		{
			for (int j = 0; j < wymiar / 2; j++)
			{
				if (i + j + 4 <= wymiar && k - 4 >= 0) {
					if (przyciski[i + j][k]->label() == x && przyciski[i + j + 1][k - 1]->label() == x && przyciski[i + j + 2][k - 2]->label() == x && przyciski[i + j + 3][k - 3]->label() == x && przyciski[i + j + 4][k - 4]->label() == x) { sprawdzenie = true; break; }
				}
				if (i - j >= 0 && k - 4 >= 0) {
					if (przyciski[i - j][k]->label() == x && przyciski[i - j + 1][k - 1]->label() == x && przyciski[i - j + 2][k - 2]->label() == x && przyciski[i - j + 3][k - 3]->label() == x && przyciski[i - j + 4][k - 4]->label() == x) { sprawdzenie = true; break; }
				}
			}
		}
		if (sprawdzenie)break;
	}

	if (sprawdzenie)
	{
		KoniecGry(1);
	}
}
//sztuczna inteligencja
void na3::skynet(Fl_Widget* w, void* data)
{
	coords c; c.row = 0; c.collumn = 0;
	if (tura == 1) { w = przyciski[1][1]; }
	while (w->label() != 0 || tura == 0)
	{
		//zajmowanie rogow w pierwszych turach
		if (tura == 0 || tura == 1 || tura == 2)
		{
			if (tura == 2 && przyciski[1][1]->label() != 0)
			{
				if (ostatniRuch.row == 0)
				{
					if (ostatniRuch.collumn == 0) { c.row = 2; c.collumn = 2; }
					else { c.row = 2; c.collumn = 0; }
				}
				else if (ostatniRuch.row == 2)
				{
					if (ostatniRuch.collumn == 0) { c.row = 0; c.collumn = 2; }
					else { c.row = 0; c.collumn = 0; }
				}
			}
			else do {
				c.row = rand() % wymiar;
				c.collumn = rand() % wymiar;
				w = przyciski[c.row][c.collumn];
				//cout << "wylosowano rog" << c.row << " " << c.collumn;
			} while (w->label() != 0 || ((c.row + c.collumn) % 2 != 0) || w == przyciski[1][1]);

			w = przyciski[c.row][c.collumn];
			//cout << c.row << c.collumn;
			break;
		}


		//wykanczanie gracza
		c = sprawdz_sasiednie((char*)"O");
		if (c.row != -1) {
			w = przyciski[c.row][c.collumn];
			if (w->label() == 0) { cout << "wyeliminowano"; cout << " x: " << c.row << " y: " << c.collumn << endl;  break; }
		}
		//blokowanie gracza
		c = sprawdz_sasiednie((char*)"X");
		if (c.row != -1) {
			w = przyciski[c.row][c.collumn];
			if (w->label() == 0) { cout << "zablokowano"; cout << " x: " << c.row << " y: " << c.collumn << endl;  break; }
		}

		//zajmowanie nie-rogu w turze 3
		if (tura == 3 && przyciski[1][1]->label() == (char*)"O")
		{
			do {
				c.row = rand() % wymiar;
				c.collumn = rand() % wymiar;
				w = przyciski[c.row][c.collumn];
				//cout << "wylosowano nie-rog" << c.row << " " << c.collumn;
			} while (w->label() != 0 || ((c.row + c.collumn) % 2 == 0) && w != przyciski[1][1]);

			w = przyciski[c.row][c.collumn];
			break;
		}

		//losowe zajmowanie pol jezeli nie wyjdzie
		do {
			c.row = rand() % wymiar;
			c.collumn = rand() % wymiar;
			w = przyciski[c.row][c.collumn];
		} while (w->label() != 0);

		cout << "wylosowano " << c.row << " " << c.collumn;
		break;
	}
	ostatniRuch.row = c.row;
	ostatniRuch.collumn = c.collumn;
	if (gracz) w->label("X");
	else w->label("O");
	w->labelsize(30);
	sprawdz_linie();
	gracz = !gracz;
	tura++;
	cout << "tura: " << tura << endl;
}
void na5::skynet(Fl_Widget* w, void* data)
{
	if (tura == 0) { w = przyciski[rand() % wymiar][rand() % wymiar]; }
	while (w->label() != 0)
	{
		if (tura < 2) {
			w = przyciski[rand() % wymiar][rand() % wymiar];
		}

		coords c;
		c = sprawdz_sasiednie((char*)"X");
		//blokowanie gracza
		if (c.row != -1) {
			w = przyciski[c.row][c.collumn];
			if (w->label() == 0) { cout << "zablokowano"; cout << " x: " << c.row << " y: " << c.collumn << endl;  break; }
		}
		c = sprawdz_sasiednie((char*)"O");
		//wykanczanie gracza
		if (c.row != -1) {
			w = przyciski[c.row][c.collumn];
			if (w->label() == 0) { cout << "wyeliminowano"; cout << " x: " << c.row << " y: " << c.collumn << endl;  break; }
		}


		//losowe zajmowanie pol jezeli nie wyjdzie
		while (w->label() != 0)
		{
			w = przyciski[rand() % wymiar][rand() % wymiar];
			cout << "wylosowano";
		}
		break;
	}
	if (gracz) w->label("X");
	else w->label("O");
	w->labelsize(30);
	sprawdz_linie();
	gracz = !gracz;
	tura++;
	cout << endl << "tura:" << tura << endl;
}
//obsługa planszy
void fCallback(Fl_Widget* w, void* data) {
	pole* P = (pole*)data;
	if (P->end) {
		//sprzatanie
		for (int i = 0; i < P->rozmiar - 1; i++)
		{
			for (int j = 0; j < P->rozmiar - 1; j++)
			{
				P->przyciski[1][1]->label("");
			}
		}
		P->tura = 0;
		tryb->show();
		plansza->hide();
		return;
	}
	if (w->label() != "X" && w->label() != "O")
	{
		if (P->gracz || !P->komputer)
		{
			if (P->gracz) w->label("X");
			else w->label("O");
			w->labelsize(30);
			P->tura++;
			if ((P->tura == 9 && P->wymiar == 3) || (P->tura == 400 && P->wymiar == 20)) { P->sprawdz_linie(); if (!P->end) P->KoniecGry(0); }
			//if (P->tura == 24 && P->rozmiar == 5) {P->sprawdz_linie(); P->KoniecGry(0); }
			cout << "tura: " << P->tura << endl;
			P->sprawdz_linie();
			P->gracz = !P->gracz;
		}
		//ruch komputera
		if (P->komputer && !P->end) {
			P->skynet(w, data);
			if ((P->tura == 9 && !P->end && P->wymiar == 3) || (P->tura == 25 && !P->end && P->wymiar == 5)) { P->sprawdz_linie(); if (!P->end) P->KoniecGry(0); }
		}

	}
	/*
	int alfa, beta, gamma;
	cin >> alfa >> beta;
	w = P->przyciski[alfa][beta];
	w->label("X");
	w->labelsize(30);
	P->tura++;
	P->gracz = !P->gracz;
	*/
}
//rysowanie planszy / ustawienia trybów
void pole::rysuj_plansze(int x, int y, int wymiar)
{
	cout << " gra gracz" << gracz << endl;
	int X = x; int Y = x;
	int W = y; int H = y;
	for (int j = 0; j < wymiar; j++)
	{
		for (int i = 0; i < wymiar; i++)
		{

			przyciski[i][j] = new Fl_Button(X, Y, W, H, 0);
			//przyciski[i][j]->color(87+i);
			przyciski[i][j]->callback(fCallback, this);

			X += y;
		}
		X = x; Y += y; //reset x, zwiekszenie y
	}
	Fl_Button* surrender = new Fl_Button(ramka, rozmiar, 100, 30, "powrot");
	Fl_Button* toggleCPU = new Fl_Button(2 * ramka + 100, rozmiar, 100, 30, "komputer");
	surrender->callback(poddajSie, this); toggleCPU->callback(skynetOn, this);
	if (!gracz && komputer) przyciski[0][0]->do_callback();
}
void pole::skynetOn(Fl_Widget* w, void* data)
{
	Fl_Button* W = (Fl_Button*)w;
	pole* P = (pole*)data;
	P->komputer = !P->komputer;
	if (P->komputer) { cout << " dziala "; W->value(1); }
	else { cout << " nie dziala "; W->value(0); }
	if (P->tura > 0 && !P->gracz) P->przyciski[rand() % P->wymiar][rand() % P->wymiar]->do_callback();
}
void pole::gracz2pierwszy(Fl_Widget* w, void* data)
{
	Fl_Button* W = (Fl_Button*)w;
	pole* P = (pole*)data;
	P->gracz = !P->gracz;
	if (P->gracz) { cout << " gracz 1 zaczyna "; W->value(0); }
	else { cout << " gracz 2 zaczyna "; W->value(1); }
}
void pole::ustawtryb3(Fl_Widget* w, void* data)
{
	pole* P = (pole*)data;
	P = new na3(P->komputer, P->gracz);
	pole::ustawtryb(w, P);
}
void pole::ustawtryb5(Fl_Widget* w, void* data)
{
	pole* P = (pole*)data;
	P = new na5(P->komputer, P->gracz);
	pole::ustawtryb(w, P);
}
void pole::ustawtryb(Fl_Widget* w, void* data) //wszystko dziala, moze by tak CPU on/off?
{
	pole* P = (pole*)data;

	plansza = new Fl_Window(500, 100, P->rozmiar, P->rozmiar + 60, "GAME ON");
	P->rysuj_plansze(P->ramka, P->przycisk, P->wymiar); //rysuje 20[j] od rogu okna
	plansza->end();
	plansza->hide();
	tryb->hide(); plansza->show();
}
//wyjscie z gry
void pole::close(Fl_Widget* w, void* v)
{
	exit(0);
}


int main()
{
	srand(time(NULL));
	//inicjalizacja
	pole* B = new pole(0, 1);
	B->szkielet_gry();


	return Fl::run();
	//return 0;
}