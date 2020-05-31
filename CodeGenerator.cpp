#include"CodeGenerator.h"

CodeGenerator::CodeGenerator()
{
	varMng = new VarManager(&this->resAsm);
}


std::string* CodeGenerator::getAsm(std::vector<IRcode>* IRList)
{
	resAsm = "";
	addAsm(".386");
	addAsm("data segment use16\nmistake db 0dh, 0ah, \"input error\", '$'\ndata ends");
	addAsm("stacks segment");
	addAsm("	db 65536 dup(?)");
	addAsm("stacks ends");
	addAsm("code segment use16");
	addAsm("assume ds: data, cs: code, ss: stacks");
	addAsm("main:\njmp __main");
	addAsm("mov ax, data\n\
		mov ds, ax\n\
		mov ax, stacks\n\
		mov ss, ax");
	addReadFunc();
	addWriteFunc();

	//////////
	//todo
	for (auto it = IRList->begin(); it != IRList->end(); it++)
		addCode(*it);
	//////
	addAsm("exit:\nmov ah, 4Ch\nint 21h\n");
	addAsm("code ends");
	addAsm("end main");
	return &resAsm;
}

void CodeGenerator::addReadFunc()
{
	addAsm(
		"read proc near  \nxor ebx, ebx\nread_char:\nmov ah, 01h \nint 21h\ncmp al, 13 \nje read_done\nsub al, '0'\ncmp al, 9 \nja char_error\nmov cl, al\nmov ax, 10\nmul bx\nadd ax, cx\n\ncmp dx, 0\nja char_error\n\nmov bx, ax\njmp read_char\nread_done:\n mov ax, bx \n retn \nchar_error:\nmov dx, offset mistake\nmov ah, 09h\nint 21h\n");
	varMng->retFunc();
	addAsm("read endp");
}
void CodeGenerator::addWriteFunc()
{
	addAsm(
		"write  proc near\nxor ecx, ecx\nmov si, ss:[sp]\n\
		mov ax, ss:[si]\n\
		divide:\n\
	xor edx, edx\n\
		mov ebx, 10\n\
		div ebx\n\
		add edx, '0'\n\
		push dx\n\
		inc cx\n\
		cmp eax, 0\n\
		jne divide\n\
		print :\n\
	pop dx\n\
		mov ah, 02h\n\
		int 21h\n\
		loop print\n\
		ret\n\
		write endp\n"
	);

	addAsm(
		"writeln  proc near\nxor ecx, ecx\nmov si, ss:[sp]\n\
		mov ax, ss:[si]\n\
		divide:\n\
	xor edx, edx\n\
		mov ebx, 10\n\
		div ebx\n\
		add edx, '0'\n\
		push dx\n\
		inc cx\n\
		cmp eax, 0\n\
		jne divide\n\
		print :\n\
		pop dx\n\
		mov ah, 02h\n\
		int 21h\n\
		loop print\n\
		mov dx, '\n'\n\
		mov ah, 02h\n\
		int 21h\n\
		ret\n\
		writeln endp\n"
	);
}

void CodeGenerator::addAsm(std::string str)
{
	resAsm += str + "\n";
}


