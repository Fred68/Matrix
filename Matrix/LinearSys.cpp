

#ifndef LINEARSYSCPP
#define LINEARSYSCPP

#include "LinearSys.h"


using namespace matrix;

template <class DATA, class MOD> bool LinearSys<DATA, MOD>::Factor(Matrix <DATA> &A)
	{
	int n = A.rows();
	if (n != A.cols())
		{
		throw std::runtime_error(MatrixDef::ERR_NOTSQUARE);
		}
	if (n < 1)
		{
		throw std::runtime_error(MatrixDef::ERR_ZEROSIZE);
		}
	pivot.dim(n-1, 1);		// Vettore colonna n-1 elementi
	a = A;							// Assegnazione della classe Matrix<>
	if (n == 1)						// Se matrice 1x1: il determinante è il valore.
		{
		_det = a(0,0);
		if (abs(_det) < _epszero)	
			{
			return false;
			}
		return true;
		}
	int k, i, io, j;				// Ciclo di calcolo
	MOD amax;				
	DATA tmp;
	for (_det = (DATA)1, io = 0, k = 0; k < n - 1; k++)		// Ciclo 1 su tutte le righe k=0...n-2
		{
		for (amax = (MOD)0, i = k; i < n; i++)				// Cerca la riga (io), dalla k in poi, con il massimo amax sulla colonna k.
			{
			if (abs(a(i, k)) >= amax)
				{
				io = i;
				amax = abs(a(i, k));
				}
			}
		pivot(k,0) = io;				// Imposta il pivot
		if (amax < _epszero)					// Se il valore massimo dei coefficienti è (quasi) zero...
			{
			_det = (MOD) 0;						// ...azzera il determinante ed esce.
			return false;
			}
		if (io != k)							// Se l'indice non è k...
			{
			for (j = k; j < n; j++)				// ...scambia le righe k e io (solo la parte superiore, l'inferiore è nulla)...
				{
				tmp = a(k, j);
				a(k, j) = a(io, j);
				a(io, j) = tmp;
				}
			_det = -_det;							// ...e cambia segno al determinante.
			}
		for (i = k + 1; i < n; i++)				// Ciclo 2, per tutte le righe successive alla riga k attuale
			{
			a(i, k) = -a(i, k) / a(k, k);		// Coefficiente per metodo di Gauss (su riga i, colonna k), sotto diagonale.
			for (j = k + 1; j < n; j++)			// Combina linearmente le due righe, sopra la diagonale 
				{
				a(i, j) = a(i, j) + a(i, k) * a(k, j);
				}
			}								// Fine ciclo 2
		_det = _det * a(k, k);
		}
	_det = _det * a(n - 1, n - 1);
	return true;
	}





#endif
