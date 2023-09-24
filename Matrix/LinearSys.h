

#ifndef LINEARSYSH
#define LINEARSYSH

#include "Matrix.h"

#ifdef _DEBUG
using std::cout;
using std::endl;
using std::ostream;
#endif


namespace matrix
{

template <class DATA, class MOD> class LinearSys
	{
	private:
		Matrix<DATA> A;				// Matrice fattorizzata
		Matrix<int> pivot;			// Vettore di pivot
		const MOD EPS = std::numeric_limits<MOD>::epsilon();
		//MOD _
	public:

		/* Ctor */
		LinearSys() requires RQfloatabs<DATA,MOD>
			{
			#ifdef _DEBUG		
			cout << "LinearSys():" << EPS << "." << endl;
			#endif
			}


	};


// #include "LinearSys.cpp" per ora superfluo

}


#endif