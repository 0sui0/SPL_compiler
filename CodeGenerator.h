#pragma once
#include<iostream>
#include<cstring>
#include<vector>
#include"IRcode.h"
class CodeGenerator {
private:
	std::string resCode;
	void addCode(IRcode code);

public:
	CodeGenerator();
	std::string *getAsm(std::vector<IRcode>* IRList);
	
};
