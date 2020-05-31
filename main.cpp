#include<iostream>
#include"IRcode.h"
#include<vector>
#include<fstream>
#include"CodeGenerator.h"

using namespace std;

int main()
{
	CodeGenerator cg;
	vector<IRcode> list;
	IRcode *a;
	a = new IRcode(IRcode::FUNC, "_gcd", "2", "");
	list.push_back(*a);
	list.push_back(*a);
	a = new IRcode(IRcode::LABEL, "__main", "", "");
	list.push_back(*a);

	a = new IRcode(IRcode::LABEL, "__main", "", "");
	list.push_back(*a);
	a = new IRcode(IRcode::CALL_V, "v1", "read", "0");
	list.push_back(*a);
	a = new IRcode(IRcode::CALL_V, "v2", "read", "0");
	list.push_back(*a);
	a = new IRcode(IRcode::ADD_I, "v1", "v1", "v2");
	list.push_back(*a);
	a = new IRcode(IRcode::PARAM, "v1", "", "");
	list.push_back(*a);
	a = new IRcode(IRcode::CALL, "", "write", "1");
	list.push_back(*a);

	ofstream fout("C:\\Users\\sui\\Desktop\\DosBox86\\masm\\CODE\\test.asm");
	fout<<*cg.getAsm(&list);
	system("pause");
}