#include "objetos.hpp"
#include <iostream>
#include <string>

using namespace std;

objetos::objetos()
{
 
}

void objetos::obtenObjeto(string nombre)
{
 name = nombre;
}

void objetos::muestraObjeto()
{
 cout <<" o " << name << "\n";
}

string objetos::darObjeto()
{
 return name;
}
