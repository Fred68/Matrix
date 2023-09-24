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
		static const int CMD_SZ = 1;
		static const string ERR_ALLOC;
		static const string ERR_OUTOFBOUND;
		static const string ERR_WRONGPARAM;
		static const string ERR_SZMISMATCH;
		static const string ERR_NOTVECTOR;
		static const string ERR_ZEROSIZE;
    };

}
#endif

