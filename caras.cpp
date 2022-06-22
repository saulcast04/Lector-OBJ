#include "caras.hpp"
#include <iostream>
#include <vector>

using namespace std;


caras::caras()
{
 
}


void caras::muestraCaras()
{
 vector<int>::iterator indiceIt;
 
 indiceIt = indice.begin();
 cout << " f ";
 while(indiceIt != indice.end())
 {
  cout << *indiceIt << " ";
  indiceIt++;
 }
 cout << endl; 
}

