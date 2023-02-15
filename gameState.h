#pragma once
#include<FL/Fl_Window.H>

 class gameState {
	 public:
		Fl_Window* gameSelection;
		Fl_Window* gameMain;
		static void fCallback(Fl_Widget* w, void* data);
};