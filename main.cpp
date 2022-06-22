#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <math.h>

/*Se declaran los objetos del programa*/
#include "vectores.hpp"
#include "objetos.hpp"
#include "caras.hpp"

/*Se manda a llamar las bibliotecas de openGL*/
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GLFW/glfw3.h>

using namespace std;

 vector<objetos>listObjt;		//Cada uno de estas listas
 vector<vectores>listVec;		//nos permite almacenar la informacion
 vector<caras>listCara;			//de los archivos en la memoria	
 
 float t = 0;					//variable t que funcionara para realizar la traslacion de bezier
 
 vectores colorBoom, colorCol;


/*PROTOTIPOS DE LAS FUNCIONES QUE SE USARAN EN EL PROGRAMA*/
void lector();
void muestraDatos();
void dibujar(int argc, char **argv);
void Draw();
void translacion();
void girarX(float grados);
void girarY(float grados);
void girarZ(float grados);
void normales();
void teclado(unsigned char key, int x, int y);
void lookAt();
void perspectiva();
void luzYsombra(caras c);

////////////////////////////////////////////*FUNCION PRINCIPAL*/////////////////////////////////////////
int main(int argc, char **argv)
{
 lector();
 muestraDatos();
 normales();
 dibujar(argc, argv);
 return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////



/* La siguiente funcion sirve para abrir los archivos
 * y guardar la informacion en memoria
 * esta misma funcion manda a llamar el programa para imprimir los datos
 * en pantalla
 */
 
void lector()
{
 int contObj, contVer;
 double cx, cy, cz;
 int indiceCara;
 ifstream archivo;
 string nombre, texto, info, nomObj;
 
 
 
 objetos obj;
 vectores vec;
 caras car;
 
 contObj= contVer = 0;	//se inicializan los contadores a 0
 
 archivo.open("boomerang.obj", ios::in);		/*Modo lectura*/
 
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
	  stringstream datos;
	  if(contObj >0) //verifica si ya se ha leido el primer objeto en el archivo
	  {
	   listObjt.push_back(obj);
	  }
	  info = texto.substr(2);
	  datos << info;
	  datos >> nomObj;
	  objetos newObj; 
	  newObj.obtenObjeto(nomObj); //almacena el nombre del objeto
	  obj= newObj;
	  
	  contObj++;
	 }
	 
	 if(texto[0] == 'v')
	 {
	  stringstream coordenadas; //sirve para capturar las coordenadas de los vectores
		
	  contVer++;
	  info = texto.substr(2);
	  coordenadas << info;
	  coordenadas >> cx >> cy >> cz;
	  vectores newVec;
	  newVec.obtenVectores(cx, cy, cz);
	  vec = newVec;
	  vec.id = contVer;
	  listVec.push_back(vec);
	  obj.vecObj.push_back(contVer);
	 }
	 
	 if(texto[0] == 'f')
	 {
	  stringstream indice;
	  info = texto.substr(2);
	  indice << info;
	  caras newCara;
	  while(!indice.eof())
	  {
	   indice >> indiceCara;
	   newCara.indice.push_back(indiceCara);
	  }
	  car = newCara;
	  listCara.push_back(car);
	  obj.carObj.push_back(car);
	  
	 }

	 
 }
 listObjt.push_back(obj); //agrega el ultimo objeto que se encuentra en el archivo
 archivo.close();
}

/*
 *En esta funcion, lo que se hace es solamente mostrar los datos en pantalla
 * Esta funcion es llamada unicamente por la función lector para evitar problemas 
 * con el uso de las listas 
 */


void muestraDatos()
{
	
 vector <objetos>:: iterator objIt;	
 vector <vectores>:: iterator vecIt;
 vector <caras>:: iterator carIt;
 /* Se utilizan los iteradores para recorrer las listas
  * que se utilizaron anteriormente
  */
  
 objIt = listObjt.begin();
 vecIt = listVec.begin();
 carIt = listCara.begin();
 
 while(objIt != listObjt.end())
 {
  objIt->muestraObjeto();
  objIt ++;
   while(vecIt != listVec.end())
  {
   vecIt->muestraVector();
   vecIt++;
  }
 
  while(carIt!= listCara.end())
  {
   carIt->muestraCaras();
   carIt++;
  }
 }
}

