#ifndef CARAS_HPP
#define CARAS_HPP

#include <iostream>
#include <vector>
#include "vectores.hpp"

using namespace std;

class caras
{
	public:
		caras();
		vectores normal;
		vector<int>indice; //lista para guardar cada uno de los indices de las caras
		void muestraCaras();

			
	private:

		
};

#endif /* CARAS_HPP */ 
