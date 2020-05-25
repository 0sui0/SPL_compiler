#include"CodeGenerator.h"

CodeGenerator::CodeGenerator()
{
	
}


std::string* CodeGenerator::getAsm(std::vector<IRcode>* IRList)
{
	resAsm = "";
	addAsm(".386");
	addAsm("stacks segment");
	addAsm("	db 1048576 dup(?)");
	addAsm("stacks ends");
	addAsm("code segment use16");
	addAsm("assume ds: data, cs: code, ss: stacks");

	//////////
	//todo
	for (auto it = IRList->begin(); it != IRList->end(); it++)
		addCode(*it);
	//////
	addAsm("code ends");
	addAsm("end main");
}

void CodeGenerator::addCode(IRcode code)
{
	switch (code.op) {

		//branch
		case IRcode::LABEL:
			addAsm("	" + code.v1 + ":");
			break;
		case IRcode::GOTO:
			addAsm("	jmp " + code.v1);
			break;
		case IRcode::IF:
			addAsm("	mov ax, " + varMsg.get(code.v1));
			addAsm("	cmp ax, 0");
			addAsm("	jne " + code.v2);
			break;

		//variable
		case IRcode::MALLOC:
			varMsg.add(code.v1, code.v2);
			break;
		

			
	}
}