/* A continuacion se mostrará una funcion
 * la cual nos servirá para mostrar los objetos
 * en pantalla
 * Para esto, se usaran las librerías de openGL
 */

void dibujar(int argc, char **argv)
{
 int ancho = 640, alto = 480;
 
 glutInit(&argc, argv);
 glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
 glDepthFunc(GL_LESS);		//funciones para
 glEnable(GL_DEPTH_TEST);	//el modelado de los objetos
 glutInitWindowSize(ancho, alto);
 glutCreateWindow("lectorOBJ");
 
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 perspectiva();		//funcion de perspectiva
 glClearColor(0, 0.5, 0.5, 1); 		//color de fondo
 
  
 glutDisplayFunc(Draw);
 glutKeyboardFunc(teclado);
 glutIdleFunc(Draw);
 glutMainLoop();
}


/*
 * funcion que tendrá todos los datos a dibujar
 * de las listas, razón por la que las listas son
 * globales
 */




void Draw()
{
	vector <objetos>:: iterator objIt;	
	vector <vectores>:: iterator vecIt;
	vector <caras>:: iterator carIt;
	vector <int>:: iterator indiceVer;
	
	
	float x, y, z;
	string nombre;
	
	
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);	//funcion que igualmente ayuda al modelado
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	lookAt();

	glRotatef(180, 0, 0, 1);  //funciones de openGL utilizadas
	glRotatef(15, 1, 0, 0);  //para poder dibujar el objeto en un 
	glRotatef(30, 0, 1, 0); //angulo en el que se pueda ver en 3D

	
	objIt = listObjt.begin();
    while(objIt != listObjt.end())
	{
	 nombre = objIt->darObjeto();
	 carIt = objIt->carObj.begin();
	 while(carIt != objIt->carObj.end())
	 {
	  glBegin(GL_POLYGON);
	  indiceVer = carIt->indice.begin();
	  while(indiceVer != carIt->indice.end())
	  {
	   vecIt = listVec.begin();
	   advance(vecIt, *indiceVer - 1);
	   if(nombre== "boomerang")
	   {
	    x = vecIt->darX(); //Se recolectan primero
	    y = vecIt->darY(); //los datos de los vertices
	    z = vecIt->darZ(); //para hacer la traslación
		luzYsombra(*carIt); //aplicamos la iluminacion y sombreado
		
	    glColor3f(colorBoom.darX(), colorBoom.darY(), colorBoom.darZ()); //x, y, z, serviran como, R, G, B
	    glVertex3f(x, y, z);
	    indiceVer++;
	   }
	   else if(nombre == "columna")
	   {
	    x = vecIt->darX(); //se recolectan nuevamente para
	    y = vecIt->darY(); //poder dibujar los vertices
	    z = vecIt->darZ(); //ya trasladados
	    luzYsombra(*carIt);
	    
	    glColor3f(colorCol.darX(), colorCol.darY(), colorCol.darZ());
	    glVertex3f(x, y, z);
	    indiceVer++;
	   }
	  }
	  glEnd();
	  carIt++;
	 }
	 objIt++;
	}
 glutSwapBuffers();
 glFlush();
 translacion();
 normales();
}


