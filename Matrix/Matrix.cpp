
#ifndef MATRIXCPP
#define MATRIXCPP

/*************************************/
/*                                   */
/*  Matrix.cpp                   */
/*                                   */
/*************************************/



#include "Matrix.h"

#ifdef _DEBUG
#include <iostream>
using std::cout;
using std::endl;
#endif

using namespace matrix;

/* Costruttori */
template<class DATA> Matrix<DATA>::Matrix()
	{
	_row = _col = 0;
	dat = (DATA*)nullptr;
	_iterators = 0;
	#ifdef _DEBUG
	cout << "Matrix()" << endl;
	#endif
	}
template <class DATA> Matrix<DATA>::Matrix(int rows, int cols)
	{
	if ((rows > 0) && (cols > 0))
		{
		_row = rows;
		_col = cols;
		dat = new DATA[rows * cols];
		if (!dat)							// Se fallita allocazione, esce con errore
			{
			_row = _col = 0;
			throw std::runtime_error(MatrixDef::ERR_ALLOC);
			}
		}
	else
		{									// Matrice vuota
		_row = _col = 0;
		dat = (DATA*)nullptr;
		}
	_iterators = 0;
	#ifdef _DEBUG
	cout << "Matrix(int rows, int cols)" << endl;
	#endif
	}
template <class DATA> Matrix<DATA>::Matrix(int rows, int cols, DATA d) requires RQassign<DATA>
	{
	#pragma warning (disable : 6385)
	if ((rows > 0) && (cols > 0))
		{
		_row = rows;
		_col = cols;
		dat = new DATA[rows * cols];
		if (!dat)							// Se fallita allocazione, esce con errore
			{
			_row = _col = 0;
			throw std::runtime_error(MatrixDef::ERR_ALLOC);
			}
		else
			{
			int ir, ic;
			for(ir = 0; ir < _row; ir++)
				for (ic = 0; ic < _col; ic++)
					dat[ir * _col + ic] = d;
			}
		}
	else
		{									// Matrice vuota
		_row = _col = 0;
		dat = (DATA*)nullptr;
		}
	_iterators = 0;
	#ifdef _DEBUG
	cout << "Matrix(int rows, int cols, DATA d)" << endl;
	#endif
	#pragma warning (default : 6385)
	}
template <class DATA> Matrix<DATA>::Matrix(int rows, int cols, DATA(*pf)(int r, int c)) requires RQassign<DATA>
	{
	if ((rows > 0) && (cols > 0))
		{
		_row = rows;
		_col = cols;
		dat = new DATA[rows * cols];
		if (!dat)							// Se fallita allocazione, esce con errore
			{
			_row = _col = 0;
			throw std::runtime_error(MatrixDef::ERR_ALLOC);
			} else
			{
			int ir, ic;
			for (ir = 0; ir < _row; ir++)
				for (ic = 0; ic < _col; ic++)
					dat[ir * _col + ic] = (*pf)(ir,ic);
			}
		}
	else
		{									// Matrice vuota
		_row = _col = 0;
		dat = (DATA*)nullptr;
		}
	_iterators = 0;
	#ifdef _DEBUG
	cout << "Matrix(int rows, int cols, DATA d)" << endl;
	#endif
	}

/* Costruttore di copia */
template <class DATA> Matrix<DATA>::Matrix(const Matrix& m) requires RQassign<DATA>
	{
	_row = m._row;							// Copia le dimensioni
	_col = m._col;
	if ((_row > 0) && (_col > 0))				
		{
		dat = new DATA[_row * _col];		// Alloca nuovi dati
		if (dat)							// Ricopia il contenuto
			{
			int ir, ic;							// Non usa memcpy((void*)dat, (void*)m.dat, row * col * sizeof(DATA)); ma '='.
			for (ir = 0; ir < _row; ir++)		// Ricopia i valori. Usa operatore di assegnazione.
				for (ic = 0; ic < _col; ic++)
					dat[ir*_col+ic] = m.dat[ir * _col + ic];
			}
		else
			{								// Fallita allocazione
			_row = _col = 0;
			throw std::runtime_error(MatrixDef::ERR_ALLOC);
			}
		}
	else
		{									// Matrice vuota
		_row = _col = 0;
		dat = (DATA*)nullptr;
		}
	_iterators = 0;
	#ifdef _DEBUG
	cout << "Matrix(const Matrix& m)" << endl;
	#endif

	}

