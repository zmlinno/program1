#define _CRT_SECURE_NO_WARNINGS
#include "MatrixOperations.h"
#include <vector>
#include <iostream>
#include <omp.h>
#include <string>
#include <sstream>
#include <fstream>

//int main() {
//    // 初始化矩阵和向量
//    int size = 10000; // 示例大小
//    SparseMatrix matrix;
//    matrix.rows = size;
//    matrix.cols = size;
//    matrix.col_ptr = { 0, 2, 3, 4, 5 }; // 示例数据
//    matrix.row_idx = { 0, 1, 1, 2, 3 }; // 示例数据
//    matrix.values = { 1.0, 4.0, 3.0, 7.0, 5.0 }; // 非零元素的值
//
//    std::vector<double> x = { 1.0, 2.0, 3.0, 4.0 }; // 示例向量
//
//    // 设置 OpenMP 使用的线程数
//    omp_set_num_threads(4);
//
//    // 性能测试
//    std::cout << "Testing SpMV performance...\n";
//    performanceTest(matrix, x, false, false, false); // CSR
//    performanceTest(matrix, x, true, false, false);  // CSCV without padding removal
//    performanceTest(matrix, x, true, false, true);   // CSCV with padding removal
//    performanceTest(matrix, x, true, true, false);   // CSCV + VxG
//
//    return 0;
//}


//int main()
//{
//	int size = 10000;
//	SparseMatrix matrix;
//	matrix.rows = size;
//	matrix.cols = size;
//
//	//生成随机稀疏矩阵数据
//	for (int i = 0; i < size; i++)
//	{
//		matrix.col_ptr.push_back(matrix.values.size());
//		for (int j = 0; j < size; j++)
//		{
//			if (rand() % 100 < 5)
//			{
//				matrix.values.push_back(rand() % 100 + 1);
//				matrix.row_idx.push_back(j);
//			}
//		}
//	}
//	matrix.col_ptr.push_back(matrix.values.size());
//	std::vector<double> x(size, 1.0);
//
//
//	//设置OpenMP使用的线程数
//	omp_set_num_threads(4);
//
//	//性能测试
//	std::cout << "Testing SpMV performance...\n";
//	performanceTest(matrix, x, false, false, false); // CSR
//	performanceTest(matrix, x, true, false, false);  // CSCV without padding removal
//	performanceTest(matrix, x, true, false, true);   // CSCV with padding removal
//	performanceTest(matrix, x, true, true, false);   // CSCV + VxG
//	performanceTest(matrix, x, true, true, false); // 确保调用优化后的版本
//
//	return 0;
//}



//int main() {
//    // 设置矩阵大小和稀疏度
//    int size = 10000;  // 矩阵大小
//    double sparsity = 0.8;  // 稀疏度（80% 的元素为零）
//
//    SparseMatrix matrix;
//
//    // 调用生成随机稀疏矩阵的函数
//    generateRandomSparseMatrix(matrix, size, size, sparsity);  // 传递三个参数
//
//    // 假设 new_order 是一个包含重新排序索引的向量，根据你的应用场景定义
//    std::vector<int> new_order(size);
//    for (int i = 0; i < size; ++i) {
//        new_order[i] = i;  // 初始化为顺序索引，可以根据具体需求修改
//    }
//
//    // 1. 使用 IOBLR 重新排序矩阵
//    reorderMatrixBasedOnIOBLR(matrix, new_order);
//
//    // 2. 去除零填充
//    removeZeroPadding(matrix);
//
//    std::vector<double> x(size, 1.0); // 向量初始化为全1
//
//    // 性能测试
//    std::cout << "Testing SpMV performance with IOBLR and zero-padding removal...\n";
//    performanceTest(matrix, x, true, true, false); // 使用优化后的矩阵进行测试
//
//    return 0;
//}




// 函数：读取投影数据文件
std::vector<std::vector<double>> readProjectionData(const std::string& filename) {
    std::vector<std::vector<double>> projections;
    std::ifstream infile(filename);
    std::string line;

    if (!infile.is_open()) {
        std::cerr << "无法打开文件: " << filename << std::endl;
        return projections;
    }

    while (std::getline(infile, line)) {
        if (line.empty()) continue;  // 跳过空行
        std::vector<double> row;
        std::istringstream iss(line);
        double value;

        // 打印读取的每一行
        std::cout << "Reading line: " << line << std::endl;

        while (iss >> value) {
            row.push_back(value);
        }

        projections.push_back(row);
    }

    infile.close();
    return projections;
}



