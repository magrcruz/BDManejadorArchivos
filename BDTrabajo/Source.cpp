#include "Header.h"
#include "comparators.h"
using namespace std;

// ================ IMPLEMENTACIONES ================
//Auxiliares
void Tolower(string& s) {
	string lower;
	for (int i = 0; i < s.size(); i++)
		lower += tolower(s[i]);
	s = lower;
}
void borrarEspacios(string& linea) {
	linea.erase(std::remove(linea.begin(), linea.end(), ' '), linea.end());
}

inline void borrarEspaciosAA(string& linea) {
	size_t i = 0, f = linea.size() - 1;
	while (linea[i++] == ' ');
	while (linea[f--] == ' ');
	linea = linea.substr(i - 1, f - i + 3);
	//+3 porque siempre se le resta 1 a len, al inicio se le resta 1 y como es size y no pos final
}

string convertToString(char* a, int size){
	string s;
	for (int i = 0; i < size; i++)
		s = s + a[i];
	return s;
}


bool existeArchivo(string fname) {
	FILE* file;
	if ((file = fopen(fname.c_str(), "r"))) {//existe
		fclose(file);
		return 1;
	}
	return 0;
}

bool getMetadata(string nombreTabla, string& meta) {
	Tolower(nombreTabla);
	ifstream archivo("metadata.txt");//falta verificar si abrio correctamente
	while (getline(archivo, meta))
		if (meta.find(nombreTabla) != string::npos) return 1;
	return 0;
}

void getDataTypesMeta(string parametros, vector<int>& dataType) {
	for (size_t i = 0, m = parametros.find('-'), f = parametros.find(','); m != string::npos; i = f + 1, m = parametros.find('-', f), f = parametros.find(',', m)) {
		string tipoDato = parametros.substr(m + 1, f - m - 1);
		if (tipoDato == "int") dataType.push_back(-1);
		else if (tipoDato == "date") dataType.push_back(-2);
		else dataType.push_back(stoi(tipoDato.substr(5, tipoDato.size() - 1)));
	}
}

void getDataTypeColMeta(string parametros, vector<int>& dataType, vector<string>& colName) {
	for (size_t i = 0, m = parametros.find('-'), f = parametros.find(','); m != string::npos; i = f + 1, m = parametros.find('-', f), f = parametros.find(',', m)) {
		string nombreCol = parametros.substr(i, m - i);//
		colName.push_back(nombreCol);
		string tipoDato = parametros.substr(m + 1, f - m - 1);
		if (tipoDato == "int") dataType.push_back(-1);
		else if (tipoDato == "date") dataType.push_back(-2);
		else dataType.push_back(stoi(tipoDato.substr(5, tipoDato.size() - 1)));
	}
}

inline bool comprobarTipoDato(string parametros, vector<int> dataTypeMeta) {
	//paso 1 encontrar la coma y crear tu substr del dato
	size_t i = 0, f = parametros.find(',');
	for (int j = 0; f != string::npos; i = f + 1, f = parametros.find(',', i), j++) {
		string col = parametros.substr(i, f - i);
		borrarEspaciosAA(col);
		if (!isType(col, dataTypeMeta[j])) return 0;
	}
	string col = parametros.substr(i);
	borrarEspaciosAA(col);
	if (!isType(col, dataTypeMeta[dataTypeMeta.size() - 1])) return 0;
	return 1;
}

void parsear(string str, vector<string>& palabras, char separator) {
	//Separa el str segun el separador, tokenizar
	size_t i = 0, f = str.find(separator);
	for (; f != string::npos; i = f + 1, f = str.find(separator, i))
		palabras.push_back(str.substr(i, f - i));
	palabras.push_back(str.substr(i));
}

void parsear(string str, vector<string>& palabras, vector<int> datatype){
	//Recibe datatype de la metadata, -1 es int y -2 como fecha
	datatype.push_back(0);
	int i = 0, v = 0, len = 0;
	while (i < str.size()) {
		int dT = datatype[v++];
		if (dT == -1) len = 4;
		else if (dT == -2) len =  10;
		else len = dT;
		palabras.push_back(str.substr(i, len));
		i += len;
	}
	datatype.pop_back();
	//remove last element
}
//Funcion para retornar el tamaño del registro
int getTamRegistro(vector<int> tipodato){
	int tam = 0;
	for (int i = 0; i < tipodato.size(); i++) {
		if (tipodato[i] == -1)
			tam += 4;
		else if (tipodato[i] == -2)
			tam += 10;
		else
			tam += tipodato[i];
	}
	return tam+8;// se suma 8 debido al puntero 
}

//CORE
bool exeCommad(string cmd) {
	size_t espacio = cmd.find(' ');
	string c = cmd.substr(0, espacio);
	Tolower(c);
	cmd = cmd.substr(espacio + 1);//quita la primera palabra
	if (c == "crear") return crear(cmd);
	else if (c == "insertar") return insertar(cmd);
	else if (c == "select") return select(cmd);
	else if (c == "borrar") return borrar(cmd);
	else if (c == "modificar") return modificar(cmd);
	return 0;
}