/* set from array () */
template <class DATA> void Matrix<DATA>::set(int rows, int cols, DATA *d) requires RQassign<DATA>
	{

	#ifdef _DEBUG
	//cout << endl << "-----------------------------------" << endl;
	//int a, b, c = 0;
	//a = sizeof(d);
	//b = sizeof(*d);
	////c = sizeof(d[0]);
	//cout << "sizeof(d)=" << a << endl;
	//cout << "sizeof(*d)=" << b << endl;
	//cout << "datasize=" << _datasize << endl;
	//cout << "rows=" << a/_datasize << endl;
	//cout << "sizeof(d[0])=" << c << endl;
	//cout << "-----------------------------------" << endl;
	#endif

	DATA *datnew;							// Puntatore ai nuovi dati
	if ((rows < 0) || (cols < 0))			// Verifica le nuove dimensioni
		{
		throw std::runtime_error(MatrixDef::ERR_OUTOFBOUND);
		}
	if ((rows == 0) || (cols == 0))			// Se una dimensione è nulla
		{
		if (dat) delete[] dat;				// Dealloca la vecchia matrice
		_row = _col = 0;					// e azzera
		dat = (DATA*)nullptr;
		return;
		}
	/*
	if (array.size() != rows * cols)
		{
		throw std::runtime_error(MatrixDef::ERR_SZMISMATCH);
		}
	*/
	datnew = new DATA[rows * cols];			// Alloca nuova matrice
	if (!datnew)							// Verifica allocazione avvenuta
		{
		throw std::runtime_error(MatrixDef::ERR_ALLOC);
		}
	int ir, ic;								// Ricopia i valori dal vettore alla matrice
	for (ir = 0; ir < rows; ir++)
		for (ic = 0; ic < cols; ic++)
			datnew[ir * cols + ic] = d[ir * cols + ic];
	if (dat) delete[] dat;					// Dealloca la vecchia matrice
	_row = rows;							// Imposta i nuovi indici
	_col = cols;
	dat = datnew;							// Imposta il nuovo puntatore

	#ifdef _DEBUG
	cout << "void Matrix::set(int rows, int cols, DATA *d). ---> No array boundary check !" << endl;
	#endif
	return;
	}
template <class DATA> void Matrix<DATA>::set(int rows, int cols, const std::vector<DATA>& array) requires RQassign<DATA>
	{
	DATA *datnew;							// Puntatore ai nuovi dati
	if ((rows < 0) || (cols < 0))			// Verifica le nuove dimensioni
		{
		throw std::runtime_error(MatrixDef::ERR_OUTOFBOUND);
		}
	if ((rows == 0) || (cols == 0))			// Se una dimensione è nulla
		{
		if (dat) delete[] dat;				// Dealloca la vecchia matrice
		_row = _col = 0;					// e azzera
		dat = (DATA*)nullptr;
		return;
		}
	if (array.size() != rows * cols)
		{
		throw std::runtime_error(MatrixDef::ERR_SZMISMATCH);
		}
	datnew = new DATA[rows * cols];			// Alloca nuova matrice
	if (!datnew)							// Verifica allocazione avvenuta
		{
		throw std::runtime_error(MatrixDef::ERR_ALLOC);
		}
	int ir, ic;								// Ricopia i valori dal vettore alla matrice
	for (ir = 0; ir < rows; ir++)
		for (ic = 0; ic < cols; ic++)
			datnew[ir * cols + ic] = array[ir * cols + ic];
	if (dat) delete[] dat;					// Dealloca la vecchia matrice
	_row = rows;							// Imposta i nuovi indici
	_col = cols;
	dat = datnew;							// Imposta il nuovo puntatore
	
	#ifdef _DEBUG
	cout << "void Matrix::set(int rows, int cols, const std::vector<DATA>& array)" << endl;
	#endif
	return;
	}


/* Operatore di assegnazione */
template <class DATA> Matrix<DATA> &Matrix<DATA>::operator=(const Matrix<DATA> &m) requires RQassign<DATA>
	{
	if (this != &m)
		{
		DATA *tmp;
		_row = m._row;
		_col = m._col;
		if ((_row > 0) && (_col > 0))
			{
			tmp = new DATA[_row * _col];		// Alloca nuovi dati
			if (tmp)							// Ricopia il contenuto
				{
				int ir, ic;							// Non usa memcpy((void*)dat, (void*)m.dat, row * col * sizeof(DATA)); ma '='.
				for (ir = 0; ir < _row; ir++)		// Ricopia i valori. Usa operatore di assegnazione.
					for (ic = 0; ic < _col; ic++)
						tmp[ir * _col + ic] = m.dat[ir * _col + ic];
				delete[] dat;
				dat = tmp;
				}
			else
				{								// Fallita allocazione
				_row = _col = 0;
				throw std::runtime_error(MatrixDef::ERR_ALLOC);
				}
			}
		else
			{									// Matrice vuota
			_row = _col = 0;
			dat = (DATA*)nullptr;
			}
		}
	#ifdef _DEBUG
	cout << "Matrix::&operator=()" << endl;
	#endif
	return *this;
	}

