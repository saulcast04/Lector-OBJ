#ifndef VECTORES_HPP
#define VECTORES_HPP
#include <iostream>

using namespace std;

class vectores
{
	public:
		vectores();
		void obtenVectores(float, float, float);
		void muestraVector();
		int id;
		float darX();
		float darY();
		float darZ();
			
	private:
		float x;
		float y;
		float z;

};

#endif /* VECTORES_HPP */ 
