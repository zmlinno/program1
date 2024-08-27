#define _CRT_SECURE_NO_WARNINGS
#include "MatrixOperations.h"
#include <vector>
#include <iostream>
#include <omp.h>
#include <string>
#include <sstream>
#include <fstream>

//int main() {
//    // ��ʼ�����������
//    int size = 10000; // ʾ����С
//    SparseMatrix matrix;
//    matrix.rows = size;
//    matrix.cols = size;
//    matrix.col_ptr = { 0, 2, 3, 4, 5 }; // ʾ������
//    matrix.row_idx = { 0, 1, 1, 2, 3 }; // ʾ������
//    matrix.values = { 1.0, 4.0, 3.0, 7.0, 5.0 }; // ����Ԫ�ص�ֵ
//
//    std::vector<double> x = { 1.0, 2.0, 3.0, 4.0 }; // ʾ������
//
//    // ���� OpenMP ʹ�õ��߳���
//    omp_set_num_threads(4);
//
//    // ���ܲ���
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
//	//�������ϡ���������
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
//	//����OpenMPʹ�õ��߳���
//	omp_set_num_threads(4);
//
//	//���ܲ���
//	std::cout << "Testing SpMV performance...\n";
//	performanceTest(matrix, x, false, false, false); // CSR
//	performanceTest(matrix, x, true, false, false);  // CSCV without padding removal
//	performanceTest(matrix, x, true, false, true);   // CSCV with padding removal
//	performanceTest(matrix, x, true, true, false);   // CSCV + VxG
//	performanceTest(matrix, x, true, true, false); // ȷ�������Ż���İ汾
//
//	return 0;
//}



//int main() {
//    // ���þ����С��ϡ���
//    int size = 10000;  // �����С
//    double sparsity = 0.8;  // ϡ��ȣ�80% ��Ԫ��Ϊ�㣩
//
//    SparseMatrix matrix;
//
//    // �����������ϡ�����ĺ���
//    generateRandomSparseMatrix(matrix, size, size, sparsity);  // ������������
//
//    // ���� new_order ��һ���������������������������������Ӧ�ó�������
//    std::vector<int> new_order(size);
//    for (int i = 0; i < size; ++i) {
//        new_order[i] = i;  // ��ʼ��Ϊ˳�����������Ը��ݾ��������޸�
//    }
//
//    // 1. ʹ�� IOBLR �����������
//    reorderMatrixBasedOnIOBLR(matrix, new_order);
//
//    // 2. ȥ�������
//    removeZeroPadding(matrix);
//
//    std::vector<double> x(size, 1.0); // ������ʼ��Ϊȫ1
//
//    // ���ܲ���
//    std::cout << "Testing SpMV performance with IOBLR and zero-padding removal...\n";
//    performanceTest(matrix, x, true, true, false); // ʹ���Ż���ľ�����в���
//
//    return 0;
//}




// ��������ȡͶӰ�����ļ�
std::vector<std::vector<double>> readProjectionData(const std::string& filename) {
    std::vector<std::vector<double>> projections;
    std::ifstream infile(filename);
    std::string line;

    if (!infile.is_open()) {
        std::cerr << "�޷����ļ�: " << filename << std::endl;
        return projections;
    }

    while (std::getline(infile, line)) {
        if (line.empty()) continue;  // ��������
        std::vector<double> row;
        std::istringstream iss(line);
        double value;

        // ��ӡ��ȡ��ÿһ��
        std::cout << "Reading line: " << line << std::endl;

        while (iss >> value) {
            row.push_back(value);
        }

        projections.push_back(row);
    }

    infile.close();
    return projections;
}



// ����ϵͳ�����ʾ������
void constructSystemMatrix(SparseMatrix& systemMatrix, const std::vector<std::vector<double>>& projections) {
    systemMatrix.rows = projections.size();    // ��������ͶӰ���ݵ�����
    systemMatrix.cols = projections[0].size(); // ��������ͶӰ���ݵ�����
    systemMatrix.col_ptr.push_back(0);

    // ʾ�����򵥹���һ��ϡ�����
    for (int i = 0; i < systemMatrix.cols; ++i) {
        for (int j = 0; j < systemMatrix.rows; ++j) {
            if (projections[j][i] > 0) {  // ��������Ԫ����ӵ�ϡ�����
                systemMatrix.values.push_back(projections[j][i]);
                systemMatrix.row_idx.push_back(j);
            }
        }
        systemMatrix.col_ptr.push_back(systemMatrix.values.size());
    }
}



// ��ʼ��ͼ������
std::vector<double> initializeImage(int size) {
    return std::vector<double>(size, 0.0);  // ��ͼ��������ʼ��Ϊȫ��
}


void reconstructCTImage(SparseMatrix& systemMatrix, const std::vector<std::vector<double>>& projections, std::vector<double>& image, int iterations) {
    for (int iter = 0; iter < iterations; ++iter) {
        // ǰ��ͶӰ
        std::vector<double> forwardProjection = spmvCSCV_VxG(systemMatrix, image);

        // �������
        std::vector<double> error(projections.size());
        for (size_t i = 0; i < projections.size(); ++i) {
            for (size_t j = 0; j < projections[i].size(); ++j) {
                error[i] = projections[i][j] - forwardProjection[i];
            }
        }

        // ����ͶӰ
        std::vector<double> backProjection = spmvCSCV_VxG_transpose(systemMatrix, error);

        // ����ͼ��
        for (size_t i = 0; i < image.size(); ++i) {
            image[i] += backProjection[i];
        }

        std::cout << "Iteration " << iter + 1 << " complete." << std::endl;
    }
}

//int main() {
//    // ��ȡͶӰ����
//    std::vector<std::vector<double>> projections = readProjectionData("projections.txt");
//
//    // ����ϡ��ϵͳ����
//    SparseMatrix systemMatrix;
//    constructSystemMatrix(systemMatrix, projections);
//
//    // ��ʼ��ͼ������
//    int imageSize = projections.size();  // ����ͼ���С��ͶӰ����������ͬ
//    std::vector<double> image = initializeImage(imageSize);
//
//    // ���õ�������
//    int iterations = 10;
//
//    // ��ʼͼ���ؽ�
//    reconstructCTImage(systemMatrix, projections, image, iterations);
//
//    // ����ؽ���ͼ��
//    std::cout << "�ؽ���ɵ�ͼ�����ݣ�" << std::endl;
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
        std::cerr << "�޷����ļ� " << filename << " ����д�롣" << std::endl;
    }
}

int main() {
    // �ؽ���ɺ󱣴�ͼ������
   // reconstructCTImage(systemMatrix, projections, image, 10);
    saveImageToFile(image, "reconstructed_image.txt");
    return 0;
}

//int main() {
//    // ָ��ͶӰ�����ļ���·��
//    std::string filename = "projections.txt";
//
//    // ��ȡͶӰ����
//    std::vector<std::vector<double>> projections = readProjectionData(filename);
//
//    // �����ȡ��ͶӰ���ݽ��м��
//    if (!projections.empty()) {
//        for (size_t i = 0; i < std::min<size_t>(5, projections.size()); ++i) {
//            for (size_t j = 0; j < std::min<size_t>(5, projections[i].size()); ++j) {
//                std::cout << projections[i][j] << " ";
//            }
//            std::cout << std::endl;
//        }
//    }
//    else {
//        std::cout << "û�ж�ȡ���κ�ͶӰ���ݡ�" << std::endl;
//    }
//
//    return 0;
//}