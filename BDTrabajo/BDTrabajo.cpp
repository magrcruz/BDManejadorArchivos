#include <iostream>
#include "Header.h"
//Siempre dejar espacios en el borrar y select
int main(){
    //
    //runDB();
    ///*
    dropDB();
    exeCommad("Crear TABLA name (id_alumno-int,nombre-char(10),apellido-char(15),fecha_nac-DATE)");
    //Insertar varios cosos
    exeCommad("Insertar name (1,Juana,Perez,12/02/2002)");
    exeCommad("Insertar name (2,Juan,Perez,01/02/2002)");
    exeCommad("Insertar name (3,Rick,Fernandez,13/04/2001)");
    exeCommad("Insertar name (4,Pepe,Fernandez,13/06/2001)");
    exeCommad("Insertar name (5,Nicoll,Fernandez,02/04/2003)");
    exeCommad("Insertar name (6,Potter,Fernandez,13/06/2002)");
    //*/

    string comando;
    //comando = "Crear TABLA name   (id_alumno-int,nombre-char(10),apellido-char(15),fecha_nac-DATE)";
    //comando = "Insertar name (3,Juana,Perez,12/02/2002)";
    ///*//Falta que lo ingresee con comilla
    //comando = "select * desde name";
    ///*
    cout << exeCommad("select * desde name") << endl;
    exeCommad("borrar desde name donde id_alumno < 3");
    /*cout << exeCommad("select * desde name") << endl;
    exeCommad("borrar desde name donde id_alumno == 2");
    cout << exeCommad("select * desde name") << endl;
    exeCommad("borrar desde name donde id_alumno == 3");
    cout << exeCommad("select * desde name") << endl;
    exeCommad("borrar desde name donde id_alumno == 1");*/
    cout << exeCommad("select * desde name") << endl;
}