void CodeGenerator::addCode(IRcode code)
{

	static int jumpLabel = 0;
	switch (code.op) {

		//branch
		case IRcode::LABEL:
			addAsm("	" + code.v1 + ":");
			break;
		case IRcode::GOTO:
			addAsm("	jmp " + code.v1);
			break;
		case IRcode::IF:
			addAsm("	mov ax, " + varMng->get(code.v1));
			addAsm("	cmp ax, 0");
			addAsm("	jne " + code.v2);
			break;

		//variable
		case IRcode::MALLOC:
			varMng->addVar(code.v1, stoi(code.v2));
			break;
		case IRcode::INT:
			////todp
			break;
		case IRcode::REAL:
			////todo
			break;
		
		//function
		case IRcode::FUNC:
			addAsm(code.v1 + " proc near");
			addAsm("	push bp");
			addAsm("	mov bp, sp\n");
			break;
		case IRcode::PARAM:
			varMng->pushParam(code.v1);
			break;
		case IRcode::CALL:
			varMng->newFunc(stoi(code.v3));
			addAsm("	call " + code.v2);
			break;
		case IRcode::CALL_V:
			varMng->newFunc(stoi(code.v3));
			addAsm("	call " + code.v2);
			addAsm("	mov " + varMng->get(code.v1) + ", ax");
			break;
		case IRcode::RETURN:
			addAsm("	mov sp, bp");
			addAsm("	pop bp");
			addAsm("	ret");
			break;
		case IRcode::RETURN_V:
			addAsm("	mov ax, " + varMng->get(code.v1));
			addAsm("	mov sp, bp");
			addAsm("	pop bp");
			addAsm("	ret");
			break;
		//operator
			//
		case IRcode::POSIVITE:
			addAsm("	mov ax, " + varMng->get(code.v2));
			addAsm("	mov " + varMng->get(code.v1) + ", ax");
			break;
		case IRcode::NEGATIVE:
			addAsm("	mov ax, " + varMng->get(code.v2));
			addAsm("	xor bx, bx");
			addAsm("	sub bx, ax");
			addAsm("	mov " + varMng->get(code.v1) + ", bx");
			break;
		case IRcode::NEG_LOGIC:
			addAsm("	mov ax, " + varMng->get(code.v2));
			addAsm("	not ax");
			addAsm("	mov " + varMng->get(code.v1) + ", bx");
			break;
			//
		case IRcode::ADD_I:
			addAsm("	mov ax, " + varMng->get(code.v2));
			addAsm("	mov bx, " + varMng->get(code.v3));
			addAsm("	add ax, bx");
			addAsm("	mov " + varMng->get(code.v1) + ", ax");
			break;
		case IRcode::MINUS_I:
			addAsm("	mov ax, " + varMng->get(code.v2));
			addAsm("	mov bx, " + varMng->get(code.v3));
			addAsm("	sub ax, bx");
			addAsm("	mov " + varMng->get(code.v1) + ", ax");
			break;
		case IRcode::MULT_I:
			addAsm("	mov ax, " + varMng->get(code.v2));
			addAsm("	mov bx, " + varMng->get(code.v3));
			addAsm("	imul ax, bx");
			addAsm("	mov " + varMng->get(code.v1) + ", ax");
			break;
		case IRcode::DIV_I:
			addAsm("	mov ax, " + varMng->get(code.v2));
			addAsm("	mov bx, " + varMng->get(code.v3));
			addAsm("	xor dx, dx");
			addAsm("	div ax, bx");
			addAsm("	mov " + varMng->get(code.v1) + ", ax");
			break;
		case IRcode::MOD_I:
			addAsm("	mov ax, " + varMng->get(code.v2));
			addAsm("	mov bx, " + varMng->get(code.v3));
			addAsm("	xor dx, dx");
			addAsm("	div ax, bx");
			addAsm("	mov " + varMng->get(code.v1) + ", dx");
			break;
		case IRcode::ADD_F:
		case IRcode::MINUS_F:
		case IRcode::MULT_F:
		case IRcode::DIV_F:

		case IRcode::GT:
			addAsm("	mov ax, " + varMng->get(code.v2));
			addAsm("	mov bx, " + varMng->get(code.v3));
			addAsm("	cmp ax, bx");
			jumpLabel++;
			addAsm("	jle jump_label" + to_string(jumpLabel));
			addAsm("	mov " + varMng->get(code.v1) + ", 1");
			addAsm("jump_label" + to_string(jumpLabel) + ":");
			break;
		case IRcode::LT:
			addAsm("	mov ax, " + varMng->get(code.v2));
			addAsm("	mov bx, " + varMng->get(code.v3));
			addAsm("	cmp bx, ax");
			jumpLabel++;
			addAsm("	jge jump_label" + to_string(jumpLabel));
			addAsm("	mov " + varMng->get(code.v1) + ", 1");
			addAsm("jump_label" + to_string(jumpLabel) + ":");
			break;
		case IRcode::GE:
			addAsm("	mov ax, " + varMng->get(code.v2));
			addAsm("	mov bx, " + varMng->get(code.v3));
			addAsm("	cmp ax, bx");
			jumpLabel++;
			addAsm("	jle	 jump_label" + to_string(jumpLabel));
			addAsm("	mov " + varMng->get(code.v1) + ", 1");
			addAsm("jump_label" + to_string(jumpLabel) + ":");
			break;
		case IRcode::LE:
			addAsm("	mov ax, " + varMng->get(code.v2));
			addAsm("	mov bx, " + varMng->get(code.v3));
			addAsm("	cmp bx, ax");
			jumpLabel++;
			addAsm("	jg	 jump_label" + to_string(jumpLabel));
			addAsm("	mov " + varMng->get(code.v1) + ", 1");
			addAsm("jump_label" + to_string(jumpLabel) + ":");
			break;
		case IRcode::EQ:
			addAsm("	mov ax, " + varMng->get(code.v2));
			addAsm("	mov bx, " + varMng->get(code.v3));
			addAsm("	cmp bx, ax");
			jumpLabel++;
			addAsm("	jne	 jump_label" + to_string(jumpLabel));
			addAsm("	mov " + varMng->get(code.v1) + ", 1");
			addAsm("jump_label" + to_string(jumpLabel) + ":");
			break;
		case IRcode::NEQ:
			addAsm("	mov ax, " + varMng->get(code.v2));
			addAsm("	mov bx, " + varMng->get(code.v3));
			addAsm("	cmp bx, ax");
			jumpLabel++;
			addAsm("	je	 jump_label" + to_string(jumpLabel));
			addAsm("	mov " + varMng->get(code.v1) + ", 1");
			addAsm("jump_label" + to_string(jumpLabel) + ":");
			break;

		//logic
		case IRcode::AND_LOGIC:
			addAsm("	mov ax, " + varMng->get(code.v2));
			addAsm("	mov bx, " + varMng->get(code.v3));
			addAsm("	and ax, bx");
			addAsm("	mov " + varMng->get(code.v1) + ", ax");
			break;
		case IRcode::OR_LOGIC:
			addAsm("	mov ax, " + varMng->get(code.v2));
			addAsm("	mov bx, " + varMng->get(code.v3));
			addAsm("	or ax, bx");
			addAsm("	mov " + varMng->get(code.v1) + ", ax");
			break;


		
			
	}
}