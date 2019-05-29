#pragma once
#ifndef MAINMENU_H
#define MAINMENU_H
#include "Menu.h"
#include "FontBuilder.h"
#include "TreeViewer.h"
#include "TreeBuilder.h"

class MainMenu: public Menu{
private:
	const int SCREEN_WIDTH=900, SCREEN_HEIGHT=600;
	unsigned int nowMenuMode = MAIN_MENU;
	int keys[1024];
	bool dragAndDrop = false;
	string pathToSave = "trees\\tree.txt";
	vector<string> buttonNames = {"Add Node|A", "Delete Node|Ctrl+X", "Save Tree|Ctrl+S", "Load Tree|Ctrl+L", "Set Player|Q", "Show/Hide ID|S"};
	enum buttonKeys {ADD, DELETE, SAVE, LOAD, SET_PLAYER, SHOW_ID};
	FontBuilder fBuilder;
	TreeViewer tViewer;
	TreeBuilder tBuilder;
	void Redraw();
	void InputInEditBox(SDL_Event event){
		if(tViewer.GetNowSelected() < 0) return;
		if(keys[SDLK_LEFT]) tViewer.InputInBox(event.key.keysym.sym, SDLK_LEFT);
		else if(keys[SDLK_RIGHT]) tViewer.InputInBox(event.key.keysym.sym, SDLK_RIGHT);
		else if(keys[SDLK_RETURN]) tViewer.InputInBox(event.key.keysym.sym, SDLK_RETURN);
		else if(keys[SDLK_BACKSPACE]) tViewer.InputInBox(event.key.keysym.sym, SDLK_BACKSPACE);
		else tViewer.InputInBox(event.key.keysym.sym);
	}
public:
	MainMenu(string name, bool active):Menu(name, active){fBuilder.TTFInit();}
	~MainMenu(){
		buttons.clear();
		fBuilder.TTFQuit();
	}
	void InitMenu();
	void MainLoop();
	void ActionOnButton(string val);
};

#endif