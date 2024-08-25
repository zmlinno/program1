#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h> // OpenMP for multi-threading

// 定义稀疏矩阵和向量
struct SparseMatrix {
    int rows, cols;
    std::vector<int> row_ptr, col_idx;
    std::vector<double> values;
};

// 初始化CSR格式的稀疏矩阵
SparseMatrix initCSR(int size) {
    SparseMatrix matrix;
    matrix.rows = size;
    matrix.cols = size;

    // 示例稀疏矩阵数据
    matrix.row_ptr = { 0, 2, 3, 5, 6 }; // 指定行开始的索引
    matrix.col_idx = { 0, 2, 1, 3, 0, 2 }; // 非零元素的列索引
    matrix.values = { 1.0, 4.0, 3.0, 7.0, 5.0, 9.0 }; // 非零元素的值

    return matrix;
}

// 稀疏矩阵-向量乘法（CSR格式）
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

// 模拟 CSCV 格式的 SpMV 操作
std::vector<double> spmvCSCV(const SparseMatrix& matrix, const std::vector<double>& x, bool removePadding) {
    std::vector<double> result(matrix.rows, 0.0);

#pragma omp parallel for
    for (int i = 0; i < matrix.rows; ++i) {
        for (int j = matrix.row_ptr[i]; j < matrix.row_ptr[i + 1]; ++j) {
            // 模拟去除填充零（假设 removePadding 控制）
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

// 测试函数
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
    // 初始化矩阵和向量
    int size = 4; // 示例大小
    SparseMatrix matrix = initCSR(size);
    std::vector<double> x = { 1.0, 2.0, 3.0, 4.0 }; // 示例向量

    // 设置 OpenMP 使用的线程数
    omp_set_num_threads(4);

    // 性能测试
    std::cout << "Testing SpMV performance...\n";
    performanceTest(matrix, x, false, false); // CSR
    performanceTest(matrix, x, true, false);  // CSCV without padding removal
    performanceTest(matrix, x, true, true);   // CSCV with padding removal

    return 0;
}