/* Matrice trasposta */
template <class DATA> Matrix<DATA> &Matrix<DATA>::operator!() requires RQassign<DATA>		// Trasposta, forma postfissa *this operator! int (fittizio)
	{
	Matrix<DATA> *tmp = new Matrix<DATA>();							// Alloca nuovo Matrix vuoto
	int ir,ic;
	if ((_row > 0) && (_col > 0))
		{
		tmp->dat = new DATA[_row * _col];							// Alloca spazio.
		if (tmp->dat)
			{
			for (ir = 0; ir < _row; ir++)							// Ricopia i valori, scambiandone le posizioni
				for (ic = 0; ic < _col; ic++)
					tmp->dat[ic * _row + ir] = dat[ir * _col + ic];
			tmp->_row = _col;
			tmp->_col = _row;
			}
		else
			{
			tmp->_row = tmp->_col = 0;
			throw std::runtime_error(MatrixDef::ERR_ALLOC);
			}
		}
	else
		{																	// Matrice nulla
		tmp->_row = tmp->_col = 0;
		tmp->dat = (DATA*)nullptr;
		}
	#ifdef _DEBUG
	cout << "Matrix::&operator!()" << endl;
	#endif
	return *tmp;
	}

template <class DATA> void Matrix<DATA>::transpose() requires RQassign<DATA>				// Traspone la matrice
	{
	DATA *tmp = new DATA[_row * _col];									// Alloca spazio.
	int ir, ic;
	if (!tmp)
		{
		throw std::runtime_error(MatrixDef::ERR_ALLOC);
		}
	for (ir = 0; ir < _row; ir++)										// Ricopia i valori, scambiandone le posizioni
		for (ic = 0; ic < _col; ic++)
			tmp[ic * _row + ir] = dat[ir * _col + ic];
	ic = _row;															// Scambia righe e colonne	
	_row = _col;
	_col = ic;
	delete[] dat;														// Dealloca i dati vecchi
	dat = tmp;															// Riassegna il puntatore ai dati nuovi
	#ifdef _DEBUG
	cout << "Matrix::transpose()" << endl;
	#endif
	return;
	}

/* ToString() */
template <class DATA> string Matrix<DATA>::to_string(char col_sep, char row_sep)
	{
	int ir, ic;
	std::stringstream ss;
	ss << "[";
	for (ir = 0; ir < _row; ir++)
		{
		for (ic = 0; ic < _col; ic++)
			{
			ss << dat[ir * _col + ic];
			if(ic != _col-1)	ss << col_sep;
			}
		ss << row_sep;
		}
	ss << "] R" << _row << " x C" << _col;
	#ifdef _DEBUG
	cout << "Matrix::ToString(char col_sep, char row_sep)" << endl;
	#endif
	return ss.str();
	}

/* ToString() */
template <class DATA> string Matrix<DATA>::to_string(MatrixDef::Cmd cmd)
	{
	std::stringstream ss;
	switch (cmd)
		{
		case MatrixDef::Cmd::size:
			ss << "R" << _row << " x C" << _col ;
			break;
		case MatrixDef::Cmd::detail:
			ss << "R" << _row << "xC" << _col << ",iters=" << _iterators << ",dtsz=" << _datasize << ",empty=" << *_empty;
		break;
		default:
			break;
		}

	#ifdef _DEBUG
	cout << "Matrix::ToString(int cmd)" << endl;
	#endif
	return ss.str();
	}

/* Modifica dimensioni */
template <class DATA> bool Matrix<DATA>::dim(int rows, int cols) requires RQassign<DATA>
	{
	DATA *datnew;							// Puntatore ai nuovi dati
	int ir, ic;
	if ((rows < 0) || (cols < 0))			// Verifica le nuove dimensioni
		{
		throw std::runtime_error(MatrixDef::ERR_OUTOFBOUND);
		}
	if ((rows == 0) || (cols == 0))			// Se una dimensione è nulla
		{
		if (dat) delete[] dat;				// Delalloca la vecchia matrice
		_row = _col = 0;					// e azzera
		dat = (DATA*)nullptr;
		return false;
		}
	datnew = new DATA[rows * cols];			// Alloca nuova matrice
	if (!datnew)							// Verifica allocazione avvenuta
		{
		throw std::runtime_error(MatrixDef::ERR_ALLOC);
		}
	for (ir = 0; ir < rows; ir++)			// Ricopia la vecchia matrice nella nuova
		for (ic = 0; ic < cols; ic++)
			{
			if ((ir < _row) && (ic < _col))
				datnew[ir * cols + ic] = dat[ir * _col + ic];	// Ricopia valori...
			else
				datnew[ir * cols + ic] = (DATA)0;		// oppure azzera se fuori indice
			}
	if (dat) delete[] dat;					// Dealloca la vecchia matrice
	_row = rows;							// Imposta i nuovi indici
	_col = cols;
	dat = datnew;							// Imposta il nuovo puntatore
	#ifdef _DEBUG
	cout << "Matrix::dim(int rows, int cols)" << endl;
	#endif
	return true;
	}

