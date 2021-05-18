#include "Header.h"
#include "comparators.h"
using namespace std;

bool borrarRegistro(FILE* ptr) { return 0; };//Alan
bool modificarRegistro(FILE* ptr) { return 0; };//Alan

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
	//Separa el registro segun el separador, tokenizar
	size_t i = 0, f = str.find(separator);
	for (; f != string::npos; i = f + 1, f = str.find(separator, i))
		palabras.push_back(str.substr(i, f - i));
	palabras.push_back(str.substr(i));
}

void parsear(string str, vector<string>& palabras, vector<int> datatype){
	//Recibe datatype de la metadata, -1 es int y -2 como fecha
	datatype.push_back(0);
	int i = 0, v = 0, len = 0;
	while (v < datatype.size()) {
		int dT = datatype[v++];
		if (dT == -1) len = 4;
		else if (dT == -2) len =  10;
		else len = dT;
		palabras.push_back(str.substr(i, len));
		i += len;
	}
	datatype.pop_back(); //remove last element
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
	return tam+4;// se suma 4 debido al siguiente registro
}

void imprimirRegistro(vector<string> columnas, vector<int> tipoDatos){
	//len maxima de un entero=10
	for (int i = 0; i < tipoDatos.size(); i++) {
		int len = tipoDatos[i];
		if (len < 0) {
			if (len == -1) {
				cout << setw(10 + 2) << bytesToInt(&columnas[i][0]);
				continue;
			}
			len = 10;
		}
		cout << setw(len+2) << columnas[i].substr(0,columnas[i].find('\0'));
	}
	cout << endl;
}

int imprimirCabeceras(vector<string> colName, vector<int> tipoDatos){
	int t = 0;
	cout << " ";
	for (int i = 0; i < colName.size(); i++) {
		int len = tipoDatos[i];
		if (len < 0) {
			if (len == -1) len = 4;
			else if (len == -2) len = 10;
		}
		if (colName[i].size() > len) len = colName[i].size();
		cout << setw(len + 2) << colName[i];
		t += len + 2;
	}
	cout << endl <<" "<< setfill('=') << setw(t) <<""<<setfill(' ') << endl;
	return t;
}

void runDB(){
	while (true) {
		string comando;
		cout << "Ingrese el comando: " << endl;
		getline(cin, comando);
		if (comando == "exit") return;
		cout << endl << (exeCommad(comando) ? "Comando ejecutado con exito" : "Hubo un error al ejecutar el comando") << endl ;
	}
}

