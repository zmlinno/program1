#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
// MatrixOperations.h
#ifndef MATRIX_OPERATIONS_H
#define MATRIX_OPERATIONS_H

#include <vector>
extern std::vector<double> image;
struct SparseMatrix {
    int rows;
    int cols;
    std::vector<int> col_ptr;
    std::vector<int> row_idx;
    std::vector<int> row_ptr;
    std::vector<int> col_idx;
    std::vector<double> values;
};

std::vector<double> spmvCSR(const SparseMatrix& matrix, const std::vector<double>& x);
std::vector<double> spmvCSCV(const SparseMatrix& matrix, const std::vector<double>& x, bool removePadding);
//std::vector<double> spmvCSCV_VxG(const SparseMatrix& matrix, const std::vector<double>& x);
std::vector<double> spmvCSCV_VxG(const SparseMatrix& matrix, const std::vector<double>& x);
std::vector<double> spmvCSCV_VxG_optimized(const SparseMatrix& matrix, const std::vector<double>& x);
std::vector<double> spmvCSCV_VxG_transpose(const SparseMatrix& matrix, const std::vector<double>& vector);
std::vector<double> image(256 * 256, 0.0);
void performanceTest(const SparseMatrix& matrix, const std::vector<double>& x, bool useCSCV, bool useVxG, bool removePadding);

void reorderMatrixBasedOnIOBLR(SparseMatrix& matrix, const std::vector<int>& new_order);
void removeZeroPadding(SparseMatrix& matrix);
void generateRandomSparseMatrix(SparseMatrix& matrix, int rows, int cols, double sparsity);
#endif // MATRIX_OPERATIONS_H