/* Sottomatrice tra gli indici (compresi) */
template <class DATA> bool Matrix <DATA>::trim(int row1, int row2, int col1, int col2) requires RQassign<DATA>
	{
	// Mantiene indici tra row1,row2 e col1,col2 compresi.
	// Se row1>row2 o col1>col2, azzera la matrice;
	DATA *datnew;							// Puntatore a nuovi dati
	int rownew, colnew;
	int ir, ic;

	rownew = row2 - row1 + 1;					// Calcola nuove dimensioni
	colnew = col2 - col1 + 1;
	if ((colnew < 0) || (rownew < 0))			// Indici errati
		{
		throw std::runtime_error(MatrixDef::ERR_WRONGPARAM);
		}

	if ((colnew <= 0) || (rownew <= 0))			// Se nulle, azzera la matrice
		{
		if (dat) delete[] dat;
		_row = _col = 0;
		dat = (DATA*)nullptr;
		return false;
		}
	if ((row1 < 0) || (row2 < 0) || (row1 >= _row) || (row2 >= _row))	// Verifica correttezza indici
		return false;
	if ((col1 < 0) || (col2 < 0) || (col1 >= _col) || (col2 >= _col))
		return false;
	if (row1 > row2)							// Ordina gli indici
		ir = row1, row1 = row2, row2 = ir;
	if (col1 > col2)
		ic = col1, col1 = col2, col2 = ic;

	datnew = new DATA[(rownew) * (colnew)];	// Alloca nuova matrice
	if (!datnew)							// Verifica avvenuta allocazione
		{
		throw std::runtime_error(MatrixDef::ERR_ALLOC);
		}
	for (ir = row1; ir <= row2; ir++)				// Ricopia i valori
		for (ic = col1; ic <= col2; ic++)
			{
			datnew[(ir - row1) * colnew + (ic - col1)] = dat[ir * _col + ic];
			}
	if (dat) delete[] dat;					// Dealloca la vecchia matrice
	_row = rownew;							// Imposta i nuovi indici
	_col = colnew;
	dat = datnew;							// Imposta il nuovo puntatore
	#ifdef _DEBUG
	cout << "Matrix::trim(int row1, int row2, int col1, int col2)" << endl;
	#endif
	return true;
	}

/* Rimuove riga e colonna */
template <class DATA> bool Matrix<DATA>::rem_row_col(int irow, int icol) requires RQassign<DATA>
	{
	DATA *datnew;							// Puntatore a nuovi dati
	int rownew, colnew;						// Nuove dimensioni
	int ir, ic;								// Indici nuova matrice
	int irold, icold;						// Indici vecchia matrice

	if ((irow < 0) || (irow >= _row) || (icol < 0) || (icol >= _col))		// Verifica correttezza indici
		{
		throw std::runtime_error(MatrixDef::ERR_OUTOFBOUND);
		}
	rownew = _row - 1;						// Nuove dimensioni
	colnew = _col - 1;

	if ((colnew != 0) && (rownew != 0))		// Se matrice di dimensione non nulla
		{
		datnew = new DATA[rownew * colnew];	// Alloca nuova matrice (row-1, col-1)
		}
	else
		{
		if(dat)		delete[] dat;			// Se dimensione nulla, dealloca la vecchia
		dat = (DATA*)nullptr;				// Azzera la matrice
		_col = _row = 0;
		return false;
		}
	if (!datnew)							// Se fallita allocazione
		{
		throw std::runtime_error(MatrixDef::ERR_ALLOC);
		}
	for (ir = 0, irold = 0; ir < rownew; ir++, irold++)
		{
		if(irold == irow)	irold++;		// Salta all'indice successivo, se uguale alla riga da elimonare
		for (ic = 0, icold = 0; ic < colnew; ic++, icold++)
			{
			if(icold == icol)	icold++;	// Salta all'indice successivo, se uguale alla colonna da elimonare
			datnew[ir * colnew + ic] = dat[irold * _col + icold];	// Copia il valore
			}
		}
	if(dat)		delete[] dat;
	_row = rownew;
	_col = colnew;
	dat = datnew;
	#ifdef _DEBUG
	cout << "Matrix::rem_row_col(int irow, int icol)" << endl;
	#endif
	return true;
	}

