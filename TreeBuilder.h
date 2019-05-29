#pragma once
#ifndef TREE_BUILDER_H
#define TREE_BUILDER_H
#include <iostream>
#include "Tree.h"
using namespace std;

class TreeBuilder{
private:
	Tree tree;
	unsigned int nowId;
public:
	TreeBuilder(){nowId = 0;}
	~TreeBuilder(){tree.DeleteNode(0);}
	void AddNodeTo(int id){
		if(tree.Empty()) CreateTree();
		else tree.AddChildNode(id, nowId++);
		tree.CalculateLevelsInfo();
	}
	void CreateTree(){
		if(!tree.Empty()){
			tree.DeleteNode(0);
			nowId = 0;
		}
		tree.AddRoot(nowId++); 
		tree.CalculateLevelsInfo();
	}
	void DeleteNode(unsigned int id){
		tree.DeleteNode(id);
		if(tree.Empty()) nowId = 0;
		tree.CalculateLevelsInfo();
	}
	bool IsEmpty(){return tree.Empty();}
	Tree* GetTree(){return &tree;}
	vector<int>* GetLevelsInfo(){return tree.GetLevelsInfo();}
	void Edit(int idx, int posX){
		if(idx < 0) return;
		Node *tmp = tree.FindNodeById(idx);
		tmp->posX = posX;
		tmp->edited = true;
	}
	void SaveTree(string filepath){tree.SaveTree(filepath);}
	void LoadTree(string filepath){
		tree.DeleteNode(0);
		nowId = 0;
		string str;
		ifstream file;
		vector<int> nodeInfo;
		file.open(filepath.c_str());
		if(!file.is_open()) return;
		getline(file, str);
		tree.SetFirstPlayer(atoi(str.c_str()));
		while(!file.eof()){
			nodeInfo.clear();
			for(unsigned int i = 0; i < 5; i++){
				getline(file, str);
				nodeInfo.push_back(atoi(str.c_str()));
			}
			if(file.eof()) continue;
			tree.LoadNode(&nodeInfo, &nowId);
		}
		file.close();
		tree.CalculateLevelsInfo();
	}
	void ClearTree(){
		tree.DeleteNode(0);
		nowId = 0;
	}
	void SetFirstPlayer(){
		tree.SetFirstPlayer();
		/*for(unsigned int i = 0; i < val.size(); i++)
			if(val[i] <= 'Z') val[i] += 32;
		if(strcmp(val.c_str(), "min") == 0){
			cout << "MIN" << endl;
			tree.SetFirstPlayer(MIN);
		} else if(strcmp(val.c_str(), "max") == 0){
			cout << "MAX" << endl;
			tree.SetFirstPlayer(MAX);
		} else{
			cout << "ELSE" << endl;
			tree.SetFirstPlayer();
		}*/
	}
	void SetData(int idx, int data){tree.SetData(idx, data);}
};

#endif