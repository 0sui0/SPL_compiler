#pragma once
#include<iostream>
#include<cstring>
#include<vector>
#include"IRcode.h"
#include"VarManager.h"

class CodeGenerator {
private:
	std::string resAsm;
	int sp;
	void addAsm(std::string str);
	void addCode(IRcode code);
	VarManager *varMng;
	void addReadFunc();
	void addWriteFunc();
public:
	CodeGenerator();
	std::string *getAsm(std::vector<IRcode>* IRList);
	friend class VarManager;
};
