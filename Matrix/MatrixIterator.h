#ifndef MATRIXITERATORH
#define MATRIXITERATORH


/*************************************/
/*                                   */
/*  MatrixIterator.h                 */
/*                                   */
/*************************************/

#include "Matrix.h"


namespace matrix
{

/* Prototyping */
template<class DATA> class Matrix;

template<class DATA> class MatrixIterator
    {
	private:
		Matrix<DATA> *_m;
		int ir, ic;
	public:
		/* Ctor con argomenti */
		MatrixIterator(Matrix<DATA> &m)
			{
			_m = &m;
			ir = ic = 0;
			_m->_iterators++;
			};
		/* Dtor con argomenti */
		~MatrixIterator()
			{
			_m->_iterators--;
			}
		/* Iteration */
		DATA *begin();
		DATA *end();
		DATA *next();
		DATA *peek()
			{
			return _m->dat + ir * _m->_col + ic;
			}
    };

#include "MatrixIterator.cpp"

}
#endif
