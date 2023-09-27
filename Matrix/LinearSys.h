

#ifndef LINEARSYSH
#define LINEARSYSH

#include "Matrix.h"

#include <string>
#include <sstream>

#ifdef _DEBUG
using std::cout;
using std::ostream;
#endif

using std::string;
using std::stringstream;
using std::endl;

namespace matrix
{

//template<class DATA> class Matrix;	// Prototyping superfluo

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
			ss << "Eps_zero=" << _epszero << endl;
			ss << "Det=" << _det << endl;
			ss << "PLU=" << a.to_string() << endl;
			ss << "Pivot=" << pivot.to_string() << endl;
			return ss.str();
			}

		/* Fattorizzazione LU con pivoting parziale e soluzione. Monegato Metodi e algoritmi... CLUT 2008, pag. 41 e succ.*/
		bool factor(Matrix <DATA> &A);
		bool solve_check(bool throw_exception = true)
			{
			bool ok = true;
			int n = a.rows();
			if (n != a.cols())
				{
				ok = false;
				if(throw_exception)		throw std::runtime_error(MatrixDef::ERR_NOTSQUARE);
				}
			if (n < 1)
				{
				ok = false;
				if (throw_exception)	throw std::runtime_error(MatrixDef::ERR_ZEROSIZE);
				}
			if (abs(_det) < _epszero)
				{
				ok = false;
				if (throw_exception)	throw std::runtime_error(MatrixDef::ERR_SINGULAR);
				}
			if ((pivot.rows() != n - 1) || (pivot.cols() != 1))
				{
				ok = false;
				if (throw_exception)	throw std::runtime_error(MatrixDef::ERR_PIVOT);
				}
			return ok;
			}
		bool solve(Matrix <DATA> &x, Matrix <DATA> &b);
	};


#include "LinearSys.cpp"

}


#endif