/*Funcion para realizar la translación de una curva de bezier*/
void translacion()
{
	vector <objetos>:: iterator objIt;	
	vector <vectores>:: iterator vecIt;
	vector <caras>:: iterator carIt;
	vector <int>:: iterator indiceVer;
	
	vectores p1, p2, p3, p4;  	//puntos que se usaran como puntos de control de la curva
	p1.obtenVectores(-0.004, 0, 0);  			//valores en x, y, z
	p2.obtenVectores(-0.003, -0.003, -0.003);	//de cada uno de los 
	p3.obtenVectores(-0.001, -0.003, -0.003);	//puntos de control de 
	p4.obtenVectores(0.02, -0.02, 0.02);		//la curva de bezier
	
	float x, y, z, w =1;
	float bx, by, bz;
	string nombre;
	float resultado[4];
	
	objIt = listObjt.begin();
    while(objIt != listObjt.end())
	{
	 nombre = objIt->darObjeto();
	 carIt = objIt->carObj.begin();
	 while(carIt != objIt->carObj.end())
	 {
	  indiceVer = carIt->indice.begin();
	  while(indiceVer != carIt->indice.end())
	  {
	   vecIt = listVec.begin();
	   advance(vecIt, *indiceVer - 1);
	   
	   if(nombre == "boomerang")
	   {
		 x = vecIt->darX();
		 y = vecIt->darY();
		 z = vecIt->darZ();
	     
	     //calculamos el siguiente punto de la curva de bezier mientras el valor de t sea menor a 1
	     if(t< 1)
	     {
		  bx = (pow((1 - t), 3)) * p1.darX() + (pow(((3 * t)*(1 - t)), 2))* p2.darX() + (pow((3*t), 2))*(1 - t)*p3.darX() + (pow(t, 3))*p4.darX();
	      by = (pow((1 - t), 3)) * p1.darY() + (pow(((3 * t)*(1 - t)), 2))* p2.darY() + (pow((3*t), 2))*(1 - t)*p3.darY() + (pow(t, 3))*p4.darY();
	      bz = (pow((1 - t), 3)) * p1.darZ() + (pow(((3 * t)*(1 - t)), 2))* p2.darZ() + (pow((3*t), 2))*(1 - t)*p3.darZ() + (pow(t, 3))*p4.darZ();
		 }
		 else
		  w = 0;	//detenemos el objeto si el valor de t es igual o mayor a 1
	     
	     float vector[4] = {x, y, z, w};
	     float traslacion[4][4]= {{1, 0, 0, bx}, {0, 1, 0, by}, {0, 0, 1, bz}, {0, 0, 0, 1}};
		 
		 resultado[0] = ( (traslacion[0][0] * vector[0]) + (traslacion[0][1] * vector[1]) + (traslacion[0][2] * vector[2]) + (traslacion[0][3] * vector[3]) );
		 resultado[1] = ( (traslacion[1][0] * vector[0]) + (traslacion[1][1] * vector[1]) + (traslacion[1][2] * vector[2]) + (traslacion[1][3] * vector[3]) );
		 resultado[2] = ( (traslacion[2][0] * vector[0]) + (traslacion[2][1] * vector[1]) + (traslacion[2][2] * vector[2]) + (traslacion[2][3] * vector[3]) );
		 resultado[3] = ( (traslacion[3][0] * vector[0]) + (traslacion[3][1] * vector[1]) + (traslacion[3][2] * vector[2]) + (traslacion[3][3] * vector[3]) );
		 
		 vecIt->obtenVectores(resultado[0], resultado[1], resultado[2]);
		 
	   }
	   indiceVer++;
	  }
	  carIt++;
	 }
	 objIt++;
	} 
	t+= 0.001;
}

//las funciones de rotacion de x, y, z solo se aplican para la columna