bool empty(vector<string> vec) {
	for (int i = 0; i < vec.size(); i++)
		if(vec[i][0]!='\0') return 0;//no esta vacio
	return 1;
}
//CORE
bool exeCommad(string cmd) {
	if (!cmd.size()) return 0;//si la string esta vacia
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
	m = cmd.find_last_of(')');
	if (m==string::npos) return 0;
	cmd = cmd.substr(f + 1, m - f-1);//deja solo los parametros sin ()
	//Comprobacion
	//comprobar que no exista la tabla
	if (existeArchivo(nombreTabla + ".txt")) return 0;
	//comprobar parametros tipos de datos, no comprueba que ya exista el parametro
	for (i = 0, m = cmd.find('-'), f = cmd.find(','); m != string::npos; i = f + 1, m = cmd.find('-', f), f = cmd.find(',', m)) {
		string tipoDato = cmd.substr(m + 1, f - m - 1);
		if (!(tipoDato == "int" || tipoDato == "date" || tipoDato.substr(0, 4) == "char")) return 0;
	}
	//TO-DO si hay time comprobar que ya exista el parametro

	//Crear A
	// string a char*
	char *c_t_name=new char[nombreTabla.length() + 1];
	char *c_params = new char[cmd.length() + 1];
	strcpy(c_t_name, nombreTabla.c_str());
	strcpy(c_params, cmd.c_str());

	// guardar la tabla en la metadata
	FILE* metadata = fopen("metadata.txt", "a+");
	fputs(c_t_name, metadata);
	fputc(':', metadata);
	fputs(c_params, metadata);
	fputc('\n', metadata);
	fclose(metadata);

	delete[] c_t_name;
	delete[] c_params;

	// crear archivo con ese nombre
	std::string f_name = nombreTabla + ".txt";
	FILE* stream = fopen(f_name.c_str(), "w+");
	int nnn = 0;
	fwrite(&nnn, sizeof(int), 1, stream);
	fclose(stream);

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

	//A
	string t_name = nombreTabla;
	string data = cmd;
	// buscar tabla en la metadata (asumiendo que existe)
	std::ifstream file("metadata.txt");
	std::string tabla;
	while (std::getline(file, tabla)) {
		if (t_name == tabla.substr(0, tabla.find(':')))
			break;
	}

	// Parseo de la metadata y tam_registro
	std::vector<int> v_tipos;
	int tam_reg = 0;
	char *c_tabla=new char[tabla.length() + 1];
	strcpy(c_tabla, tabla.c_str());
	char* token = strtok(c_tabla, "-,");
	bool b = 0;
	while (token) {
		std::string s(token);
		if (b) {
			if (s == "int") {
				v_tipos.push_back(-2);
				tam_reg += 4;
			}
			else if (s == "date") {
				v_tipos.push_back(-3);
				tam_reg += 10;
			}
			else {
				v_tipos.push_back(std::stoi(s.substr(5, s.size() - 6)));
				tam_reg += v_tipos.back();
			}
		}
		token = strtok(NULL, "-,");
		b = !b;
	}

	// Parseo de la data
	std::vector<std::string> v_data;
	char *c_data = new char[data.length() + 1];
	strcpy(c_data, data.c_str());
	token = strtok(c_data, ",");
	while (token) {
		std::string s(token);
		v_data.push_back(s);
		token = strtok(NULL, ",");
	}

	// apuntar al ultimo puntero
	std::string f_name = t_name + ".txt";
	FILE* stream = fopen(f_name.c_str(), "a+");

	// Convertion and write data
	int tipo;
	for (int i = 0; i < v_tipos.size(); i++) {
		tipo = v_tipos[i];
		if (tipo == -2) {
			int temp = std::stoi(v_data[i]);
			fwrite(&temp, sizeof(temp), 1, stream);
		}
		else if (tipo == -3) {
			fwrite(&v_data[i][0], 10, 1, stream);
		}
		else {
			fwrite(&v_data[i][0], tipo, 1, stream);
		}
	}
	int nnn = 0;
	fwrite(&nnn, sizeof(int), 1, stream);
	fclose(stream);

	delete[] c_tabla;
	delete[] c_data;

	return true;
}

bool select(string cmd) { //* DESDE tabla DONDE col operador dato
	//Primero con asterisco y despues ver con parametros

	//Parsear
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
	

	//Alistar para leer el archivo
	FILE* ptr = fopen(((nombreTabla + ".txt").c_str()), "rb");
	if (!ptr) return 0;
	int tamRegistro = getTamRegistro(tipos);
	string registro(tamRegistro + 1, '\0');
	string inicio(4, '\0');
	// obtain file size:
	fseek(ptr, 0, SEEK_END);
	long lSize = ftell(ptr);
	rewind(ptr);
	if (!lSize) {
		cout << "Tabla vacia" << endl;
		return 1;
	}

	//leer primer numero para ver donde comienza la tabla
	///*
	int start = fread(&inicio[0], 1, 4, ptr);
	if (start != 4) { fputs("Reading error", stderr); exit(3); }
	start = bytesToInt(&inicio[0]);//*/
	//ptr += start * tamRegistro;//puntero inicial del archivo

	string col, op=">", value="-1";//
	int posCol = 0,tipoDato= tipos[0];

	if (palabras.size() > 4) {
		col = palabras[i + 3];//Col que busca
		op = palabras[i + 4];//Operator = > < para int, para cadena, para fecha
		value = palabras[i + 5];//Valor con el que comparar
		posCol = findVecMm(colName, col);//Posicion de la columna que buscamos
		if (posCol < 0) return 0;
		tipoDato = tipos[posCol];
	}

	int t = imprimirCabeceras(colName, tipos),j=0;//Imprimir cabeceras
	//Leer el archivo
	for (int i = 4; i < lSize;) {
	//for (long i = 0; i < lSize; i += tamRegistro) {
		// obtenemos el registro en una string
		fread(&registro[0], 1, sizeof(char) * tamRegistro, ptr);
		/*int result = fread(&registro[0], 1, tamRegistro, ptr);
		if (result != tamRegistro) { fputs("Reading error", stderr); exit(3); }*/
		vector<string> columnas;
		parsear(registro.substr(0,registro.size()-4), columnas, tipos);
		
		//if (!columnas[columnas.size() - 1].size()) 
			i += tamRegistro;
		//else i += bytesToInt(&(columnas[columnas.size() - 1])[0]) * tamRegistro;//para mover segun el puntero
		//if (columnas.size() != tipos.size()+1) continue;
		if (empty(columnas)) continue;
		//Filtrar
		if (comparadita(value, columnas[posCol], tipoDato,op)) {
			imprimirRegistro(columnas, tipos);
			j++;
		}
	}
	cout << " " << setfill('_') << setw(t) << "" << setfill(' ') << endl ;
	cout <<" "<< setw(t)<< (to_string(j) + " Registros obtenidos")<< endl << endl;
	//Falta en el caso del uso de punteros mover esa cantidad de punteros
	return true;
}

