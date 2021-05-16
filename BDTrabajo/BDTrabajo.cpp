#include <iostream>
#include "Header.h"

void string_to_charA(std::string s, char* A, int Atam) {
    int min = s.length();
    if (Atam < min) min = Atam;
    for (int i = 0; i < min; i++) {
        *A++ = s[i];
    }
    while (min++ < Atam) {
        *A++ = '\0';
    }
}

void create_file() {
    // escribir en metadata:
    // alumnos:id_al-int;nombre-char[12];semestre-int

    FILE* stream = fopen("alumno.txt", "w");
    long pos = long(stream) + 8; // ubicacion del primer registro
    fwrite(&pos, sizeof(long), 1, stream);
    int id_al = 35;
    fwrite(&id_al, sizeof(int), 1, stream);
    char name[12];
    string_to_charA("Pedro", name, 12);
    fwrite(name, sizeof(name), 1, stream);
    int sem = 2;
    fwrite(&sem, sizeof(int), 1, stream);
    fputs("\0\0\0\0\0\0\0\0", stream); //ptr nulo que significa fin de los registros
    fclose(stream);
}

int main(){
    /*
    string comando = "Insertar alumno   (5,'Juan','Perez','13/05/2017')";
    //string comando = "Crear TABLA name   (id_alumno-int,nombre-char(10),apellido-char(15),fecha_nac-DATE)";
    cout << (exeCommad(comando)?"Comando ejecutado con exito":"Hubo un error al ejecutar el comando") << endl;
    //*/
    /*
    string param = "id_alumno-int,nombre-char(10),apellido-char(15),fecha_nac-date";
    vector<int> dataType;
    vector<string> colName;
    getDataTypeColMeta(param, dataType,colName);
    cout << "oi";//*/
    ///*
    string comando = "Select * Desde alumno donde id_al = 5";
    cout << (exeCommad(comando) ? "Comando ejecutado con exito" : "Hubo un error al ejecutar el comando") << endl;//*/

}