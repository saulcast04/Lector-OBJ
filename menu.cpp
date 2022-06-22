#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <list>
#include <vector>
#include <sstream>
#include <string.h>
#include <cstring>
#include <ctype.h>


/*Se declaran los objetos del programa*/
#include "vectores.hpp"
#include "objetos.hpp"
#include "caras.hpp"

using namespace std;

void lector();
void muestraDatos(vector<objetos>vo, vector<vectores>vv, vector<caras>vc);

int main()
{
 lector(); 
 return 0;
}

//Funcion que nos permite leer el documento, ya sea desde la ruta actual
//o desde una ruta diferente
// Esta misma función se encargará de almacenar los datos en las listas
 

void lector()
{
 // Declaramos las listas para almacenar los datos de cada objeto	
 vector<objetos>vo;
 vector<vectores> vv;
 vector<caras>vc;
 
 //Declaramos los iteradores para poder almacenar los datos a las listas

 

 int c1, c2, c3, c4;
 double cx, cy, cz;

 ifstream archivo;			//variable para abrir el archivo
  
 //variables donde se guarda el nombre del archivo
 //la información del archivo
 //y el nombre del objeto
 string nombre, texto, info, nomObj;	
 
 
//Declaramos la variable de tipo stringstream
// Esta variable nos sirve para tratar una cadena como arreglo
// Lo que sirve para saber en donde guardar los datos
 stringstream datos;
 
 
 cout<< "coloque el nombre del archivo: ";
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

	 if(texto[0] == 'o')
	 {
	  info = texto.substr(2);  //Elimina los 2 primeros datos de la cadena
	  datos<<info;
	  datos>>nomObj;
	  
	  objetos o = objetos(nomObj);
	  vo.push_back(o);
	  
	 }
	 
	 if(texto[0] == 'v')
	 {
	  info = texto.substr(2);
	  datos<<info;

	  datos >> cx;
	  datos >> cy;
	  datos >> cz; //Almacena cada parte de la cadena en las variables
	  vectores v = vectores(cx, cy, cz);
	  vv.push_back(v);
	 }
	 
	 if(texto[0] == 'f')
	 {
	  info = texto.substr(2);
      datos<<info;

	  datos >> c1;
	  datos >> c2;
	  datos >> c3;
	  datos >> c4;	  
	  caras c = caras(c1, c2, c3, c4);
	  vc.push_back(c);
	 }
 }
 
 archivo.close();
 
 muestraDatos(vo, vv, vc);
}


//FUNCION QUE NOS MUESTRA LOS DATOS EN PANTALLA

void muestraDatos(vector<objetos>vo, vector<vectores>vv, vector<caras>vc)
{
	
 vector <objetos>:: iterator objIt;	
 vector <vectores>:: iterator vecIt;
 vector <caras>:: iterator carIt;
 
 objIt = vo.begin();
 vecIt = vv.begin();
 carIt = vc.begin();
 
 while(objIt != vo.end())
 {
  objIt->muestraObjeto();
  
   while(vecIt != vv.end())
  {
   vecIt->muestraVector();
   vecIt++;
  }
 
  while(carIt!= vc.end())
  {
   carIt->muestraCarasCuad();
   carIt++;
  }
  
  objIt ++ ;
 }
}



/*PARA PODER REALIZAR LA TOKENIZACION DE CARACTERES
 * 
 * void tokenizar()
 * {
 *  string contenido; "donde tendremos el contenido"
 *  char delimitador = ' ' "Donde tendremos el caracter que separará la cadena"
 *  string *token; "será la cadena donde se guardaran las partes de la cadena original"
 *  
 *  token = strtkn(contenido, delimitador);  "la cadena token guarda el contenido de la cadena contenido siendo separada por el delimitador"
 * 
 * 
 *  if(token==NULL) "con esto decimos que el token sea igual a null"
 *  {
 *   cout << "No hay tokens \n";
 *  }
 *  else
 *  {
 *   atoi(token); "convierte el token en entero, si necesitamos flotantes o doubles se usa atof"
 *   
 *  }
 * }
 */
