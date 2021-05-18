#pragma once
#pragma warning(disable : 4996)
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cstdio>
using namespace std;

// ================ DECLARACIONES ================
//AUXILIARES
void Tolower(string& s);
void borrarEspacios(string &linea);
void borrarEspaciosAA(string& linea);
string convertToString(char* a, int size);

bool existeArchivo(string name);
bool getMetadata(string nombreTabla, string& meta);
void getDataTypesMeta(string parametros, vector<int> &dataType);
void getDataTypeColMeta(string parametros, vector<int>& dataType, vector<string>& colName);
bool comprobarTipoDato(string parametros, vector<int> dataTypeMeta);
void parsear(string str, vector<string> &palabras, char separator=' ');
void parsear(string str, vector<string>& palabras, vector<int> datatype);
int getTamRegistro(vector<int>tipodato);

void imprimirRegistro(vector<string> columnas, vector<int> tipoDatos);
int imprimirCabeceras(vector<string> colName, vector<int> tipoDatos);

template <typename T>
int findVec(vector <T> vec, T value);
int findVecMm(vector<string> vec, string value);

//No usamos comparar Registro
template <typename T>
bool CompararRegistro(T value,int posCol,string operador,string registro, vector<int> tipoDato);
bool comparadita(string val, string reg, int tipo, string op);
int bytesToInt(char* c);//Mandar &str[0]
bool empty(vector<string> vec);
//CORE
void runDB();
bool dropDB();
bool exeCommad(string cmd);

bool crear(string cmd);
bool insertar(string cmd);
bool select(string cmd);
bool borrar(string cmd);
bool modificar(string cmd);
string getCol(string registro, int indice, vector<int> tipoDato);
bool modify(FILE* ptr, int pos, string value, int dataType);
//A
void string_to_charA(std::string s, char* A, int Atam);
int tam_reg(std::string t_name);
bool esnulo(char* array, int tam);
long get_size(std::string file);
int erase(std::string t_name, FILE*& stream);
int mod(string t_name, FILE*& stream, string col, string valor);