//funcion para rotar la columna en X
void girarX(float grados)
{
	vector <objetos>:: iterator objIt;	
	vector <vectores>:: iterator vecIt;
	vector <caras>:: iterator carIt;
	vector <int>:: iterator indiceVer;
	
	float x, y, z, w =1;
	string nombre;
	float matrizX[4][4] = {{1, 0, 0, 0}, {0, cos(grados), sin(-grados), 0}, {0, sin(grados), cos(grados), 0}, {0, 0, 0, 1}};
	float resultado[4];
	float mover[4];
	
	objIt = listObjt.begin();
	nombre = objIt->darObjeto();
    while(objIt != listObjt.end())
	{
	 nombre = objIt->darObjeto();
	 carIt = objIt->carObj.begin();
	 while(carIt != objIt->carObj.end())
	 {
	  indiceVer = carIt->indice.begin();
	  while(indiceVer != carIt->indice.end())
	  {
	   vecIt = listVec.begin();
	   advance(vecIt, *indiceVer - 1);
	   if (nombre == "columna")
	   {
	    x = vecIt->darX();
	    y = vecIt->darY();
	    z = vecIt->darZ();
	    float vector[4] = {x, y, z, w};
	    float centro[4][4] = {{1, 0, 0, (-x/2)}, {0, 1, 0, (-y/2)}, {0, 0, 1, (-z/2)}, {0, 0, 0, 1}};  //creamos la matriz para llevar el vertice al centro
		float regresar[4][4] = { {1, 0, 0, (x/2)}, {0, 1, 0, (y/2)}, {0, 0, 1, (z/2)}, {0, 0, 0, 1} }; //creamos la matriz para regresar el vertice a su posicion original
		
		mover[0] = ( centro[0][0]* vector[0] + centro[0][1] * vector[1] + centro[0][2] * vector[2] + centro[0][3] * vector[3] ); //movemos los valores
		mover[1] = ( centro[1][0]* vector[0] + centro[1][1] * vector[1] + centro[1][2] * vector[2] + centro[1][3] * vector[3] ); //de X, Y, Z al centro
		mover[2] = ( centro[2][0]* vector[0] + centro[2][1] * vector[1] + centro[2][2] * vector[2] + centro[2][3] * vector[3] ); 
		mover[3] = ( centro[3][0]* vector[0] + centro[3][1] * vector[1] + centro[3][2] * vector[2] + centro[3][3] * vector[3] );
		
	    resultado[0] = ( (matrizX[0][0] * mover[0]) + (matrizX[0][1] * mover[1]) + (matrizX[0][2] * mover[2]) + (matrizX[0][3] * mover[3]) ); //realizamos la rotacion
	    resultado[1] = ( (matrizX[1][0] * mover[0]) + (matrizX[1][1] * mover[1]) + (matrizX[1][2] * mover[2]) + (matrizX[1][3] * mover[3]) ); //del vertice desde el 
	    resultado[2] = ( (matrizX[2][0] * mover[0]) + (matrizX[2][1] * mover[1]) + (matrizX[2][2] * mover[2]) + (matrizX[2][3] * mover[3]) ); //punto origen o el centro
	    resultado[3] = ( (matrizX[3][0] * mover[0]) + (matrizX[3][1] * mover[1]) + (matrizX[3][2] * mover[2]) + (matrizX[3][3] * mover[3]) ); 
		
		mover[0] = ( regresar[0][0]* resultado[0] + regresar[0][1] * resultado[1] + regresar[0][2] * resultado[2] + regresar[0][3] * resultado[3] ); //regresamos el vertice
		mover[1] = ( regresar[1][0]* resultado[0] + regresar[1][1] * resultado[1] + regresar[1][2] * resultado[2] + regresar[1][3] * resultado[3] ); //rotado ya a su posicion
		mover[2] = ( regresar[2][0]* resultado[0] + regresar[2][1] * resultado[1] + regresar[2][2] * resultado[2] + regresar[2][3] * resultado[3] ); //original
		mover[3] = ( regresar[3][0]* resultado[0] + regresar[3][1] * resultado[1] + regresar[3][2] * resultado[2] + regresar[3][3] * resultado[3] ); 
	     
	    vecIt->obtenVectores(mover[0], mover[1], mover[2]); //almacenamos los valores de mover ya que son los que tienen la posicion actual del vertice
	   }
	   indiceVer++;
	  }
	  carIt++;
	 }
	 objIt++;
	} 
}

