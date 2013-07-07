#ifndef FLENS_DATA_VECTOR_H
#define FLENS_DATA_VECTOR_H 1

#include <mpi.h>
#include <flens/flens.cxx>

#include "../Fem/Coupling.hpp"


namespace flens{

enum VectorType {typeI, typeII, nonMPI};

struct FLENSDataVector
		: public DenseVector<Array<double> >
{
	typedef double	ElementType;
    typedef int   	IndexType;

	//MPI Version (with Coupling, requires VectorType):
	explicit
	FLENSDataVector(int n, const Coupling &_coupling, const VectorType _vType)
    	: 	DenseVector<Array<double> >(n),
        	vType(_vType),
        	coupling(_coupling)
	{
	}
    
	//Non-MPI version (no coupling, assumes 'nonMPI' VectorType):
	explicit
	FLENSDataVector(int n)
    	:	DenseVector<Array<double> >(n),
    		coupling(Coupling())
	{
    	vType = nonMPI;
	}
    
	//Copy constructor:
	explicit
	FLENSDataVector(const FLENSDataVector &rhs)
    	:	DenseVector<Array<double> >(rhs),		//copy data via flens framework
    		vType(rhs.vType),
			coupling(rhs.coupling)
	{
	}
    

	//Member objects:
    VectorType vType;
	const Coupling &coupling;

	//Member methods:
	void typeII_2_I();
	void typeI_2_II();
	
	void commCrossPoints();
	void commBoundaryNodes();
	
	double* vec2c();
	

};

}	//namespace flens

namespace flens{ namespace blas{

void 
copy(FLENSDataVector &orig, FLENSDataVector &dest);

double
dot(FLENSDataVector &x1, FLENSDataVector &x2);

void
mv(Transpose trans, const double &alpha, const GeCRSMatrix<CRS<double, IndexOptions<int, 1> > > &A,
		FLENSDataVector &x, const double &beta, FLENSDataVector &y);


}	//namespace blas
}	//namespace flens


#endif	//FLENS_DATA_VECTOR_H