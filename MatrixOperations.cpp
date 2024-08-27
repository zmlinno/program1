#define _CRT_SECURE_NO_WARNINGS
// MatrixOperations.cpp
#include "MatrixOperations.h"
#include <omp.h>
#include <chrono>
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <immintrin.h> //����ʹ��SIMDָ�

std::vector<double> spmvCSCV(const SparseMatrix& matrix, const std::vector<double>& x, bool removePadding) {
    std::vector<double> result(matrix.rows, 0.0);

#pragma omp parallel for
    for (int i = 0; i < matrix.cols; ++i) {
        if (i + 1 >= matrix.col_ptr.size()) {
            continue; // ������ʳ�����Χ������
        }
        for (int j = matrix.col_ptr[i]; j < matrix.col_ptr[i + 1]; ++j) {
            if (removePadding && matrix.values[j] == 0.0) continue; // ȥ�������
            result[matrix.row_idx[j]] += matrix.values[j] * x[i];
        }
    }
    return result;
}

// ����ȷ�� row_ptr ��������Χ
std::vector<double> spmvCSR(const SparseMatrix& matrix, const std::vector<double>& x) {
    std::vector<double> result(matrix.rows, 0.0);

#pragma omp parallel for
    for (int i = 0; i < matrix.rows; ++i) {
        if (i + 1 >= matrix.row_ptr.size()) {
            continue; // ������ʳ�����Χ������
        }
        for (int j = matrix.row_ptr[i]; j < matrix.row_ptr[i + 1]; ++j) {
            result[i] += matrix.values[j] * x[matrix.col_idx[j]];
        }
    }
    return result;
}

// CSCV + VxG ��ʽ�� SpMV ʵ��
//std::vector<double> spmvCSCV_VxG(const SparseMatrix& matrix, const std::vector<double>& x) {
//    std::vector<double> result(matrix.rows, 0.0);
//
//    // ���� VxG ��ʵ���ǽ����CSCVE�ϲ�Ϊһ������
//    // �����ʵ��ֻ��һ��ʾ��
//#pragma omp parallel for
//    for (int i = 0; i < matrix.cols; i += 2) { // �򻯴���ÿ��������Ϊһ��VxG��Ԫ
//        for (int j = matrix.col_ptr[i]; j < matrix.col_ptr[i + 2]; ++j) {
//            result[matrix.row_idx[j]] += matrix.values[j] * x[i];
//        }
//    }
//    return result;
//}



// �Ż���� CSCV + VxG ʵ��
std::vector<double> spmvCSCV_VxG(const SparseMatrix& matrix, const std::vector<double>& x) {
    std::vector<double> result(matrix.rows, 0.0);

    // ʹ�� OpenMP ���в��д����������������е�����
#pragma omp parallel for
    for (int i = 0; i < matrix.cols; i += 4) {  // ÿ�δ���4��
        __m256d vec_x = _mm256_loadu_pd(&x[i]);  // �������� x �� 4 ��Ԫ��

        // ����4�еľ��������˻�
        for (int j = i; j < i + 4 && j < matrix.cols; ++j) {
            if (j + 1 >= matrix.col_ptr.size()) {
                continue; // ������ʳ�����Χ������
            }

            // Ԥȡ��һ���ڴ棬�Լ����ӳ�
            _mm_prefetch((const char*)&matrix.row_idx[matrix.col_ptr[j]], _MM_HINT_T0);
            _mm_prefetch((const char*)&matrix.values[matrix.col_ptr[j]], _MM_HINT_T0);

            for (int k = matrix.col_ptr[j]; k < matrix.col_ptr[j + 1]; ++k) {
                result[matrix.row_idx[k]] += matrix.values[k] * x[j];
            }
        }
    }

    return result;
}


// ���Ż��� CSCV + VxG ʵ��
std::vector<double> spmvCSCV_VxG_optimized(const SparseMatrix& matrix, const std::vector<double>& x) {
    std::vector<double> result(matrix.rows, 0.0);

#pragma omp parallel for
    for (size_t i = 0; i < matrix.cols; i += 4) {  // ʹ�� size_t ��������
        double local_result[4] = { 0.0, 0.0, 0.0, 0.0 };

        for (size_t j = 0; j < 4 && (i + j) < matrix.cols; ++j) {
            size_t col_start = matrix.col_ptr[i + j];
            size_t col_end = matrix.col_ptr[i + j + 1];

            _mm_prefetch((const char*)&matrix.row_idx[col_start], _MM_HINT_T0);
            _mm_prefetch((const char*)&matrix.values[col_start], _MM_HINT_T0);

            for (size_t k = col_start; k < col_end; ++k) {
                size_t row = matrix.row_idx[k];
                double value = matrix.values[k];
                local_result[j] += value * x[i + j];
            }
        }

        for (size_t j = 0; j < 4 && (i + j) < matrix.cols; ++j) {
            size_t col_start = matrix.col_ptr[i + j];
            size_t col_end = matrix.col_ptr[i + j + 1];

            for (size_t k = col_start; k < col_end; ++k) {
                size_t row = matrix.row_idx[k];
                result[row] += local_result[j];
            }
        }
    }

    return result;
}

