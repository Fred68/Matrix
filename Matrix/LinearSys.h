

#ifndef LINEARSYSH
#define LINEARSYSH

#include "Matrix.h"

#include <string>
#include <sstream>

#ifdef _DEBUG
using std::cout;
using std::endl;
using std::ostream;
#endif

using std::string;
using std::stringstream;

namespace matrix
{

template <class DATA, class MOD> class LinearSys
	{
	private:
		Matrix<DATA> a;				// Matrice fattorizzata
		Matrix<int> pivot;			// Vettore di pivot
		const MOD EPS = std::numeric_limits<MOD>::epsilon();
		const MOD MIN = std::numeric_limits<MOD>::min();
		const MOD MAX = std::numeric_limits<MOD>::max();
		MOD _epszero;				// Valore minimo considerato > 0
		MOD _det;					// Determinante
	public:

		/* Ctor. Vincolo nel costruttore, non serve altrove */
		LinearSys() requires RQfloatabs<DATA,MOD>
			{
			_epszero = EPS;
			_det = (MOD) 0;
			#ifdef _DEBUG		
			cout << "LinearSys(): epsilon=" << EPS << ", min=" << MIN << ", max=" << MAX << endl;
			#endif
			}

		/* Soglia sotto la quale un valore si considera nullo */
		void set_eps_zero(MOD eps_zero)
			{
			if(eps_zero > EPS)
				{
				_epszero = eps_zero;
				}
			}
		void set_eps_zero_ratio(MOD eps_zero)
			{
			if (eps_zero >= (MOD)1)
				{
				_epszero = eps_zero * EPS;
				}
			}
		MOD get_eps_zero()
			{
			return _epszero;
			}

		/* to_string() */
		string to_string()
			{
			stringstream ss;
			ss << "Eps_zero=" << _epszero;
			return ss.str();
			}

		/* Fattorizzazione LU con pivoting parziale. Monegato Metodi e algoritmi per il calcolo numerico CLUT 2008, pag. 41 e succ. */
		bool Factor(Matrix <DATA> &A);
	};


#include "LinearSys.cpp"

}


#endif