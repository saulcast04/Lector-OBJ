#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>

using namespace std;

void lectura();

int main(int argc, char **argv)
{
 lectura();
}


void lectura()
{
 ifstream archivo;
 string nombre, texto;
 
 cout<< "coloque el nombre del texto: ";
 getline(cin, nombre);
 
 archivo.open(nombre, ios::in);		/*Modo lectura*/
 
 if(archivo.fail())
 {
  cout <<"error, el archivo no pudo abrirse";
  exit(1);
 }
 
 while(!archivo.eof())
 {
	 getline(archivo, texto);
	 cout<<texto<<endl;
 }
 
 archivo.close();
}
