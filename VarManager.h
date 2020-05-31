#pragma once
#include<iostream>
#include<map>
#include<string>
#include<stack>

using namespace std;
class VarManager {

private:
	string *resAsm;
	const static int VARSIZE = 2;
	map<string, int> varPos;
	stack<int> fpStack;
	int fpOffset;
	stack<string> paramSt;

	string IsParam(string name);
	string IsVar(string name);
	string IsArray(string name);
	string IsNumber(string name);
public:
	VarManager(string * resString);
	void addVar(string name, int x);
	void newFunc(int n);
	void retFunc();
	string get(string name);
	void pushParam(string varName);
};