bool borrar(string cmd){
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

	//Alistar para leer el archivo
	FILE* ptr = fopen(((nombreTabla + ".txt").c_str()), "rb+");
	if (!ptr) return 0;
	int tamRegistro = getTamRegistro(tipos);
	string registro(tamRegistro + 1, '\0');
	string inicio(4, '\0');
	// obtain file size:
	fseek(ptr, 0, SEEK_END);
	long lSize = ftell(ptr);
	rewind(ptr);
	if (!lSize) {
		cout << "Tabla vacia" << endl;
		return 1;
	}

	int start = fread(&inicio[0], 1, 4, ptr);
	if (start != 4) { fputs("Reading error", stderr); exit(3); }
	start = bytesToInt(&inicio[0]);//*/

	string col, op = ">", value = "-1";//
	int posCol = 0, tipoDato = tipos[0];

	if (palabras.size() > 4) {
		col = palabras[i + 3];//Col que busca
		op = palabras[i + 4];//Operator = > < para int, para cadena, para fecha
		value = palabras[i + 5];//Valor con el que comparar
		posCol = findVecMm(colName, col);//Posicion de la columna que buscamos
		if (posCol < 0) return 0;
		tipoDato = tipos[posCol];
	}
	else return 0;

	//Leer el archivo
	for (int i = 4; i < lSize;) {

		fread(&registro[0], 1, sizeof(char) * tamRegistro, ptr);
		int finReg = ftell(ptr);
		vector<string> columnas;
		parsear(registro.substr(0, registro.size() - 4), columnas, tipos);

		i += tamRegistro;
		if (empty(columnas)) continue;
		
		//Filtrar
		if (comparadita(value, columnas[posCol], tipoDato, op)) {
			erase(nombreTabla, ptr);
			fseek(ptr, finReg, SEEK_SET);
		}
	}
	return true;
}

