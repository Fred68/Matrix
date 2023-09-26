#ifndef MATRIXDEFH
#define MATRIXDEFH


/*************************************/
/*                                   */
/*  MatrixDef.h                  */
/*                                   */
/*************************************/



#include <string>
using std::string;

namespace matrix
{
class MatrixDef
    {
	public:
		enum class Cmd	{size, detail};
		static const string ERR_ALLOC;
		static const string ERR_OUTOFBOUND;
		static const string ERR_WRONGPARAM;
		static const string ERR_SZMISMATCH;
		static const string ERR_NOTVECTOR;
		static const string ERR_ZEROSIZE;
		static const string ERR_NOTSQUARE;
		static const string ERR_SINGULAR;
		static const string ERR_PIVOT;
    };

}
#endif

