#include"CodeGenerator.h"

CodeGenerator::CodeGenerator()
{
	
}


std::string* CodeGenerator::getAsm(std::vector<IRcode>* IRList)
{
	resCode = ".386\n";
	resCode += "stacks segment\n";
	resCode += "	db 1048576 dup(?)\n";
	resCode += "stacks ends\n";
	resCode += "code segment use16\n";
	resCode += "assume ds: data, cs: code, ss: stacks";
	resCode += "main:\n";
	resCode += "	mov ax, data\n";
	resCode += "	mov ds, ax\n";

	//////////
	//todo
	for (auto it = IRList->begin(); it != IRList->end(); it++)
		addCode(*it);
	//////
	resCode += "code ends\n";
	resCode += "end main\n";
}

void CodeGenerator::addCode(IRcode code)
{
	switch (code.op) {
		case IRcode::LABEL:
			res
	}
}