/* Rimuove riga */
template <class DATA> bool Matrix<DATA>::rem_row(int irow) requires RQassign<DATA>
	{
	DATA *datnew;							// Puntatore a nuovi dati
	int rownew, colnew;						// Nuove dimensioni
	int ir, ic;								// Indici nuova matrice
	int irold, icold;						// Indici vecchia matrice

	if ((irow < 0) || (irow >= _row) )		// Verifica correttezza indici
		{
		throw std::runtime_error(MatrixDef::ERR_OUTOFBOUND);
		}
	rownew = _row - 1;						// Nuove dimensioni
	colnew = _col;

	if ((colnew != 0) && (rownew != 0))		// Se matrice di dimensione non nulla
		{
		datnew = new DATA[rownew * colnew];	// Alloca nuova matrice (row-1, col-1)
		}
	else
		{
		if (dat)		delete[] dat;		// Se dimensione nulla, dealloca la vecchia
		dat = (DATA*)nullptr;				// Azzera la matrice
		_col = _row = 0;
		return false;
		}
	if (!datnew)							// Se fallita allocazione
		{
		throw std::runtime_error(MatrixDef::ERR_ALLOC);
		}
	for (ir = 0, irold = 0; ir < rownew; ir++, irold++)
		{
		if (irold == irow)	irold++;		// Salta all'indice successivo, se uguale alla riga da elimonare
		for (ic = 0, icold = 0; ic < colnew; ic++, icold++)
			{
			datnew[ir * colnew + ic] = dat[irold * _col + icold];	// Copia il valore
			}
		}
	if (dat)		delete[] dat;
	_row = rownew;
	_col = colnew;
	dat = datnew;
	#ifdef _DEBUG
	cout << "Matrix::rem_row(int irow)" << endl;
	#endif
	return true;
	}

/* Rimuove colonna */
template <class DATA> bool Matrix<DATA>::rem_col(int icol) requires RQassign<DATA>
	{
	DATA *datnew;							// Puntatore a nuovi dati
	int rownew, colnew;						// Nuove dimensioni
	int ir, ic;								// Indici nuova matrice
	int irold, icold;						// Indici vecchia matrice

	if ((icol < 0) || (icol >= _col))		// Verifica correttezza indici
		{
		throw std::runtime_error(MatrixDef::ERR_OUTOFBOUND);
		}
	rownew = _row;							// Nuove dimensioni
	colnew = _col - 1;

	if ((colnew != 0) && (rownew != 0))		// Se matrice di dimensione non nulla
		{
		datnew = new DATA[rownew * colnew];	// Alloca nuova matrice (row-1, col-1)
		}
	else
		{
		if (dat)		delete[] dat;		// Se dimensione nulla, dealloca la vecchia
		dat = (DATA*)nullptr;				// Azzera la matrice
		_col = _row = 0;
		return false;
		}
	if (!datnew)							// Se fallita allocazione
		{
		throw std::runtime_error(MatrixDef::ERR_ALLOC);
		}
	for (ir = 0, irold = 0; ir < rownew; ir++, irold++)
		{
		for (ic = 0, icold = 0; ic < colnew; ic++, icold++)
			{
			if (icold == icol)	icold++;	// Salta all'indice successivo, se uguale alla colonna da elimonare
			datnew[ir * colnew + ic] = dat[irold * _col + icold];	// Copia il valore
			}
		}
	if (dat)	delete[] dat;
	_row = rownew;
	_col = colnew;
	dat = datnew;
	#ifdef _DEBUG
	cout << "Matrix::rem_col(int icol)" << endl;
	return true;
	#endif
	}

/* Estrae una riga */
template <class DATA> Matrix<DATA> &Matrix<DATA>::get_row(int irow) requires RQassign<DATA>
	{
	int ic;
	if ((irow < 0) || (irow >= _row))
		{
		throw std::runtime_error(MatrixDef::ERR_OUTOFBOUND);
		}
	Matrix<DATA> *m = new Matrix<DATA>(1,_col);
	for (ic = 0; ic < _col; ic++)
		{
		m->dat[ic] = dat[irow * _col + ic];
		}
	#ifdef _DEBUG
	cout << "Matrix::get_row(int irow)" << endl;
	#endif
	return *m;	
	}

/* Estrae una colonna */
template <class DATA> Matrix<DATA> &Matrix<DATA>::get_col(int icol) requires RQassign<DATA>
	{
	int ir;
	if ((icol < 0) || (icol >= _col))
		{
		throw std::runtime_error(MatrixDef::ERR_OUTOFBOUND);
		}
	Matrix<DATA> *m = new Matrix<DATA>(_row, 1);
	for (ir = 0; ir < _row; ir++)
		{
		m->dat[ir] = dat[ir * _col + icol];
		}
	#ifdef _DEBUG
	cout << "Matrix::get_col(int icol)" << endl;
	#endif
	return *m;
	}