// 构建系统矩阵的示例函数
void constructSystemMatrix(SparseMatrix& systemMatrix, const std::vector<std::vector<double>>& projections) {
    systemMatrix.rows = projections.size();    // 行数等于投影数据的行数
    systemMatrix.cols = projections[0].size(); // 列数等于投影数据的列数
    systemMatrix.col_ptr.push_back(0);

    // 示例：简单构建一个稀疏矩阵
    for (int i = 0; i < systemMatrix.cols; ++i) {
        for (int j = 0; j < systemMatrix.rows; ++j) {
            if (projections[j][i] > 0) {  // 仅将非零元素添加到稀疏矩阵
                systemMatrix.values.push_back(projections[j][i]);
                systemMatrix.row_idx.push_back(j);
            }
        }
        systemMatrix.col_ptr.push_back(systemMatrix.values.size());
    }
}



// 初始化图像向量
std::vector<double> initializeImage(int size) {
    return std::vector<double>(size, 0.0);  // 将图像向量初始化为全零
}


void reconstructCTImage(SparseMatrix& systemMatrix, const std::vector<std::vector<double>>& projections, std::vector<double>& image, int iterations) {
    for (int iter = 0; iter < iterations; ++iter) {
        // 前向投影
        std::vector<double> forwardProjection = spmvCSCV_VxG(systemMatrix, image);

        // 计算误差
        std::vector<double> error(projections.size());
        for (size_t i = 0; i < projections.size(); ++i) {
            for (size_t j = 0; j < projections[i].size(); ++j) {
                error[i] = projections[i][j] - forwardProjection[i];
            }
        }

        // 后向投影
        std::vector<double> backProjection = spmvCSCV_VxG_transpose(systemMatrix, error);

        // 更新图像
        for (size_t i = 0; i < image.size(); ++i) {
            image[i] += backProjection[i];
        }

        std::cout << "Iteration " << iter + 1 << " complete." << std::endl;
    }
}

//int main() {
//    // 读取投影数据
//    std::vector<std::vector<double>> projections = readProjectionData("projections.txt");
//
//    // 构建稀疏系统矩阵
//    SparseMatrix systemMatrix;
//    constructSystemMatrix(systemMatrix, projections);
//
//    // 初始化图像向量
//    int imageSize = projections.size();  // 假设图像大小与投影数据行数相同
//    std::vector<double> image = initializeImage(imageSize);
//
//    // 设置迭代次数
//    int iterations = 10;
//
//    // 开始图像重建
//    reconstructCTImage(systemMatrix, projections, image, iterations);
//
//    // 输出重建的图像
//    std::cout << "重建完成的图像数据：" << std::endl;
//    for (const auto& val : image) {
//        std::cout << val << " ";
//    }
//    std::cout << std::endl;
//
//    return 0;
//}





void saveImageToFile(const std::vector<double>& image, const std::string& filename) {
    std::ofstream outfile(filename);
    if (outfile.is_open()) {
        for (const auto& val : image) {
            outfile << val << std::endl;
        }
        outfile.close();
    }
    else {
        std::cerr << "无法打开文件 " << filename << " 进行写入。" << std::endl;
    }
}

int main() {
    // 重建完成后保存图像数据
   // reconstructCTImage(systemMatrix, projections, image, 10);
    saveImageToFile(image, "reconstructed_image.txt");
    return 0;
}

//int main() {
//    // 指定投影数据文件的路径
//    std::string filename = "projections.txt";
//
//    // 读取投影数据
//    std::vector<std::vector<double>> projections = readProjectionData(filename);
//
//    // 输出读取的投影数据进行检查
//    if (!projections.empty()) {
//        for (size_t i = 0; i < std::min<size_t>(5, projections.size()); ++i) {
//            for (size_t j = 0; j < std::min<size_t>(5, projections[i].size()); ++j) {
//                std::cout << projections[i][j] << " ";
//            }
//            std::cout << std::endl;
//        }
//    }
//    else {
//        std::cout << "没有读取到任何投影数据。" << std::endl;
//    }
//
//    return 0;
//}