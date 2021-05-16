#pragma once
#pragma warning(disable : 4996)
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

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

template <typename T>
int findVec(vector <T> vec, T value);
int findVecMm(vector<string> vec, string value);

template <typename T>
bool CompararRegistro(T value,int posCol,string operador,string registro, vector<int> tipoDato);

int bytesToInt(char* c);

//CORE
bool exeCommad(string cmd);

bool crear(string cmd);
bool insertar(string cmd);
bool select(string cmd);
bool borrar(string cmd);
bool modificar(string cmd);
string getCol(string registro, int indice, vector<int> tipoDato);