/* Estrae una sottomatrice */
template <class DATA> Matrix<DATA> &Matrix<DATA>::get_sub(int row_ini, int col_ini, int n_row, int n_col) requires RQassign<DATA>
	{
	if ((n_row < 0) || (n_col < 0))
		{
		throw std::runtime_error(MatrixDef::ERR_WRONGPARAM);
		}
	int row_fin = row_ini + n_row -1;
	int col_fin = col_ini + n_col -1;
	if ( (row_ini < 0) || (row_fin >= _row) || (col_ini < 0) || (col_fin >= _col))
		{
		throw std::runtime_error(MatrixDef::ERR_OUTOFBOUND);
		}
	Matrix<DATA> *m = new Matrix<DATA>(row_fin - row_ini + 1, col_fin - col_ini + 1);
	if ((m->_row > 0) && (m->_col > 0))
		{
		int ir,ic;
		for (ir = 0; ir < n_row; ir++)
			{
			for (ic = 0; ic < n_col; ic++)
				{
				m->dat[ir * m->_col + ic] = dat[(row_ini+ir) * _col + (col_ini+ic)];
				}
			}
		}
	#ifdef _DEBUG
	cout << "Matrix::get_sub(int row_ini, int col_ini, int n_row, int n_col)" << endl;
	#endif
	return *m;
	}

/* Indici valore minimo */
template <class DATA> pair<int, int> &Matrix<DATA>::min_row_col() requires RQorder<DATA>
	{
	pair<int,int> *r = new pair<int,int>(-1,-1);
	DATA tmp;
	if( (_row > 0) && (_col > 0) )
		{
		int ir, ic;
		tmp = dat[0];
		ir = ic = 0;
		for (ir = 0; ir < _row; ir++)
			{
			for (ic = 0; ic < _col; ic++)
				{
				if(dat[ir * _col + ic] < tmp)
					{
					tmp = dat[ir * _col + ic];
					r->first = ir;
					r->second = ic;
					}
				}
			}
		}
	#ifdef _DEBUG
	cout << "Matrix::min_row_col()" << endl;
	#endif
	return *r;
	}

/* Indici valore massimo */
template <class DATA> pair<int, int> &Matrix<DATA>::max_row_col() requires RQorder<DATA>
	{
	pair<int, int> *r = new pair<int, int>(-1, -1);
	DATA tmp;
	if ((_row > 0) && (_col > 0))
		{
		int ir, ic;
		tmp = dat[0];
		ir = ic = 0;
		for (ir = 0; ir < _row; ir++)
			{
			for (ic = 0; ic < _col; ic++)
				{
				if (dat[ir * _col + ic] > tmp)
					{
					tmp = dat[ir * _col + ic];
					r->first = ir;
					r->second = ic;
					}
				}
			}
		}
	#ifdef _DEBUG
	cout << "Matrix::max_row_col()" << endl;
	#endif
	return *r;
	}

