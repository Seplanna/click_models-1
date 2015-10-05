#ifndef MATRIX_TOOLS
#define MATRIX_TOOLS

#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <cassert>
 
namespace ublas = boost::numeric::ublas;

using ublas::matrix;
using ublas::permutation_matrix;
using ublas::lu_factorize;
using ublas::identity_matrix;
 
/**
* @brief	Matrix inversion routine. Uses lu_factorize and lu_substitute in uBLAS to invert a matrix
* @param[in]	input - matrix to inverse
* @param[out]	inverse - inversed matrix
*/
template<class T> bool InvertMatrix(
    const matrix<T>& input
    , matrix<T>* inverse) 
{
    assert(input.size1() == input.size2());
    size_t size = input.size1();
    // Copy matrix and create permuation matrix
    matrix<T> matrixCopy(input);
    permutation_matrix<size_t> permutationMatrix(size);
    // perform LU-factorization
    if (lu_factorize(matrixCopy, permutationMatrix) != 0) 
    {
        return false;
    }
    // create identity matrix of "inverse"
    inverse->assign(identity_matrix<T>(size));
    // backsubstitute to get the inverse
    lu_substitute(matrixCopy, permutationMatrix, *inverse);
    return true;
}

template<class T> T CalculateDeterminant(const matrix<T>& input) 
{
    assert(input.size1() == input.size2());
    size_t size = input.size1();
    // Copy matrix and create permuation matrix
    matrix<T> matrixCopy(input);
    permutation_matrix<size_t> permutationMatrix(size);
    double determinant = 1.0;
    // Factorize
    if (lu_factorize(matrixCopy, permutationMatrix) != 0) 
    {
        determinant = 0.0;
    }
    // Calculate determinant without size
    for(size_t i = 0; i < size; ++i)
    {
        determinant *= matrixCopy(i,i);
    }
    // Calculate size of the determinant
    for (size_t i = 0; i < size; ++i)
    {
        if (i != permutationMatrix(i))
        {
            determinant *= -1.0;
        }
    }
    // Return value
    return determinant;
}

template<class T> 
bool CalculateLogDeterminantAndInverseMatrix(
    const matrix<T>& input
    , ublas::matrix<T>* inverse
    , double* logDeterminant)
{
    assert(input.size1() == input.size2());
    size_t size = input.size1();
    // Copy matrix and create permuation matrix
    matrix<T> matrixCopy(input);
    permutation_matrix<size_t> permutationMatrix(size);
    // perform LU-factorization
    if (lu_factorize(matrixCopy, permutationMatrix) != 0) 
    {
        *logDeterminant = -1;
        return false;
    }
    *logDeterminant = 0.0;
    // Calculate determinant without size
    for(size_t i = 0; i < size; ++i)
    {
        *logDeterminant += std::log(std::abs(matrixCopy(i,i)));
    }
    // create identity matrix of "inverse"
    inverse->assign(identity_matrix<T>(size));
    // backsubstitute to get the inverse
    lu_substitute(matrixCopy, permutationMatrix, *inverse);
    return true;
}
 
#endif // MATRIX_TOOLS