// ���ܲ��Ժ���
void performanceTest(const SparseMatrix& matrix, const std::vector<double>& x, bool useCSCV, bool useVxG, bool removePadding) {
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<double> result;
    if (useCSCV) {
        if (useVxG) {
            result = spmvCSCV_VxG(matrix, x);
        }
        else {
            result = spmvCSCV(matrix, x, removePadding);
        }
    }
    else {
        result = spmvCSR(matrix, x);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << (useCSCV ? (useVxG ? "CSCV + VxG" : "CSCV") : "CSR")
        << (removePadding ? " with padding removal" : " without padding removal")
        << " execution time: " << elapsed.count() << " seconds\n";
}

// �������ϡ�����ĺ���
void generateRandomSparseMatrix(SparseMatrix& matrix, int rows, int cols, double sparsity) {
    matrix.rows = rows;
    matrix.cols = cols;
    matrix.col_ptr.push_back(0);

    srand(static_cast<unsigned int>(time(0)));  // ʹ�õ�ǰʱ����Ϊ���������

    for (size_t i = 0; i < cols; ++i) {
        for (size_t j = 0; j < rows; ++j) {
            if (static_cast<double>(rand()) / RAND_MAX > sparsity) {  // ����ϡ���
                double value = rand() % 100 + 1;  // �������ֵ
                matrix.values.push_back(value);
                matrix.row_idx.push_back(j);
            }
        }
        matrix.col_ptr.push_back(matrix.values.size());
    }
}

// IOBLR �ֲ�������ʵ��
void reorderMatrixBasedOnIOBLR(SparseMatrix& matrix, const std::vector<int>& new_order) {
    SparseMatrix reordered_matrix;
    reordered_matrix.rows = matrix.rows;
    reordered_matrix.cols = matrix.cols;
    reordered_matrix.col_ptr.resize(matrix.col_ptr.size());

    // ��ʱ����洢�������ķ���Ԫ��
    std::vector<double> temp_values;
    std::vector<int> temp_row_idx;

    // ���� new_order ��������
    for (size_t i = 0; i < matrix.cols; ++i) {
        int col_start = matrix.col_ptr[i];
        int col_end = matrix.col_ptr[i + 1];
        reordered_matrix.col_ptr[i] = temp_values.size();

        for (int j = col_start; j < col_end; ++j) {
            int old_row = matrix.row_idx[j];
            double value = matrix.values[j];
            int new_row = new_order[old_row];  // ʹ���µ���˳��

            temp_values.push_back(value);
            temp_row_idx.push_back(new_row);
        }
    }
    reordered_matrix.col_ptr[matrix.cols] = temp_values.size();
    reordered_matrix.values.swap(temp_values);
    reordered_matrix.row_idx.swap(temp_row_idx);

    // ����ԭʼ����
    matrix = reordered_matrix;
}




// ������Ƴ�����ʵ��
void removeZeroPadding(SparseMatrix& matrix) {
    SparseMatrix compact_matrix;
    compact_matrix.rows = matrix.rows;
    compact_matrix.cols = matrix.cols;
    compact_matrix.col_ptr.push_back(0);

    for (size_t i = 0; i < matrix.cols; ++i) {
        int col_start = matrix.col_ptr[i];
        int col_end = matrix.col_ptr[i + 1];

        for (int j = col_start; j < col_end; ++j) {
            int row = matrix.row_idx[j];
            double value = matrix.values[j];

            if (value != 0) {  // ȥ����Ԫ��
                compact_matrix.values.push_back(value);
                compact_matrix.row_idx.push_back(row);
            }
        }
        compact_matrix.col_ptr.push_back(compact_matrix.values.size());
    }

    // ����ԭʼ����
    matrix = compact_matrix;
}




std::vector<double> spmvCSCV_VxG_transpose(const SparseMatrix& matrix, const std::vector<double>& vector) {
    // ��ʼ���������
    std::vector<double> result(matrix.cols, 0.0);

    // ���������ÿһ��
    for (size_t i = 0; i < matrix.cols; ++i) {
        for (size_t j = matrix.col_ptr[i]; j < matrix.col_ptr[i + 1]; ++j) {
            int row = matrix.row_idx[j];
            result[i] += matrix.values[j] * vector[row];
        }
    }

    return result;
}