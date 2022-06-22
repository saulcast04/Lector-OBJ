#include "vectores.hpp"
#include <iostream>

using namespace std;


vectores::vectores()
{
 
}


void vectores::obtenVectores(float posX, float posY, float posZ)
{
 x= posX;
 y= posY;
 z= posZ;
}

void vectores::muestraVector()
{
 cout <<" v "<< x << " " << y << " " << z << "\n";
}

float vectores::darX()
{
 return x;
}

float vectores::darY()
{
 return y;
}
float vectores::darZ()
{
 return z;
}
