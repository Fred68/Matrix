

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
		const MOD eps = std::numeric_limits<MOD>::epsilon();

	public:

		/* Ctor */
		LinearSys() requires RQfloatabs<DATA,MOD>
			{
			pivot.dim(1,1);	// inizializzazione solo per test
			#ifdef _DEBUG		
			cout << "LinearSys():" << eps << "." << endl;
			#endif
			}


	};


// #include "LinearSys.cpp" per ora superfluo

}


#endif