//funcion para girar la columna en Y
void girarY(float grados)
{
	vector <objetos>:: iterator objIt;	
	vector <vectores>:: iterator vecIt;
	vector <caras>:: iterator carIt;
	vector <int>:: iterator indiceVer;
	
	float x, y, z, w =1;
	string nombre;
	float matrizY[4][4] = {{cos(grados), 0, sin(grados), 0}, {0, 1, 0, 0}, {sin(-grados), 0, cos(grados), 0}, {0, 0, 0, 1}};
	float resultado[4];
	float mover[4];
	
	objIt = listObjt.begin();
	nombre = objIt->darObjeto();
    while(objIt != listObjt.end())
	{
	 nombre = objIt->darObjeto();
	 carIt = objIt->carObj.begin();
	 while(carIt != objIt->carObj.end())
	 {
	  indiceVer = carIt->indice.begin();
	  while(indiceVer != carIt->indice.end())
	  {
	   vecIt = listVec.begin();
	   advance(vecIt, *indiceVer - 1);
	   if (nombre == "columna")
	   {
	    x = vecIt->darX();
	    y = vecIt->darY();
	    z = vecIt->darZ();
	    float vector[4] = {x, y, z, w};
	    float centro[4][4] = {{1, 0, 0, (-x/2)}, {0, 1, 0, (-y/2)}, {0, 0, 1, (-z/2)}, {0, 0, 0, 1}};  //creamos la matriz para llevar el vertice al centro
		float regresar[4][4] = { {1, 0, 0, (x/2)}, {0, 1, 0, (y/2)}, {0, 0, 1, (z/2)}, {0, 0, 0, 1} }; //creamos la matriz para regresar el vertice a su posicion original
		
		mover[0] = ( centro[0][0]* vector[0] + centro[0][1] * vector[1] + centro[0][2] * vector[2] + centro[0][3] * vector[3] ); //movemos los valores
		mover[1] = ( centro[1][0]* vector[0] + centro[1][1] * vector[1] + centro[1][2] * vector[2] + centro[1][3] * vector[3] ); //de X, Y, Z al centro
		mover[2] = ( centro[2][0]* vector[0] + centro[2][1] * vector[1] + centro[2][2] * vector[2] + centro[2][3] * vector[3] ); 
		mover[3] = ( centro[3][0]* vector[0] + centro[3][1] * vector[1] + centro[3][2] * vector[2] + centro[3][3] * vector[3] );

	    resultado[0] = ( (matrizY[0][0] * mover[0]) + (matrizY[0][1] * mover[1]) + (matrizY[0][2] * mover[2]) + (matrizY[0][3] * mover[3]) );
	    resultado[1] = ( (matrizY[1][0] * mover[0]) + (matrizY[1][1] * mover[1]) + (matrizY[1][2] * mover[2]) + (matrizY[1][3] * mover[3]) );
	    resultado[2] = ( (matrizY[2][0] * mover[0]) + (matrizY[2][1] * mover[1]) + (matrizY[2][2] * mover[2]) + (matrizY[2][3] * mover[3]) );
	    resultado[3] = ( (matrizY[3][0] * mover[0]) + (matrizY[3][1] * mover[1]) + (matrizY[3][2] * mover[2]) + (matrizY[3][3] * mover[3]) );

		mover[0] = ( regresar[0][0]* resultado[0] + regresar[0][1] * resultado[1] + regresar[0][2] * resultado[2] + regresar[0][3] * resultado[3] ); //regresamos el vertice
		mover[1] = ( regresar[1][0]* resultado[0] + regresar[1][1] * resultado[1] + regresar[1][2] * resultado[2] + regresar[1][3] * resultado[3] ); //rotado ya a su posicion
		mover[2] = ( regresar[2][0]* resultado[0] + regresar[2][1] * resultado[1] + regresar[2][2] * resultado[2] + regresar[2][3] * resultado[3] ); //original
		mover[3] = ( regresar[3][0]* resultado[0] + regresar[3][1] * resultado[1] + regresar[3][2] * resultado[2] + regresar[3][3] * resultado[3] ); 
	   } 
	   vecIt->obtenVectores(mover[0], mover[1], mover[2]); //almacenamos los valores de mover ya que son los que tienen la posicion actual del vertice

		
	   indiceVer++;
	  }
	  carIt++;
	 }
	 objIt++;
	} 
}

