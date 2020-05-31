#include"VarManager.h"

int toNumber(string name)
{
	int x = 0;
	for (int i = 0; name[i]; ++i)
		if (name[i]<'0' || name[i]>'9')
			return -1;
		else x = x * 10 + name[i] - '0';
	return x;
}

VarManager::VarManager(string * resString)
{
	fpOffset = 0;
	this->resAsm = resString;
}

void VarManager::addVar(string name, int x)
{
	varPos[name]=fpOffset;
	fpOffset += x * VARSIZE;
}

void VarManager::newFunc(int n)
{	
	string res = "";
	for (int i = 0; i < n; ++i)
	{
		string opt = "";
		opt += "	lea ax, " + get(paramSt.top()) + "\n";
		paramSt.pop();
		opt += "	push ax\n";
		res = res + opt;
		fpOffset += VARSIZE;
	}
	*resAsm += res;

	fpStack.push(fpOffset);
	fpOffset = VARSIZE;

}

void VarManager::retFunc() {
	if (!fpStack.empty()) {
		fpOffset = fpStack.top();
		fpStack.pop(); 
	}
}



void VarManager::pushParam(string name)
{
	paramSt.push(name);
}

string VarManager::get(string name)
{
	string res;
	res = IsParam(name);
	if (res != "") return res;
	res = IsArray(name);
	if (res != "") return res;
	res = IsNumber(name);
	if (res != "") return res;
	return IsVar(name);
}

string VarManager::IsArray(string name)
{
	int l = name.find_first_of('[');
	if (l < 0) return "";
	int r = name.find_last_of(']');
	if (name[r + 1] != 0) return "";
	string a = name.substr(0, l);
	//string to int 
	int pos = toNumber(name.substr(l + 1, r)) * VARSIZE;
	if (pos < 0) return "";
	pos += varPos[name];
	return "ss:[bp+" + to_string(pos) + "]";
	
}
string VarManager::IsParam(string name)
{
	if (name.size()<=3||name.substr(3) != "arg") return "";
	int x = toNumber(name.substr(3));
	int l = name.find_first_of('[');
	if (l>=0) {
		int r = name.find_last_of(']');
		int pos = toNumber(name.substr(l + 1, r)) * VARSIZE;
		pos += -1 * (toNumber(name.substr(3, l))+1) * VARSIZE;
		*resAsm += "	mov si, ss:[bp+" + to_string(pos) + "]\n";
	}
	else {
		int pos = -1*(toNumber(name.substr(3)) + 1) * VARSIZE;
		*resAsm += "	mov si, ss:[bp+" + to_string(pos) + "]\n";
	}	
	return "ss:[si]";
}
string VarManager::IsVar(string name) 
{
	if (varPos.find(name) == varPos.end())
		addVar(name, 1);
	int x = varPos[name];
	return "ss:[bp+" + to_string(x) + "]";
}


string VarManager::IsNumber(string name)
{
	int i = 0;
	while (name[i] && name[i] <= '9'&&name[i] >= '0') i++;
	if (name[i]) {
		if (name[i] != '.') return "";
		i++;
	}
	while (name[i] && name[i] <= '9'&&name[i] >= '0') i++;
	if (name[i]) return "";
	return name;
}
