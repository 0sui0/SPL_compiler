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
		case IRcode::INT:
			////todp
			break;
		case IRcode::REAL:
			////todo
			break;
		
		//function
		case IRcode::FUNC:

		case IRcode::PARAM:

		case IRcode::CALL:

		case IRcode::CALL_V:
		
		case IRcode::RETURN:

		case IRcode::RETURN_V:

		//operator
		case IRcode::POSIVITE:
			addAsm("	mov ax, " + varMng.get(code.v2));
			addAsm("	mov " + varMng.get(code.v1) + ", ax");
			break;
		case IRcode::NEGATIVE:
			addAsm("	mov ax, " + varMng.get(code.v2));
			addAsm("	xor bx, bx")
			addAsm("	sub bx, ax");
			addAsm("	mov " + varMng.get(code.v1) + ", bx");
			break;
		case IRcode::NEG_LOGIC:
			addAsm("	mov ax", + varMng.get(code.v2));
			addAsm("	not ax");
			addAsm("	mov " + varMng.get(code.v1) + ", bx");
			break;
		

		

			
	}
}