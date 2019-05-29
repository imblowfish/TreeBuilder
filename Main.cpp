#include "pch.h"
#include <iostream>
#include "Main.h"

/*
	Задачи:
		-уменьшить шрифт для нижних узлов
		-начать делать саму лабу
*/

int main(int argc, char *argv[]){
	if(!initSDL("Tree Constructor")){
		getchar();
		return -1;
	} else cout << "initSDL() success" << endl;
	MainMenu menu("MainMenu", true);
	menu.LinkRenderer(renderer);
	menu.InitMenu();
	menu.MainLoop();
	return 0;
}

