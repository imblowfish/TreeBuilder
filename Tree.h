#pragma once
#ifndef TREE_H
#define TREE_H
#include <iostream>
#include "FileWriter.h"
using namespace std;
enum playerType{
	MAX, MIN
};
struct Node{
	int data, 
		id, 
		lvl, 
		posX, 
		posY;
	bool edited;
	Node *child, 
		 *parent, 
		 *next, 
		 *prev;
	Node(){
		child = parent = next = prev = nullptr;
		id = lvl = -1;
		data = posX = posY = 0;
		edited = false;
	}
};
class Tree: public FileWriter{
private:
	Node *root;
	int whoFirst = -1;
	vector<int> levelsInfo;
	//очистка дерева
	void ClearTree(){
		ClearNode(root);
		levelsInfo.clear();
		root = nullptr;
	}
	//рекурсивная очистка нода, соседей и child
	void ClearNode(Node *n){
		if(!n) return;
		Node *tmp;
		tmp = n;
		while(tmp){
			n = n->next;
			if(tmp->child) ClearNode(tmp->child);
			delete tmp;
			tmp = n;
		}
	}
	//удаление только нода и child этого нода
	void ClearNodeById(unsigned int id){
		Node *tmp = FindNodeById(root, id);
		if(!tmp) return;
		if(tmp == root){
			ClearTree();
			return;
		}
		if(tmp->child) ClearNode(tmp->child);
		if(tmp->parent)
			if(tmp->parent->child == tmp)
				tmp->parent->child = tmp->next;
		if(tmp->prev) tmp->prev->next = tmp->next;
		if(tmp->next) tmp->next->prev = tmp->prev;
		delete tmp;
	}
	//поиск ноды по идентификатору
	Node* FindNodeById(Node *n, unsigned int id){
		if(!n) return nullptr;
		while(n){
			if(n->id == id) return n;
			if(n->child)
				if(FindNodeById(n->child, id))
					return FindNodeById(n->child, id);
			n = n->next;
		}
		return nullptr;
	}
	void CalculateLevels(Node *n, vector<int> *arr){
		if(!n) return;
		Node *tmp = n;
		if((*n).lvl >= (*arr).size()) (*arr).push_back(0);
		while(tmp){
			if(tmp->child) CalculateLevels(tmp->child, arr);
			tmp = tmp->next;
			(*arr)[(*n).lvl]++;
		}
	}
	void WriteNodeToFile(Node *n){
		if(!n) return;
		while(n){
			WriteNode(n);
			if(n->child) WriteNodeToFile(n->child);
			n = n->next;
		}
	}
	void SetNodeData(int id, int data){
		Node *tmp = FindNodeById(id);
		tmp->data = data;
	}
	void SetNodePos(int id, int posX, int posY){
		Node *tmp = FindNodeById(id);
		tmp->posX = posX;
		tmp->posY = posY;
	}
public:
	Tree(){root = nullptr;}
	~Tree(){ClearTree();}
	bool Empty(){return root == nullptr;}
	void AddRoot(unsigned int id, int data=0){
		if(!root){
			root = new Node();
			root->data = data;
			root->id = id;
			root->lvl = 0;
			return;
		}
	}
	bool AddChildNode(unsigned int parentId, unsigned int id, int data=0){
		Node *tmp = FindNodeById(root, parentId);
		if(!tmp) return false;
		if(FindNodeById(tmp, id)) return true;
		if(!tmp->child){
			tmp->child = new Node();
			tmp->child->data = data;
			tmp->child->id = id;
			tmp->child->parent = tmp;
			tmp->child->lvl = tmp->child->parent->lvl+1;
			return true;
		}
		tmp = tmp->child;
		while(tmp->next) tmp = tmp->next;
		tmp->next = new Node();
		tmp->next->prev = tmp;
		tmp->next->parent = tmp->parent;
		tmp = tmp->next;
		tmp->data = data;
		tmp->id = id;
		tmp->lvl = tmp->prev->lvl;
		return true;
	}
	Node* FindNodeById(unsigned int id){return FindNodeById(root, id);}
	void DeleteNode(unsigned int id){
		if(Empty())	return;
		ClearNodeById(id);
		if(id == 0) root = nullptr;
	}
	void CalculateLevelsInfo(){
		levelsInfo.clear();
		CalculateLevels(root, &levelsInfo);
	}
	//возвращает вектор размеров с глубину дерева и количество нодов на каждом уровне
	vector<int>* GetLevelsInfo(){
		if(levelsInfo.empty()) return nullptr;
		return &levelsInfo;
	}
	Node* GetRoot(){return root;}
	void SaveTree(string filepath){
		SetPath(filepath);
		ResetFile();
		WriteVal(whoFirst);
		WriteNodeToFile(root);
	}
	void LoadNode(vector<int> *nodeInfo, unsigned int *maxId){
		enum infoSelector {PARENT, ID, DATA, POSX, POSY};
		if(*maxId <= (*nodeInfo)[ID])
			*maxId = (*nodeInfo)[ID]+1;
		Node *tmp;
		if(Empty()){
			AddRoot((*nodeInfo)[ID]);
			root->posX = (*nodeInfo)[POSX];
			root->posY = (*nodeInfo)[POSY];
			root->edited = true;
			return;
		}
		if((*nodeInfo)[PARENT] >= 0) AddChildNode((*nodeInfo)[PARENT], (*nodeInfo)[ID]);
		tmp = FindNodeById((*nodeInfo)[ID]);
		if(!tmp->child) tmp->data = (*nodeInfo)[DATA];
		tmp->posX = (*nodeInfo)[POSX];
		tmp->posY = (*nodeInfo)[POSY];
		tmp->edited = true;
	}
	void SetFirstPlayer(int val=-2){
		if(val == -2){
			if(whoFirst == MAX || whoFirst < 0) whoFirst = MIN;
			else if(whoFirst == MIN) whoFirst = MAX;
		} else whoFirst = val;
	}
	int GetFirstPlayer(){return whoFirst;}
	void SetData(int idx, int data){
		Node *tmp = FindNodeById(idx);
		tmp->data = data;
	}
};

#endif