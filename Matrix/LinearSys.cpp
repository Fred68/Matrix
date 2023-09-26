

#ifndef LINEARSYSCPP
#define LINEARSYSCPP

#include "LinearSys.h"


using namespace matrix;

template <class DATA, class MOD> bool LinearSys<DATA, MOD>::factor(const Matrix <DATA> &A)
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

template <class DATA, class MOD> bool LinearSys<DATA, MOD>::solve(Matrix <DATA> &x, const Matrix <DATA> &b)
	{
	int n = a.rows();
	solve_check();
	if ((b.rows() != n) || (b.cols() != 1))		// Verifica dimensioni vettore termini noti
		{
		throw std::runtime_error(MatrixDef::ERR_SZMISMATCH);
		}
	x = b;
	if (n == 1)
		{
		if (abs(a(0, 0)) < _epszero)
			{
			throw std::runtime_error(MatrixDef::ERR_SINGULAR);
			}
		x(0,0) = x(0,0) / a(0,0);
		return true;
		}
	int k,j,i;
	DATA tmp;

	for (k = 0; k < n - 1; k++)			// Ciclo su tutte le righe, tranne l'ultima
		{
		j = pivot(k,0);		// Ottiene la riga da scambiare con le riga k
		if (j != k)						// Le scambia, se necessario
			{
			tmp = x(j,0);
			x(j,0) = x(k,0);
			x(k, 0) = tmp;
			}
		for(i=k+1; i<n; i++)
			{
			x(i, 0) = (i, 0) + a(i, k) * x(k, 0);	// Non usa l'operatore+=, nel caso in cui sia definito solo l'operatore +
			}
		}
	x(n-1, 0) = x(n-1, 0) / a(n-1, n-1);
	for (i = n - 2; i >= 0; i--)
		{
		for (tmp = (DATA)0.0, j = i + 1; j < n; j++)
			tmp = tmp + a(i, j) * x(j, 0);	// Non usa l'operatore+=, nel caso in cui sia definito solo l'operatore +
		x(i, 0) = (x(i, 0) - tmp) / a(i, i);
		}
	return true;
	}




#endif
