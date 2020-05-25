#pragma once
#include<iostream>
#include<cstring>
#include<vector>
#include"IRcode.h"
class CodeGenerator {
private:
	std::string resAsm;
	int sp;
	void addAsm(std::string str);
	void addCode(IRcode code);

public:
	CodeGenerator();
	std::string *getAsm(std::vector<IRcode>* IRList);
	
};
