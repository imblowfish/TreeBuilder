#pragma once
#ifndef SCREEN_ELEMENT_H
#define SCREEN_ELEMENT_H
#include <iostream>
#include <string>
#include "FontBuilder.h"
#include "Colors.h"
using namespace std;

class ScreenElement{
protected:
	int maxPixelDiff = 5;		//������������ ���������� � �������� ��� ����������� ������� ������
	int fontSize = 0;
	bool isAlpha = false;
	FontBuilder fBuilder;		//����� �������� ������
	SDL_Renderer *renderer;		//��������� �� ��������
	SDL_Texture *texture;		//�������� � �������
	string value;				//��������� �������� ��������(��� ����� ��������)
	int xPos, yPos;				//��������� �� ������
	SDL_Color color,			//���� ����
			  frame,			//���� �����
			  inputColor,		//����, ����� ������������ ���� � ����
			  fontColor;		//���� ������
	SDL_Rect bgRect, textRect;	//������������� ���� � ������
	TTF_Font *font;				//����� ������
	bool isSelected, isVisible;	//�����(������) �������, ����� �� �������
	//������������� ������ � ������
	void InitText(bool alphaBackground);
public:
	ScreenElement();
	~ScreenElement(){/*Clear();*/}
	void Clear(){
		if(font) fBuilder.CloseFont(font);
		font = nullptr;
		SDL_DestroyTexture(texture);
	}
	//��������� �������� ��� ����� ������ ������������
	void SetupMe(string val, int x, int y, unsigned int fontSize, SDL_Color color, SDL_Color frameColor, SDL_Color inputColor, SDL_Color fontColor, bool alphaBackground = false);
	void LinkRenderer(SDL_Renderer *renderer){this->renderer = renderer;}
	void SetVisibility(bool val){isVisible = val;}
	bool GetVisibility(){return isVisible;}
	bool MouseEventListener(SDL_Event e, int x, int y);
	int GetIntValue(){
		return atoi(value.c_str());
	}
	void SetNewVal(string val){
		value = val;
		InitText(isAlpha);
	}
	void SetNewValAndPos(string val, int x, int y){
		this->value = val;
		this->xPos = x;
		this->yPos = y;
	}
	string GetStrValue(){
		return value;
	}
	virtual void FeedbackOnMouse(SDL_Event e) = 0;
	virtual void Draw();
};

#endif