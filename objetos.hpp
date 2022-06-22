#ifndef OBJETOS_HPP
#define OBJETOS_HPP
#include <iostream>
#include <string>
#include <vector>
#include "caras.hpp"
#include "vectores.hpp"

using namespace std;

class objetos
{
	public:
		objetos();
		void obtenObjeto(string);
		void muestraObjeto();
		string darObjeto();
		int numCara;
		vector<int>vecObj; //lista para los vectores de cada objeto
		vector<caras>carObj; //lista para las caras que conforman al objeto
			
	private:
		string name;
};

#endif /* OBJETOS_HPP */ 