bool modificar(string cmd){
	//Parsear
	vector<string> palabras, colName;
	vector<int> tipos;
	string meta;
	parsear(cmd, palabras, ' ');
	if (palabras.size() < 9) return 0;

	int i = 0;
	string nombreTabla = palabras[i];
	//SacaDataMetaData
	getMetadata(nombreTabla, meta);//Extrae linea metadata
	meta = meta.substr(meta.find(':') + 1);//quita el titulo de la tabla
	getDataTypeColMeta(meta, tipos, colName);

	//Ignorar SET e = porque siempre va a estar en el mismo lugar
	string colCambiar = palabras[2];//ejemplo nom
	int posColCambiar = findVecMm(colName, colCambiar);
	if (posColCambiar < 0) return 0;
	string valCambiar = palabras[4];//ejemplo 'Renso'

	i = findVecMm(palabras, "donde");
	if (i < 0) return 0;

	string col = palabras[i + 1];//Col que busca
	string op = palabras[i + 2];//Operator = > < para int, para cadena, para fecha
	string value = palabras[i + 3];//Valor con el que comparar
	int posCol = findVecMm(colName, col);//Posicion de la columna que buscamos
	if (posCol < 0) return 0;
	int tipoDato = tipos[posCol];

	//Alistar para leer el archivo
	FILE* ptr = fopen(((nombreTabla + ".txt").c_str()), "rb+");
	if (!ptr) return 0;
	int tamRegistro = getTamRegistro(tipos);
	string registro(tamRegistro, '\0');
	string inicio(4, '\0');

	// obtain file size:
	fseek(ptr, 0, SEEK_END);
	long lSize = ftell(ptr);
	rewind(ptr);

	//leer primer numero para ver donde comienza la tabla
	int start = fread(&inicio[0], 1, 4, ptr);
	if (start != 4) { fputs("Reading error", stderr); exit(3); }
	start = bytesToInt(&inicio[0]);//*/

	//Leer el archivo
	for (int i = 4; i < lSize;) {
		int iniReg = ftell(ptr);
		fread(&registro[0], 1, sizeof(char) * tamRegistro, ptr);
		int finReg = ftell(ptr);
		fseek(ptr, iniReg, SEEK_SET);
		vector<string> columnas;
		parsear(registro.substr(0, registro.size() - 4), columnas, tipos);

		i += tamRegistro;
		if (empty(columnas)) continue;

		//Filtrar
		if (comparadita(value, columnas[posCol], tipoDato, op)) {
			//Sumita para ver en que char modifica
			int posColByte = 0;
			for (int ii = 0; ii < posColCambiar; ii++) {
				if (tipos[ii] == -1) posColByte += 4;
				else if (tipos[ii] == -2) posColByte += 10;
				else posColByte += tipos[ii];
			}
			modify(ptr, posColByte, valCambiar, tipos[posColCambiar]);//Deberia es la pos y que poner alli
			//mod(nombreTabla, ptr, colName[posCol],valCambiar);
		}
		fseek(ptr, finReg, SEEK_SET);
	}
	return true;
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

bool comparadita(string val, string reg, int tipo,string op) {//Val el que llega de usuario y reg el del archivo
	if (tipo == -1) {
		int a = stoi(val);
		int b = bytesToInt(&reg[0]);

		bool(*comp)(int, int) = 0;//es un puntero a funcion para los operadores
		if (op == "<") comp = menor;
		else if (op == ">") comp = mayor;
		else  comp = igual;
		return comp(b,a);
	}
	else if (tipo == -2) {
		Fecha a(val);
		Fecha b(reg);

		bool(*comp)(Fecha, Fecha) = 0;//es un puntero a funcion para los operadores
		if (op == "<") comp = menor;
		else if (op == ">") comp = mayor;
		else  comp = igual;

		return comp(b,a);
	}
	reg = reg.substr(0, reg.find('\0'));
	//val = val.substr(0, val.find('\0'));
	bool(*comp)(string, string) = 0;//es un puntero a funcion para los operadores
	if (op == "<") comp = menor;
	else if (op == ">") comp = mayor;
	else  comp = igual;
	return comp(reg,val);
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

int bytesToInt(char* c) {//Mandar &registro[0]
	int result = 0;
	memcpy(&result, c, sizeof(int));
	return result;
}

bool dropDB() {
	ifstream meta("metadata.txt");
	string linea;
	while (getline(meta,linea)){
		size_t i = linea.find(':');
		remove((linea.substr(0, i) + ".txt").c_str());
	}
	meta.close();
	remove("metadata.txt");
	return 1;
}

//======INICIA MERGE (funciones extra)========
void string_to_charA(std::string s, char* A, int Atam) {
	int min = s.length();
	if (Atam < min) min = Atam;
	for (int i = 0; i < min; i++)
		*A++ = s[i];
	while (min++ < Atam)
		*A++ = '\0';
}


int tam_reg(std::string t_name) {
	int tam = 0;
	std::ifstream file("metadata.txt");
	std::string tabla;
	while (std::getline(file, tabla)) {
		if (t_name == tabla.substr(0, tabla.find(':')))
			break;
	}
	char *c_tabla=new char[tabla.length() + 1];
	strcpy(c_tabla, tabla.c_str());
	char* token = strtok(c_tabla, "-,");
	bool b = 0;
	while (token) {
		std::string s(token);
		if (b) {
			if (s == "int") tam += 4;
			else if (s == "date") tam += 10;
			else tam += std::stoi(s.substr(5, s.size() - 6));
		}
		token = strtok(NULL, "-,");
		b = !b;
	}
	delete[] c_tabla;
	return tam;
}


bool esnulo(char* array, int tam) {
	for (int i = 0; i < tam; i++)
		if (array[i]) return 0;
	return 1;
}

long get_size(std::string file) {
	long size;
	FILE* pFile = fopen(file.c_str(), "rb");
	if (pFile == NULL) return 0;
	else {
		fseek(pFile, 0, SEEK_END);
		size = ftell(pFile);
		fclose(pFile);
	}
	return size;
}

int erase(std::string t_name, FILE*& stream) {
	// paso = tam de registro + 4
	int paso = tam_reg(t_name) + 4;
	int f_size = get_size(t_name + ".txt");

	int fdsaf = 0;

	int reg_pos = ftell(stream);
	int reg_x = 0;
	int reg_y = 0;

	int x = 0;
	int y = 0;
	char* p = new char[paso];
	// get x
	while (ftell(stream) > paso) {
		if (ftell(stream) == 4) {
			fseek(stream, -4, SEEK_CUR);
			reg_x = 0;
			fread(&x, sizeof(int), 1, stream);
			break;
		}
		fseek(stream, -paso, SEEK_CUR);
		fread(p, 1, paso, stream);
		if (!esnulo(p, paso)) {
			fdsaf++;
			if (fdsaf == 1) {
				fseek(stream, -paso, SEEK_CUR);
				reg_pos = ftell(stream);
				fseek(stream, paso, SEEK_CUR);
			}
			else if (fdsaf == 2) {
				fseek(stream, -4, SEEK_CUR);
				reg_x = ftell(stream);
				fread(&x, sizeof(int), 1, stream);
				break;
			}
		}
		fseek(stream, -paso, SEEK_CUR);
	}
	delete[] p;
	// get y
	fseek(stream, 0, SEEK_SET);
	fseek(stream, reg_pos + paso - 4, SEEK_SET);
	reg_y = ftell(stream);
	fread(&y, sizeof(int), 1, stream);

	// borrar registro
	fseek(stream, reg_pos, SEEK_SET);
	for (int i = 0; i < paso; i++)
		fputc('\0', stream);

	// calculo de la distancia
	int dis = x + y + 1;

	// caso especial: solo 1 registro
	if (f_size == 4 + paso) { dis = 0; }

	// actualizar int del registro anterior
	fseek(stream, reg_x, SEEK_SET);
	fwrite(&dis, sizeof(int), 1, stream);
	return 1;
}
void print_tam(std::string file) {
	FILE* pFile;
	long size;

	pFile = fopen(file.c_str(), "rb");
	if (pFile == NULL) perror("Error opening file");
	else {
		fseek(pFile, 0, SEEK_END);
		size = ftell(pFile);
		fclose(pFile);
		printf("Size of %s: %ld bytes.\n", file.c_str(), size);
	}
}

/*
int mod(string t_name, FILE*& stream, string col, string valor) {
	// buscar tabla en la metadata (asumiendo que existe)
	std::ifstream file("metadata.txt");
	std::string tabla;
	while (std::getline(file, tabla)) {
		if (t_name == tabla.substr(0, tabla.find(':')))
			break;
	}

	tabla = tabla.substr(tabla.find(':') + 1, tabla.size() - tabla.find(':') - 1);

	// Parseo de la metadata
	std::vector<int> v_tipos;
	std::vector<std::string> columnas;
	char* c_tabla = new char[tabla.length() + 1];
	strcpy(c_tabla, tabla.c_str());
	char* token = strtok(c_tabla, "-,");
	bool b = 0;
	while (token) {
		std::string s(token);
		if (b) {
			if (s == "int") {
				v_tipos.push_back(-2);
			}
			else if (s == "date") {
				v_tipos.push_back(-3);
			}
			else {
				v_tipos.push_back(std::stoi(s.substr(5, s.size() - 6)));
			}
		}
		else {
			columnas.push_back(s);
		}
		token = strtok(NULL, "-,");
		b = !b;
	}
	int pos = 0;
	while (pos < columnas.size()) {
		if (columnas[pos] == col) break;
		pos++;
	}

	// funcion para parsear string al tipo de dato q sea v_tipos[pos]
	// valor = funcion_tipo(valor, v_tipos[pos]);
	// asumo q ya la convirtieron

	int tipo = v_tipos[pos];
		int tam = v_tipos[pos];
	if (tam == -2) tam = 4;
	if (tam == -3) tam = 10;

	int espacio_prev = 0;
	for (int i = 0; i < pos; i++) {
		if (v_tipos[i] == -2) espacio_prev += 4;
		else if (v_tipos[i] == -3) espacio_prev += 10;
		else espacio_prev += v_tipos[i];
	}

	fseek(stream, espacio_prev, SEEK_CUR);
	fwrite(&valor, tam, 1, stream);

	return 1;
}*/

bool modify(FILE* ptr, int pos, string value, int dataType) {
	int posPtr = ftell(ptr);
	fseek(ptr, pos, SEEK_CUR);

	if (dataType == -1) {
		int n = stoi(value);
		fwrite(&n, 1, sizeof(int), ptr);
	}
	else if (dataType == -2) {
		Fecha f(value);
		fwrite(&f.fecha[0], 1, 10, ptr);
	}
	else {
		fwrite(&value[0], 1, dataType, ptr);
	}
	fseek(ptr, posPtr, SEEK_SET);
	return 1;
}