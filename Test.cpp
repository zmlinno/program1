#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h> // OpenMP for multi-threading

// ����ϡ����������
struct SparseMatrix {
    int rows, cols;
    std::vector<int> row_ptr, col_idx;
    std::vector<double> values;
};

// ��ʼ��CSR��ʽ��ϡ�����
SparseMatrix initCSR(int size) {
    SparseMatrix matrix;
    matrix.rows = size;
    matrix.cols = size;

    // ʾ��ϡ���������
    matrix.row_ptr = { 0, 2, 3, 5, 6 }; // ָ���п�ʼ������
    matrix.col_idx = { 0, 2, 1, 3, 0, 2 }; // ����Ԫ�ص�������
    matrix.values = { 1.0, 4.0, 3.0, 7.0, 5.0, 9.0 }; // ����Ԫ�ص�ֵ

    return matrix;
}

// ϡ�����-�����˷���CSR��ʽ��
std::vector<double> spmvCSR(const SparseMatrix& matrix, const std::vector<double>& x) {
    std::vector<double> result(matrix.rows, 0.0);

#pragma omp parallel for
    for (int i = 0; i < matrix.rows; ++i) {
        for (int j = matrix.row_ptr[i]; j < matrix.row_ptr[i + 1]; ++j) {
            result[i] += matrix.values[j] * x[matrix.col_idx[j]];
        }
    }

    return result;
}

// ģ�� CSCV ��ʽ�� SpMV ����
std::vector<double> spmvCSCV(const SparseMatrix& matrix, const std::vector<double>& x, bool removePadding) {
    std::vector<double> result(matrix.rows, 0.0);

#pragma omp parallel for
    for (int i = 0; i < matrix.rows; ++i) {
        for (int j = matrix.row_ptr[i]; j < matrix.row_ptr[i + 1]; ++j) {
            // ģ��ȥ������㣨���� removePadding ���ƣ�
            if (removePadding && matrix.values[j] != 0) {
                result[i] += matrix.values[j] * x[matrix.col_idx[j]];
            }
            else if (!removePadding) {
                result[i] += matrix.values[j] * x[matrix.col_idx[j]];
            }
        }
    }

    return result;
}

// ���Ժ���
void performanceTest(const SparseMatrix& matrix, const std::vector<double>& x, bool useCSCV, bool removePadding) {
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<double> result;
    if (useCSCV) {
        result = spmvCSCV(matrix, x, removePadding);
    }
    else {
        result = spmvCSR(matrix, x);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << (useCSCV ? "CSCV" : "CSR")
        << (removePadding ? " with padding removal" : " without padding removal")
        << " execution time: " << elapsed.count() << " seconds\n";
}

int main() {
    // ��ʼ�����������
    int size = 4; // ʾ����С
    SparseMatrix matrix = initCSR(size);
    std::vector<double> x = { 1.0, 2.0, 3.0, 4.0 }; // ʾ������

    // ���� OpenMP ʹ�õ��߳���
    omp_set_num_threads(4);

    // ���ܲ���
    std::cout << "Testing SpMV performance...\n";
    performanceTest(matrix, x, false, false); // CSR
    performanceTest(matrix, x, true, false);  // CSCV without padding removal
    performanceTest(matrix, x, true, true);   // CSCV with padding removal

    return 0;
}