bool crear(string cmd) {
	cmd = cmd.substr(cmd.find(' ') + 1);//Elimina la palabra tabla
	borrarEspacios(cmd);
	Tolower(cmd);
	//obtener el nombre de la tabla
	size_t i = 0, f = cmd.find('(');
	if (f == string::npos) return 0;//comprueba que este el (
	size_t m = f;

	string nombreTabla = cmd.substr(i, m);//guarda nombre sin espacios
	if (nombreTabla == "") return 0;
	//obtener parametros
	m = cmd.size() - 2;
	if (cmd[m] != ')') return 0;
	cmd = cmd.substr(f + 1, m - f);//deja solo los parametros sin ()
	//Comprobacion
	//comprobar que no exista la tabla
	if (existeArchivo(nombreTabla + ".txt")) return 0;
	//comprobar parametros tipos de datos, no comprueba que ya exista el parametro
	for (i = 0, m = cmd.find('-'), f = cmd.find(','); m != string::npos; i = f + 1, m = cmd.find('-', f), f = cmd.find(',', m)) {
		string tipoDato = cmd.substr(m + 1, f - m - 1);
		if (!(tipoDato == "int" || tipoDato == "date" || tipoDato.substr(0, 4) == "char")) return 0;
	}
	//TO-DO si hay time comprobar que ya exista el parametro
	return 1;
}

bool insertar(string cmd) {
	size_t i = 0, f = cmd.size(), m = cmd.find(' ');
	string meta;
	vector<int> tipos;
	//Paso 1 comprobar tabla existe
	string nombreTabla = cmd.substr(0, m);//falta ver si hay espacios extra
	if (!existeArchivo(nombreTabla + ".txt")) return 0;
	//Paso 2 extraer los parametros de la metadata
	i = cmd.find('(');
	cmd = cmd.substr(i + 1, cmd.find(')') - i - 1);//solo parametros

	//SacaDataMetaData
	getMetadata(nombreTabla, meta);//Extrae linea metadata
	meta = meta.substr(meta.find(':') + 1);
	getDataTypesMeta(meta, tipos);

	//Paso 3 comprobar los parametros correctos
	comprobarTipoDato(cmd, tipos);
	return true;
}

bool select(string cmd) {
	//* DESDE tabla DONDE col operador dato
	//Primero con asterisco y despues ver con parametros
	vector<string> palabras, colName;
	vector<int> tipos;
	string meta;
	parsear(cmd, palabras, ' ');
	int i = findVecMm(palabras, "desde");
	if (i < 0) return 0;
	string nombreTabla = palabras[i + 1];
	//SacaDataMetaData
	getMetadata(nombreTabla, meta);//Extrae linea metadata
	meta = meta.substr(meta.find(':') + 1);//quita el titulo de la tabla
	getDataTypeColMeta(meta, tipos, colName);
	
	if (palabras[i + 2] != "donde") return 0;//Donde opcional a futuro
	string col = palabras[i + 3];//Col que busca
	string op = palabras[i + 4];//Operator = > < para int, para cadena, para fecha
	string value = palabras[i + 5];//Valor con el que comparar
	int posCol= findVecMm(colName, col);
	if (posCol < 0) return 0;
	int tipoDato = tipos[posCol];

	FILE* ptr = fopen( ((nombreTabla + ".txt").c_str()) , "r");
	if (!ptr) return 0;
	int tamRegistro = getTamRegistro(tipos);
	char* aux = new char[tamRegistro];

	while (!feof(ptr)) {
		size_t bytes= fread(aux, sizeof(char), sizeof(char)*tamRegistro, ptr);
		string registro = convertToString(aux, tamRegistro);
		if (tipoDato == -1) {//es numero
			int val = int(value);//to fix
			CompararRegistro(val, posCol, op, registro, tipos);
		}
		else if (tipoDato[posCol] == -2) {
			Fecha date(getcol(str,posCol));
			if (comp(date, value))return 1;
		}
		else
			if (comp(getco, value))return 1;
	}
	/*
	for (; feof(ptr); ptr += tamRegistro) {
		fread(aux, sizeof(char), tamRegistro, ptr);
		cout << *aux << endl;
	}*/
	/*
	if (tipoDato == -1) {//es numero
		int val = int(value);//to fix
		CompararRegistro(val, posCol, op, string registro, tipos);
	}
	else if (tipoDato[posCol] == -2) {
		Fecha date(getcol);
		if (comp(date, value))return 1;
	}
	else
		if (comp(getco, value))return 1;

	CompararRegistro()

	*/
	return true;
}

bool borrar(string cmd)
{
	return false;
}

bool modificar(string cmd){
	return false;
}
//Get col retorna el contenido de la columna requerida
inline string getCol(string registro, int indice,vector<int> tipoDato){
	vector<string> palabras;
	parsear(registro, palabras, tipoDato);
	return palabras[indice];
}

template<typename T>
int findVec(vector<T> vec, T value) {
	for (int i = 0; i < vec.size(); i++)
		if (vec[i] == value) return i;
	return -1;
}

template<typename T>
bool CompararRegistro(T value, int posCol, string op, string registro, vector<int> tipoDato){
	string getco = getCol(registro, posCol, tipoDato);//valor de la columna a comparar
	bool(*comp)(T, T) = 0;//es un puntero a funcion para los operadores
	if (op == "<") comp = menor;
	else if (op == ">") comp = mayor;
	else  comp = igual;
	if (tipoDato[posCol] == -1) {
		int val = int(getco);
		if (comp(val, value)) return 1;
	}
	else if (tipoDato[posCol] == -2) {
		Fecha date(getco);
		if (comp(date, value))return 1;
	}
	else
		if (comp(getco, value))return 1;
	
	return 0;
}

int findVecMm(vector<string> vec, string value) {
	Tolower(value);
	for (int i = 0; i < vec.size(); i++) {
		string str = vec[i];
		Tolower(str);
		if (str == value) return i;
	}
	return -1;
}

int bytesToInt(char* c) {
	int result = 0;
	memcpy(&result, c, sizeof(int));
	return result;
}