//funcion para girar la columna en Z
void girarZ(float grados)
{
	vector <objetos>:: iterator objIt;	
	vector <vectores>:: iterator vecIt;
	vector <caras>:: iterator carIt;
	vector <int>:: iterator indiceVer;
	
	float x, y, z, w =1;
	string nombre;
	float matrizZ[4][4] = {{cos (grados), sin (-grados), 0, 0}, {sin(grados), cos(grados), 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
	float resultado[4];
	float mover[4];
	
	objIt = listObjt.begin();
	nombre = objIt->darObjeto();
    while(objIt != listObjt.end())
	{
	 nombre = objIt->darObjeto();
	 carIt = objIt->carObj.begin();
	 while(carIt != objIt->carObj.end())
	 {
	  indiceVer = carIt->indice.begin();
	  while(indiceVer != carIt->indice.end())
	  {
	   vecIt = listVec.begin();
	   advance(vecIt, *indiceVer - 1);
	   if (nombre == "columna")
	   {
	    x = vecIt->darX();
	    y = vecIt->darY();
	    z = vecIt->darZ();
	    float vector[4] = {x, y, z, w};
	    float centro[4][4] = {{1, 0, 0, (-x/2)}, {0, 1, 0, (-y/2)}, {0, 0, 1, (-z/2)}, {0, 0, 0, 1}};  //creamos la matriz para llevar el vertice al centro
		float regresar[4][4] = { {1, 0, 0, (x/2)}, {0, 1, 0, (y/2)}, {0, 0, 1, (z/2)}, {0, 0, 0, 1} }; //creamos la matriz para regresar el vertice a su posicion original
		
		mover[0] = ( centro[0][0]* vector[0] + centro[0][1] * vector[1] + centro[0][2] * vector[2] + centro[0][3] * vector[3] ); //movemos los valores
		mover[1] = ( centro[1][0]* vector[0] + centro[1][1] * vector[1] + centro[1][2] * vector[2] + centro[1][3] * vector[3] ); //de X, Y, Z al centro
		mover[2] = ( centro[2][0]* vector[0] + centro[2][1] * vector[1] + centro[2][2] * vector[2] + centro[2][3] * vector[3] ); 
		mover[3] = ( centro[3][0]* vector[0] + centro[3][1] * vector[1] + centro[3][2] * vector[2] + centro[3][3] * vector[3] );


	    resultado[0] = ( (matrizZ[0][0] * mover[0]) + (matrizZ[0][1] * mover[1]) + (matrizZ[0][2] * mover[2]) + (matrizZ[0][3] * mover[3]) );
	    resultado[1] = ( (matrizZ[1][0] * mover[0]) + (matrizZ[1][1] * mover[1]) + (matrizZ[1][2] * mover[2]) + (matrizZ[1][3] * mover[3]) );
	    resultado[2] = ( (matrizZ[2][0] * mover[0]) + (matrizZ[2][1] * mover[1]) + (matrizZ[2][2] * mover[2]) + (matrizZ[2][3] * mover[3]) );
	    resultado[3] = ( (matrizZ[3][0] * mover[0]) + (matrizZ[3][1] * mover[1]) + (matrizZ[3][2] * mover[2]) + (matrizZ[3][3] * mover[3]) );

		mover[0] = ( regresar[0][0]* resultado[0] + regresar[0][1] * resultado[1] + regresar[0][2] * resultado[2] + regresar[0][3] * resultado[3] ); //regresamos el vertice
		mover[1] = ( regresar[1][0]* resultado[0] + regresar[1][1] * resultado[1] + regresar[1][2] * resultado[2] + regresar[1][3] * resultado[3] ); //rotado ya a su posicion
		mover[2] = ( regresar[2][0]* resultado[0] + regresar[2][1] * resultado[1] + regresar[2][2] * resultado[2] + regresar[2][3] * resultado[3] ); //original
		mover[3] = ( regresar[3][0]* resultado[0] + regresar[3][1] * resultado[1] + regresar[3][2] * resultado[2] + regresar[3][3] * resultado[3] ); 
	     
	   vecIt->obtenVectores(mover[0], mover[1], mover[2]); //almacenamos los valores de mover ya que son los que tienen la posicion actual del vertice

	   }
	   indiceVer++;
	  }
	  carIt++;
	 }
	 objIt++;
	} 
}


/*Funcion para crear las normales de las caras*/

void normales()
{
	vector <objetos>:: iterator objIt;	
	vector <vectores>:: iterator vecIt;
	vector <caras>:: iterator carIt;
	vector <int>:: iterator indiceVer;
	
    vectores va, vb; 	//objetos de tipos vectores para calcular las normales
	float magnitud, nx, ny, nz, res, temporalX, temporalY, temporalZ;	//datos que se utilizaran para realizar las normales
	float verticesCara[3][3];
	int cont;
		
	objIt = listObjt.begin();
    while(objIt != listObjt.end())
	{
	 carIt = objIt->carObj.begin();
	 while(carIt != objIt->carObj.end())
	 {
	  indiceVer = carIt->indice.begin();
	  cont = 0;
	  while(indiceVer != carIt->indice.end())
	  {
	   vecIt = listVec.begin();
	   advance(vecIt, *indiceVer - 1);
	   
	   verticesCara[cont][0] = vecIt->darX();
	   verticesCara[cont][1] = vecIt->darY();
	   verticesCara[cont][2] = vecIt->darZ();
	   cont ++;
	   
	   va.obtenVectores(verticesCara[1][0] - verticesCara[0][0], verticesCara[1][1] - verticesCara[0][1], verticesCara[1][2] - verticesCara[0][2]);						//obtencion de los datos de los 
	   vb.obtenVectores(verticesCara[2][0] - verticesCara[1][0], verticesCara[2][1] - verticesCara[1][1], verticesCara[2][2] - verticesCara[1][2]);			//vectores con los que se operaran
 
	   nx =((va.darY()) * (vb.darZ())) - ((va.darZ()) * (vb.darY()));
	   ny =((va.darZ()) * (vb.darX())) - ((va.darX()) * (vb.darZ()));
       nz =((va.darX()) * (vb.darY())) - ((va.darY()) * (vb.darX()));
 
	   carIt->normal.obtenVectores(nx, ny, nz);	//datos de la normal
	   
	   res = (pow(carIt->normal.darX(), 2)) + pow(carIt->normal.darY(), 2) + pow(carIt->normal.darZ(), 2); //suma para calcular la magnitud
	   magnitud = sqrt(abs(res)); //calculo de la magintud, se utiliza abs() para evitar la aparicion de datos nan
	   temporalX = carIt->normal.darX();	//se utilizan temporales para
       temporalY = carIt->normal.darY();	//poder guardar los datos de x, y, z
	   temporalZ = carIt->normal.darZ();	//y poder dividir los datos entre la magnitud
	   
	   indiceVer++;
	  }
	  carIt->normal.obtenVectores((temporalX/magnitud), (temporalY/magnitud), (temporalZ/magnitud)); //datos de la normal con la magnitud aplicada
	  carIt++;
	 }
	 objIt++;
	} 
}


/*FUNCION PARA CREAR EL SOMBREADO FLAT, EN ESTA FUNCION SE CREA LA ILUMINACION AMBIENTAL Y DIFUSA*/
void luzYsombra(caras c)
{
	colorBoom.obtenVectores(1, 0.7, 0.025); //se usan los vectores para guardar los colores
	colorCol.obtenVectores(1, 0.033, 0.247); //x, y, z, pasan a ser valores R, G, B
	
	vectores foco;
	foco.obtenVectores(1, 0, 0); //direccion a la que apunta el foco	
	
	float colorAmbiental[3] = {0, 0, 1};
	float intensidadAmbiental = 0.5;
	float escalar, distanciaFoco = 0.8;
	float colorDifuso[3] = {1, 0, 1};
	float luzAmbiental[3] = {intensidadAmbiental*colorAmbiental[0], intensidadAmbiental*colorAmbiental[1], intensidadAmbiental*colorAmbiental[2]};
	float luzDifusa[3] = {colorDifuso[0] * distanciaFoco, colorDifuso[1] * distanciaFoco, colorDifuso[2] * distanciaFoco};
	
	escalar= ( (foco.darX() * c.normal.darX()) + (foco.darY() * c.normal.darY()) + (foco.darZ() * c.normal.darZ()));
	
	/*tomando en cuenta la formula del sombreado flat : I = KaIa + IdKd(NxL)*/
	
	colorBoom.obtenVectores( GLclampf(luzAmbiental[0] + (luzDifusa[0] * escalar) ), GLclampf(luzAmbiental[1] + (luzDifusa[1] * escalar)), GLclampf(luzAmbiental[2] + (luzDifusa[2] * escalar)) );
	colorCol.obtenVectores( GLclampf(luzAmbiental[0] + (luzDifusa[0] * escalar) ), GLclampf(luzAmbiental[1] + (luzDifusa[1] * escalar)), GLclampf(luzAmbiental[2] + (luzDifusa[2] * escalar)) );
}

/*FUNCION PARA UTILIZAR EL TECLADO*/
void teclado(unsigned char key, int x, int y)
{
	switch(key)
	{		
		/*Las funciones de rotacion se usaran dependiendo de la tecla pulsada*/
		case('j'):
			girarX(0.01);		
		break;
		
		case('l'):
			girarX(-0.01);
		break;
		
		case('i'):
			girarY(0.01);
		break;
		
		case('k'):
			girarY(-0.01);
		break;
		
		case('o'):
			girarZ(0.01);
		break;
		
		case('u'):
			girarZ(-0.01);
		break;
	}
	glutPostRedisplay();	//se utiliza para que openGL redibuje de nuevo
}


/*FUNCION PARA CREAR LA CAMARA
 * 
 * Recordar que la matriz de vista esta compuesta como:
 * {posicionCamaraX, posicionCamaraY, posicionCamaraZ, 0,  mirandoHaciaX, mirandoHaciaY, mirandoHaciaZ, 0,  0, posicionY(1 o 0), 0, 0, 
 *  0, 0, 0, 1}
 * 
 */

void lookAt()
{
	float matriz[16] = {-5, 5, -5, 0,   5, 5, 5, 0,   0, 1, 0, 0,   0, 0, 0, 1};	//creamos la matriz de la camara
	glMultMatrixf(matriz);
	glGetFloatv(GL_MODELVIEW, matriz);
}

/*FUNCION PARA CREAR LA MATRIZ DE PERSPECTIVA
 * 
 * Recordar que la matriz de perspectiva esta compuesta como:
 * {1, 0, 0, (campo de vista en Y dado en grados), 0, 1, 0, (campo de vista en X dado en grados), 0, 0, 1, (parte cercana del plano), 
 * 0, 0, 0, (parte lejana del plano)}
 * 
 */

void perspectiva()
{
	float matriz[16] = {1, 0, 0, (480/640),  0, 1, 0, (640/480),  0, 0, 1, 0,  0, 0, 0, 11.5}; //creamos la matriz con la que operaremos
	glMultMatrixf(matriz);
	glGetFloatv(GL_PROJECTION_MATRIX, matriz);
}