/* Somma, differenza e prodotto di matrici */
template <class DATA> Matrix<DATA> &Matrix<DATA>::operator+(const Matrix <DATA> &m) requires RQsum<DATA>
	{
	if ((m._row != _row) || (m._col != _col))
		{
		throw std::runtime_error(MatrixDef::ERR_SZMISMATCH);
		}
	Matrix<DATA> *tmp = new Matrix<DATA>();							// Alloca nuova Matrix vuota
	int ir, ic;
	if ((_row > 0) && (_col > 0))
		{
		tmp->dat = new DATA[_row * _col];							// Alloca spazio.
		if (tmp->dat)
			{
			for (ir = 0; ir < _row; ir++)							// Somma
				for (ic = 0; ic < _col; ic++)
					tmp->dat[ir * _col + ic] = dat[ir * _col + ic] + m.dat[ir * _col + ic];
			tmp->_row = _row;
			tmp->_col = _col;
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
	cout << "Matrix::operator+(const Matrix <DATA> &m)" << endl;
	#endif
	return *tmp;
	}
template <class DATA> Matrix<DATA> &Matrix<DATA>::operator+=(const Matrix <DATA> &m) requires RQsum<DATA>
	{
	if ((m._row != _row) || (m._col != _col))
		{
		throw std::runtime_error(MatrixDef::ERR_SZMISMATCH);
		}
	int ir, ic;														// Non alloca nulla, non deve cambiare dimensioni della matrice.
	if ((_row > 0) && (_col > 0))
		{
		for (ir = 0; ir < _row; ir++)								// Somma
			for (ic = 0; ic < _col; ic++)
				dat[ir * _col + ic] = dat[ir * _col + ic] + m.dat[ir * _col + ic];
		} 
	#ifdef _DEBUG
	cout << "Matrix::operator+=(const Matrix <DATA> &m)" << endl;
	#endif
	return *this;
	}
template <class DATA> Matrix<DATA> &Matrix<DATA>::operator-(const Matrix <DATA> &m) requires RQsumdifprod<DATA>
	{
	if ((m._row != _row) || (m._col != _col))
		{
		throw std::runtime_error(MatrixDef::ERR_SZMISMATCH);
		}
	Matrix<DATA> *tmp = new Matrix<DATA>();							// Alloca nuova Matrix vuota
	int ir, ic;
	if ((_row > 0) && (_col > 0))
		{
		tmp->dat = new DATA[_row * _col];							// Alloca spazio.
		if (tmp->dat)
			{
			for (ir = 0; ir < _row; ir++)							// Somma
				for (ic = 0; ic < _col; ic++)
					tmp->dat[ir * _col + ic] = dat[ir * _col + ic] - m.dat[ir * _col + ic];
			tmp->_row = _row;
			tmp->_col = _col;
			} else
			{
			tmp->_row = tmp->_col = 0;
			throw std::runtime_error(MatrixDef::ERR_ALLOC);
			}
		} else
		{															// Matrice nulla
		tmp->_row = tmp->_col = 0;
		tmp->dat = (DATA*)nullptr;
		}
		#ifdef _DEBUG
		cout << "Matrix::operator-(const Matrix <DATA> &m)" << endl;
		#endif
		return *tmp;
	}
template <class DATA> Matrix<DATA> &Matrix<DATA>::operator-=(const Matrix <DATA> &m) requires RQsumdifprod<DATA>
	{
	if ((m._row != _row) || (m._col != _col))
		{
		throw std::runtime_error(MatrixDef::ERR_SZMISMATCH);
		}
	int ir, ic;														// Non alloca nulla, non deve cambiare dimensioni della matrice.
	if ((_row > 0) && (_col > 0))
		{
		for (ir = 0; ir < _row; ir++)								// Somma
			for (ic = 0; ic < _col; ic++)
				dat[ir * _col + ic] = dat[ir * _col + ic] - m.dat[ir * _col + ic];
		}
	#ifdef _DEBUG
	cout << "Matrix::operator-=(const Matrix <DATA> &m)" << endl;
	#endif
	return *this;
	}
template <class DATA> Matrix<DATA> &Matrix<DATA>::operator*(const Matrix <DATA> &m) requires RQsumdifprod<DATA>
	{
	if (_col != m._row)
		{
		throw std::runtime_error(MatrixDef::ERR_SZMISMATCH);
		}
	Matrix<DATA> *tmp = new Matrix<DATA>();				// Alloca nuova Matrix vuota
	int ir, ic, cc;
	if ((_row > 0) && (m._col > 0))
		{
		DATA sum;
		tmp->dat = new DATA[_row * m._col];				// Alloca spazio.
		if (tmp->dat)
			{
			for (ir = 0; ir < _row; ir++)
				for (ic = 0; ic < m._col; ic++)
					{
					sum = (DATA) 0;
					for (cc = 0; cc < _col; cc++)		// Ciclo per sommatoria
						{
						sum += dat[ir * _col + cc] * m.dat[cc * m._col + ic];
						}
					tmp->dat[ir * m._col + ic] = sum;
					}
			tmp->_row = _row;
			tmp->_col = m._col;
			}
		else
			{
			tmp->_row = tmp->_col = 0;
			throw std::runtime_error(MatrixDef::ERR_ALLOC);
			}
		}
	else
		{												// Matrice nulla
		tmp->_row = tmp->_col = 0;
		tmp->dat = (DATA*)nullptr;
		}
	#ifdef _DEBUG
	cout << "Matrix::operator*(const Matrix <DATA> &m)" << endl;
	#endif
	return *tmp;
	}
template <class DATA> Matrix<DATA> &Matrix<DATA>::operator*=(const Matrix <DATA> &m) requires RQsumdifprod<DATA>
	{
	if (_col != m._row)
		{
		throw std::runtime_error(MatrixDef::ERR_SZMISMATCH);
		}
	Matrix<DATA> *tmp = new Matrix<DATA>();				// Alloca nuova Matrix vuota
	int ir, ic, cc;
	if ((_row > 0) && (m._col > 0))
		{
		DATA sum;
		tmp->dat = new DATA[_row * m._col];				// Alloca spazio.
		if (tmp->dat)
			{
			for (ir = 0; ir < _row; ir++)
				for (ic = 0; ic < m._col; ic++)
					{
					sum = (DATA)0;
					for (cc = 0; cc < _col; cc++)		// Ciclo per sommatoria
						{
						sum += dat[ir * _col + cc] * m.dat[cc * m._col + ic];
						}
					tmp->dat[ir * m._col + ic] = sum;
					}
			tmp->_row = _row;
			tmp->_col = m._col;
			}
		else
			{
			tmp->_row = tmp->_col = 0;
			throw std::runtime_error(MatrixDef::ERR_ALLOC);
			}
		}
	else
		{												// Matrice nulla
		tmp->_row = tmp->_col = 0;
		tmp->dat = (DATA*)nullptr;
		}
	if(dat != (DATA*)nullptr)							// Dealloca (in this) lo spazio per la matrice 
		delete[] dat;
	dat = tmp->dat;										// Assegna (a this) i valori di tmp 
	_row = tmp->_row;
	_col = tmp->_col;
	#ifdef _DEBUG
	cout << "Matrix::operator*=(const Matrix <DATA> &m)" << endl;
	#endif
	return *this;
	}

/* Prodotto scalare tra vettori */
template <class DATA> DATA &Matrix<DATA>::operator^(const Matrix <DATA> &m) requires RQsumdifprod<DATA>
	{
	if ((_row != m._row) || (_col != m._col) || (_row==0) || (_col==0))
		{
		throw std::runtime_error(MatrixDef::ERR_SZMISMATCH);
		}
	if ((_row != 1) && (_col != 1))
		{
		throw std::runtime_error(MatrixDef::ERR_NOTVECTOR);
		}
	DATA *tmp = new DATA;
	*tmp = (DATA) 0;
	int ir, ic;
	for(ir=0; ir<_row; ir++)
		for(ic=0; ic<_col; ic++)
			*tmp += dat[ir * _col + ic] * m.dat[ir * _col + ic];
	#ifdef _DEBUG
	cout << "Matrix::operator^(const Matrix <DATA> &m)" << endl;
	#endif
	return *tmp;
	}

/* Prodotto con scalare */
template <class DATA> Matrix <DATA> &Matrix<DATA>::operator*(const DATA &x) requires RQsumdifprod<DATA>
	{
	Matrix<DATA> *tmp = new Matrix<DATA>();							// Alloca nuova Matrix vuota
	int ir, ic;
	if ((_row > 0) && (_col > 0))
		{
		tmp->dat = new DATA[_row * _col];							// Alloca spazio.
		if (tmp->dat)
			{
			for (ir = 0; ir < _row; ir++)							// Somma
				for (ic = 0; ic < _col; ic++)
					tmp->dat[ir * _col + ic] = dat[ir * _col + ic] * x;
			tmp->_row = _row;
			tmp->_col = _col;
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
		cout << "Matrix::operator*(const DATA &x)" << endl;
		#endif
		return *tmp;
	}

/* Divisione con scalare */
template <class DATA> Matrix<DATA> &Matrix<DATA>::operator/(const DATA &x) requires RQsumdifprod<DATA>
	{
	Matrix<DATA> *tmp = new Matrix<DATA>();							// Alloca nuova Matrix vuota
	int ir, ic;
	if ((_row > 0) && (_col > 0))
		{
		tmp->dat = new DATA[_row * _col];							// Alloca spazio.
		if (tmp->dat)
			{
			for (ir = 0; ir < _row; ir++)							// Somma
				for (ic = 0; ic < _col; ic++)
					tmp->dat[ir * _col + ic] = dat[ir * _col + ic] / x;		// Possibile throw
			tmp->_row = _row;
			tmp->_col = _col;
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
	cout << "Matrix::operator/(const DATA &x)" << endl;
	#endif
	return *tmp;
	}

template <class DATA> const Matrix<DATA> &Matrix<DATA>::Id(int sz) requires RQsumdifprod<DATA>
	{
	if (sz <= 0)
		{
		throw std::runtime_error(MatrixDef::ERR_WRONGPARAM);
		}
	Matrix<DATA> *tmp = new Matrix<DATA>(sz,sz,(DATA)0);
	int irc;
	for (irc = 0; irc < sz; irc++)
		tmp->dat[irc * sz + irc] = (DATA) 1;
	return *tmp;
	}

/* Inizializza membri statici */
template <class DATA> DATA *Matrix<DATA>::_empty = new DATA;			// Costruttore di default di DATA. Deallocato a fine programma.
template <class DATA> size_t Matrix<DATA>::_datasize = sizeof(*_empty);	// Dimensione di DATA

#endif