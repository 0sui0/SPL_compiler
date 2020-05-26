#pragma once
#include<string>
#include<cstring>
#include<iostream>

class IRcode {
public:
	static enum OPTION {
		MALLOC, ARRAY, ADDRESS, VALUE, INT, REAL,
		POSIVITE, NEGATIVE, NEG_LOGIC,
		ADD_I, MINUS_I, MULT_I, DIV_I, MOD_I,
		ADD_F, MINUS_F, MULT_F, DIV_F,
		GT, LT, GE, LE,
		AND_LOGIC, OR_LOGIC,
		AND_BIT, OR_BIT, XOR_BIT, SL, SR,
		LABEL, GOTO, IF, 
		FUNC, PARAM, CALL, RETURN, CALL_V, RETURN_V
	};
	OPTION op;
	std::string v1, v2, v3;
	
	
};

