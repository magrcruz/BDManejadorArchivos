#pragma once
#include <string>
using namespace std;

bool isNumber(const string& str);
bool isFecha(string str);
bool isChar(string str, int s);

bool isNumber(const string & str) {
	for (char const& c : str)
		if (std::isdigit(c) == 0) return false;
	return true;
}

bool isFecha(string str) {
	//'16/02/1998' o '09/09/2001'
	if (!(str[0] == '\'' && str[str.size() - 1] == '\'')) return 0;
	str = str.substr(1, str.size() - 2);//quita ' '
	if (str.size() != 10) return 0;
	if (!(str[2] == '/' && str[5] == '/')) return 0;
	if (!isNumber(str.substr(0, 2))) return 0;
	if (!isNumber(str.substr(3, 2))) return 0;
	if (!isNumber(str.substr(6, 4))) return 0;
	return 1;
}

bool isChar(string str, int s) {
	if (!(str[0] == '\'' && str[str.size() - 1] == '\'')) return 0;//verifica comilla
	str = str.substr(1, str.size() - 2);//quita ' '
	if (str.size() > s) return 0;
	return 1;
}

bool isType(string str, int type) {
	if (type == -1) { //es int
		if (!isNumber(str)) return 0;
	}
	else if (type == -2) {
		if (!isFecha(str)) return 0;
	}
	else {
		if (!isChar(str, type)) return 0;
	}
	return 1;
}

template <typename T>
bool mayor(T a, T b) { return a > b; }
template <typename T>
bool menor(T a, T b) { return a < b; }
template <typename T>
bool igual(T a, T b) { return a == b; }

struct Fecha {
	int d = 0, m = 0, a = 2000;
	Fecha(int _d, int _m, int _a) : d(_d), m(_m), a(_a) {};
	Fecha(string str) {
		if (!(str[0] == '\'' && str[str.size() - 1] == '\''))//si hay comillas
			str = str.substr(1, str.size() - 2);//quita ' '
		if (str.size() != 10) return;
		if (!(str[2] == '/' && str[5] == '/')) return;
		string s[3] = { str.substr(0, 2), str.substr(3, 2),str.substr(6, 4) };
		if (isNumber(s[0]) && isNumber(s[1]) && isNumber(s[2])) {
			d = stoi(s[0]);
			m = stoi(s[1]);
			a = stoi(s[2]);
		}
	}
	//static bool isFecha(string str);
	friend bool operator >(const Fecha& f1, const Fecha& f2) {
		if (f1.a > f2.a) return true;
		else if (f1.a == f2.a) {
			if (f1.m > f2.m) return true;
			else if (f1.m == f2.m) return f1.d > f2.d;
		}
		return false;
	}
	friend bool operator <(const Fecha& f1, const Fecha& f2) {
		if (f1.a < f2.a) return true;
		else if (f1.a == f2.a) {
			if (f1.m < f2.m) return true;
			else if (f1.m == f2.m) return f1.d < f2.d;
		}
		return false;
	}
	friend bool operator ==(const Fecha& f1, const Fecha& f2) {
		return (f1.a == f2.a && f1.m == f2.m && f1.d < f2.d);
	}
};
