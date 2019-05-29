#pragma once
#ifndef TREE_VIEWER_H
#define TREE_VIEWER_H
#include <iostream>
#include "Colors.h"
#include "SDL.h"
#include "Tree.h"
#include "Drawer.h"
#include "EditBox.h"
#include "Textfield.h"
using namespace std;

class TreeViewer{
private:
	SDL_Renderer *renderer;
	EditBox editBox;
	Drawer drawer;
	int nodeRadius = 10;
	int SCREEN_WIDTH, SCREEN_HEIGHT;
	int offsetY, verticalMax, nowSelected;
	bool nowEdit;
	bool showId = false;
	vector<int> *levelInfo, count;
	vector<Node> nodes, bottomNodes;
	TextField textfield;
	void CalculateNodeCoord(Node *n, int count, int *x, int *y){
		if((*n).lvl > 0){
			*y = int(offsetY * ((*n).lvl + 1));
		} else *y = int(offsetY);
		if((*n).edited){
			*x = (*n).posX;
			(*n).posY = *y;
			return;
		}
		/*if(!(*n).parent) */*x = int((SCREEN_WIDTH / ((*levelInfo)[(*n).lvl] + 1)) * count);
		//else *x = (*n).parent->posX;
		(*n).posX = *x;
		(*n).posY = *y;
	}
	void ConnectNodes(Node *parent, Node *child){
		drawer.DrawLine(parent->posX, parent->posY, child->posX, child->posY, {BLUE_COLOR});
	}
	void DrawNode(int x, int y, bool selected = false){
		SDL_Color ellipseColor;
		if(!selected) ellipseColor = {BLUE_COLOR};
		else ellipseColor = {RED_COLOR};
		drawer.DrawFilledCircle(x, y, nodeRadius, ellipseColor);
	}
	void DrawTree(Node *n){
		if(!n) return;
		Node *tmp = n;
		int x, y = 0;
		while(tmp){
			CalculateNodeCoord(tmp, count[(*tmp).lvl], &x, &y);
			if(tmp->parent) ConnectNodes(tmp->parent, tmp);
			if(tmp->child){
				nodes.push_back(*tmp);
				DrawTree(tmp->child);
			}else bottomNodes.push_back(*tmp);
			//отрисовка
			if(tmp->id == nowSelected)DrawNode(x, y, true);
			else DrawNode(x, y);
			//если нужно посмотреть ID элементов
			if(showId) DrawID(x, y, tmp->id);
			tmp = tmp->next;
			count[(*n).lvl]++;
		}
	}
	void DrawLevels(int firstPlayer, int levels){
		textfield.Clear();
		int x = 15, y;
		for(unsigned int i = 0; i < levels; i++){
			if(i > 0) y = int(offsetY * (i + 1));
			else y = int(offsetY);
			drawer.DrawLine(0, y, SCREEN_WIDTH, y, {GREY_COLOR});
			string val;
			if(firstPlayer >= 0){
				if(firstPlayer == MAX) val = "max";
				else if(firstPlayer == MIN) val = "min";
				else val = "Nan";
				firstPlayer = !firstPlayer;
			}
			textfield.LinkRenderer(renderer);
			textfield.SetupMe(val, x, y, 15, {BG_COLOR}, {BG_COLOR}, {BG_COLOR}, {FONT_COLOR});
			textfield.Draw();
		}
	}
	void DrawID(int posX, int posY, int val){
		textfield.Clear();
		textfield.LinkRenderer(renderer);
		textfield.SetupMe(to_string(val), posX, posY-20, 10,
						  {RED_COLOR}, {BG_COLOR}, {BG_COLOR}, {BLACK_COLOR}, true);
		textfield.Draw();
		textfield.Clear();
	}
	void DrawNodesValues(vector<Node> *nodeArr = nullptr){
		if(!nodeArr) return;
		int x, y;
		for(unsigned int i = 0; i < (*nodeArr).size(); i++){
			if(showId) DrawID((*nodeArr)[i].posX, (*nodeArr)[i].posY, (*nodeArr)[i].id);
			x = (*nodeArr)[i].posX;
			y = (*nodeArr)[i].posY;

			textfield.LinkRenderer(renderer);
			if((*nodeArr)[i].id == nowSelected)
				textfield.SetupMe(to_string((*nodeArr)[i].data), x, y, 12,
								  {RED_COLOR}, {BG_COLOR}, {BG_COLOR}, {WHITE_COLOR}, true);
			else
				textfield.SetupMe(to_string((*nodeArr)[i].data), x, y, 12,
								  {BLUE_COLOR}, {BG_COLOR}, {BG_COLOR}, {WHITE_COLOR}, true);
			textfield.Draw();
		}
		textfield.Clear();
	}
public:
	~TreeViewer(){
		nodes.clear();
		bottomNodes.clear();
	}
	void LinkRenderer(SDL_Renderer *renderer){
		this->renderer = renderer; 
		drawer.LinkRenderer(renderer);
	}
	void Setup(int width, int height){
		SCREEN_WIDTH = width;
		SCREEN_HEIGHT = height;
		nowSelected = -1;
	}
	void Draw(Tree *tree, vector<int> *levelInfo){
		if(tree->Empty()) return;
		this->levelInfo = levelInfo;
		count.clear();
		verticalMax = (*levelInfo).size();
		for(unsigned int i = 0; i < (*levelInfo).size(); i++) count.push_back(1);
		nodes.clear();
		bottomNodes.clear();
		offsetY = int(SCREEN_HEIGHT / (verticalMax + 1));
		DrawLevels(tree->GetFirstPlayer(), (*levelInfo).size());
		DrawTree(tree->GetRoot());
		//DrawNodesValues(&nodes);
		DrawNodesValues(&bottomNodes);
		if(nowEdit) editBox.Draw();
	}
	int ClickedOnNode(int mouseX, int mouseY){
		int maxPixelDiff = 15;
		for(unsigned int i = 0; i < nodes.size(); i++)
			if(fabs(nodes[i].posX - mouseX) < maxPixelDiff && fabs(nodes[i].posY - mouseY) < maxPixelDiff)
				return nowSelected = nodes[i].id;
		return -1;
	}
	int ClickedOnBottomNode(int mouseX, int mouseY){
		int maxPixelDiffX = 10, maxPixelDiffY = 10;
		for(unsigned int i = 0; i < bottomNodes.size(); i++)
			if(fabs(bottomNodes[i].posX - mouseX) < maxPixelDiffX && fabs(bottomNodes[i].posY - mouseY) < maxPixelDiffY)
				return nowSelected = bottomNodes[i].id;
		return -1;
	}
	int GetNowSelected(){return nowSelected;}
	int GetVal(){return editBox.GetIntValue();}
	void EndInput(){
		//nowSelected = -1;
		editBox.Clear();
		nowEdit = false;
	}
	void InputInBox(char symbol, int val = -100){
		if(!nowEdit) return;
		editBox.KeyboardListener(symbol, val);
	}
	void StartEdit(){
		if(nowSelected < 0) return;
		int id = -1;
		for(unsigned int i = 0; i < bottomNodes.size(); i++)
			if(nowSelected == bottomNodes[i].id) id = i;
		if(id < 0) return;
		editBox.Clear();
		editBox.LinkRenderer(renderer);
		editBox.SetupMe(to_string(bottomNodes[id].data), 
						bottomNodes[id].posX, bottomNodes[id].posY + 30, 10, 
					   {EDITBOX_COLOR}, {EDITBOX_FRAME_COLOR}, {EDITBOX_INPUT_COLOR}, {EDITBOX_FONT_COLOR});
		nowEdit = true;
		editBox.StartEditing();
	}
	void ResetSelected(){nowSelected = -1;}
	void SetViewID(){
		showId = !showId;
	}
};

#endif