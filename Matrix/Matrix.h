

#ifndef MATRIXH
#define MATRIXH


/*************************************/
/*                                   */
/*  Matrix.h                         */
/*                                   */
/*************************************/



#include <iostream>
#include <concepts>
#include <utility>
#include <string>
#include <sstream>
#include <complex>
#include <limits>
// #include <type_traits>	// Per complex

using std::swap;
using std::string;
using std::runtime_error;
using std::pair;
using std::is_floating_point;
using std::numeric_limits;

#ifdef _DEBUG
using std::cout;
using std::endl;
using std::ostream;
#endif

#include "Requires.h"
#include "MatrixDef.h"

namespace matrix
{

/* Prototyping della classe generica che deve essere friend */
template<class DATA> class MatrixIterator;

template<class DATA> class Matrix 
    {
	private:
		int _row;							// Numeri di righe...
		int _col;							// ...e colonne
		DATA *dat;							// Puntatore ai dati
		static DATA *_empty;				// Dato vuoto
		int _iterators;						// Numero di iteratori

	public:
		Matrix();
		Matrix(int rows, int cols);
		Matrix(int rows, int cols, DATA d) requires RQassign<DATA>;
		Matrix(int rows, int cols, DATA (*pf)(int r, int c)) requires RQassign<DATA>;
		Matrix(const Matrix& m) requires RQassign<DATA>;
		~Matrix()
			{
			if (dat != (DATA*)nullptr)
				{
				delete[] dat;
				}
			if (_iterators > 0)
				{
				// throw std::runtime_error(MatrixDef::ERR_ALLOC);	// Non interrompere un distruttore.
				#ifdef _DEBUG
				cout << "~Matrix() with pending " << _iterators << " iterators." << endl;
				#endif
				}
			#ifdef _DEBUG
			cout << "~Matrix()" << endl;
			#endif
			}
		void clear()
			{
			if (dat != (DATA*)nullptr)
				{
				delete[] dat;
				dat = (DATA*)nullptr;
				}
			_col = _row = 0;
			#ifdef _DEBUG
			cout << "Clear()" << endl;
			#endif
			}

		// Accesso in lettura
		int rows() { return _row; }
		int cols() { return _col; }
		
		// Accesso ai dati
		DATA &operator()(int irow, int icol)
			{
			if ((irow < _row) && (irow >= 0) && (icol < _col) && (icol >= 0))
				{
				return dat[irow * _col + icol];
				}
			else
				{
				throw std::runtime_error(MatrixDef::ERR_OUTOFBOUND);
				}
			}
		Matrix<DATA> &get_row(int irow) requires RQassign<DATA>;
		Matrix<DATA> &get_col(int icol) requires RQassign<DATA>;
		Matrix<DATA> &get_sub(int row_ini, int col_ini, int n_row, int n_col) requires RQassign<DATA>;

		// Assegnazione
		Matrix<DATA> &operator=(const Matrix<DATA> &m) requires RQassign<DATA>;

		// Trasposta
		Matrix<DATA> &operator!() requires RQassign<DATA>;		// Trasposta (forma prefissa: B = !A)
		void transpose() requires RQassign<DATA>;				// Traspone

		// To String() 	Operazioni su stream generano link error con: friend ostream &operator<<(ostream &stream, const Matrix &m);	
		string to_string(char col_sep = '\t', char row_sep = '\n');
		string to_string(int cmd);
		
		// Modifica dimensioni
		bool dim(int rows, int cols) requires RQassign<DATA>;
		bool dim(Matrix<DATA> &m)  requires RQassign<DATA> { return dim(m.rows(), m.cols()); }
		bool trim(int row1, int row2, int col1, int col2) requires RQassign<DATA>;
		bool rem_row_col(int irow, int icol) requires RQassign<DATA>;
		bool rem_row(int irow) requires RQassign<DATA>;
		bool rem_col(int icol) requires RQassign<DATA>;

		// Min, max
		pair<int,int> &min_row_col() requires RQorder<DATA>;
		pair<int, int> &max_row_col() requires RQorder<DATA>;
		DATA &min() requires RQorder<DATA>
			{
			pair<int,int> i = min_row_col();						// Occasionale errore su funzione min() !!!!
			return (*this)(i.first,i.second);
			}

		// Operatore somma
		Matrix <DATA> &operator+(const Matrix <DATA> &m) requires RQsum<DATA>;			// Somma semplice e...
		Matrix <DATA> &operator+=(const Matrix <DATA> &m) requires RQsum<DATA>;			// ... con assegnazione

		// Differenza e prodotto di matrici
		Matrix <DATA> &operator-(const Matrix <DATA> &m) requires RQsumdifprod<DATA>;	// Differenza
		Matrix <DATA> &operator-=(const Matrix <DATA> &m) requires RQsumdifprod<DATA>;	// Differenza con assegnazione
		Matrix <DATA> &operator*(const Matrix <DATA> &m) requires RQsumdifprod<DATA>;	// Prodotto
		Matrix <DATA> &operator*=(const Matrix <DATA> &m) requires RQsumdifprod<DATA>;	// Prodotto con assegnazione
		
		// Prodotto scalare (solo vettori)
		DATA &operator^(const Matrix <DATA> &m) requires RQsumdifprod<DATA>;			

		// Prodotto con scalare (T * Matrix<T> e Matrix<T> * T) e divisione (solo Matrix<T> / T).
		Matrix <DATA> &operator*(const DATA &x) requires RQsumdifprod<DATA>;			// Prodotto per numero
		friend Matrix<DATA> &operator*(const DATA &sx, const Matrix <DATA> &dx) requires RQsumdifprod<DATA>
			{																// Inline, per evitare errore linker
			Matrix<DATA> *tmp = new Matrix<DATA>();							// Alloca nuova Matrix vuota
			int ir, ic;
			if ((dx._row > 0) && (dx._col > 0))
				{
				tmp->dat = new DATA[dx._row * dx._col];							// Alloca spazio.
				if (tmp->dat)
					{
					for (ir = 0; ir < dx._row; ir++)							// Somma
						for (ic = 0; ic < dx._col; ic++)
							tmp->dat[ir * dx._col + ic] = dx.dat[ir * dx._col + ic] * sx;
					tmp->_row = dx._row;
					tmp->_col = dx._col;
					}
				else
					{
					tmp->_row = tmp->_col = 0;
					throw std::runtime_error(MatrixDef::ERR_ALLOC);
					}
				}
			else
				{															// Matrice nulla
				tmp->_row = tmp->_col = 0;
				tmp->dat = (DATA*)nullptr;
				}
			#ifdef _DEBUG
			cout << "Matrix::operator*(const DATA &sx, const Matrix <DATA> &dx)" << endl;
			#endif
			return *tmp;
			}
		Matrix <DATA> &operator/(const DATA &x) requires RQsumdifprod<DATA>;			// Divisione

		// Speciali
		static const Matrix <DATA> &Id(int sz) requires RQsumdifprod<DATA>;

		// Friend Iterator
		friend MatrixIterator<DATA>;
		int get_iterators_num()	{return _iterators;}
    };


#include "Matrix.cpp"

}
#endif