
#ifndef MATRIXITERATORCPP
#define MATRIXITERATORCPP


#include "MatrixIterator.h"

using namespace matrix;

template <class DATA> DATA *MatrixIterator<DATA>::begin()
	{
	ir = ic = 0;
	if ((_m->_row == 0) || (_m->_col == 0))
		{
		throw std::runtime_error(MatrixDef::ERR_ZEROSIZE);
		}
	return _m->dat;
	}

template <class DATA> DATA *MatrixIterator<DATA>::end()
	{
	ir = _m->_row - 1;
	ic = _m->_col - 1;
	if ((ir < 0) || (ic < 0))
		{
		throw std::runtime_error(MatrixDef::ERR_ZEROSIZE);
		}
	return _m->dat[ir * _m->_col + ic];
	}

template <class DATA> DATA *MatrixIterator<DATA>::next()
	{
	ic++;
	if (ic >= _m->_col)
		{
		ic = 0;
		ir ++;
		}
	if (ir >= _m->_row)				// Oltre fine matrice
		{
		ic = ir = 0;				// Azzera l'iteratore
		return (DATA*) nullptr;
		}
	return _m->dat + ir * _m->_col + ic;
	}

#endif