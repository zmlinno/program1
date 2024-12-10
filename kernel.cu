
//#include "cuda_runtime.h"
//#include "device_launch_parameters.h"
//
//#include <stdio.h>
//
//cudaError_t addWithCuda(int *c, const int *a, const int *b, unsigned int size);
//
//__global__ void addKernel(int *c, const int *a, const int *b)
//{
//    int i = threadIdx.x;
//    c[i] = a[i] + b[i];
//}
//
//int main()
//{
//    const int arraySize = 5;
//    const int a[arraySize] = { 1, 2, 3, 4, 5 };
//    const int b[arraySize] = { 10, 20, 30, 40, 50 };
//    int c[arraySize] = { 0 };
//
//    // Add vectors in parallel.
//    cudaError_t cudaStatus = addWithCuda(c, a, b, arraySize);
//    if (cudaStatus != cudaSuccess) {
//        fprintf(stderr, "addWithCuda failed!");
//        return 1;
//    }
//
//    printf("{1,2,3,4,5} + {10,20,30,40,50} = {%d,%d,%d,%d,%d}\n",
//        c[0], c[1], c[2], c[3], c[4]);
//
//    // cudaDeviceReset must be called before exiting in order for profiling and
//    // tracing tools such as Nsight and Visual Profiler to show complete traces.
//    cudaStatus = cudaDeviceReset();
//    if (cudaStatus != cudaSuccess) {
//        fprintf(stderr, "cudaDeviceReset failed!");
//        return 1;
//    }
//
//    return 0;
//}
//
//// Helper function for using CUDA to add vectors in parallel.
//cudaError_t addWithCuda(int *c, const int *a, const int *b, unsigned int size)
//{
//    int *dev_a = 0;
//    int *dev_b = 0;
//    int *dev_c = 0;
//    cudaError_t cudaStatus;
//
//    // Choose which GPU to run on, change this on a multi-GPU system.
//    cudaStatus = cudaSetDevice(0);
//    if (cudaStatus != cudaSuccess) {
//        fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
//        goto Error;
//    }
//
//    // Allocate GPU buffers for three vectors (two input, one output)    .
//    cudaStatus = cudaMalloc((void**)&dev_c, size * sizeof(int));
//    if (cudaStatus != cudaSuccess) {
//        fprintf(stderr, "cudaMalloc failed!");
//        goto Error;
//    }
//
//    cudaStatus = cudaMalloc((void**)&dev_a, size * sizeof(int));
//    if (cudaStatus != cudaSuccess) {
//        fprintf(stderr, "cudaMalloc failed!");
//        goto Error;
//    }
//
//    cudaStatus = cudaMalloc((void**)&dev_b, size * sizeof(int));
//    if (cudaStatus != cudaSuccess) {
//        fprintf(stderr, "cudaMalloc failed!");
//        goto Error;
//    }
//
//    // Copy input vectors from host memory to GPU buffers.
//    cudaStatus = cudaMemcpy(dev_a, a, size * sizeof(int), cudaMemcpyHostToDevice);
//    if (cudaStatus != cudaSuccess) {
//        fprintf(stderr, "cudaMemcpy failed!");
//        goto Error;
//    }
//
//    cudaStatus = cudaMemcpy(dev_b, b, size * sizeof(int), cudaMemcpyHostToDevice);
//    if (cudaStatus != cudaSuccess) {
//        fprintf(stderr, "cudaMemcpy failed!");
//        goto Error;
//    }
//
//    // Launch a kernel on the GPU with one thread for each element.
//    addKernel<<<1, size>>>(dev_c, dev_a, dev_b);
//
//    // Check for any errors launching the kernel
//    cudaStatus = cudaGetLastError();
//    if (cudaStatus != cudaSuccess) {
//        fprintf(stderr, "addKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
//        goto Error;
//    }
//    
//    // cudaDeviceSynchronize waits for the kernel to finish, and returns
//    // any errors encountered during the launch.
//    cudaStatus = cudaDeviceSynchronize();
//    if (cudaStatus != cudaSuccess) {
//        fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching addKernel!\n", cudaStatus);
//        goto Error;
//    }
//
//    // Copy output vector from GPU buffer to host memory.
//    cudaStatus = cudaMemcpy(c, dev_c, size * sizeof(int), cudaMemcpyDeviceToHost);
//    if (cudaStatus != cudaSuccess) {
//        fprintf(stderr, "cudaMemcpy failed!");
//        goto Error;
//    }
//
//Error:
//    cudaFree(dev_c);
//    cudaFree(dev_a);
//    cudaFree(dev_b);
//    
//    return cudaStatus;
//}


//#include<cuda_runtime.h>
//#include <cusparse.h>
//#include<cublas_v2.h>
//#include<iostream>

//int main() {
//    // 初始化 CUSPARSE
//    cusparseHandle_t cusparseHandle;
//    cusparseCreate(&cusparseHandle);
//
//    // 定义稀疏矩阵 (CSR 格式)
//    int rows = 3; // 矩阵的行数
//    int cols = 3; // 矩阵的列数
//    int nnz = 4;  // 非零元素的数量
//
//    // CSR 格式的数据
//    float h_values[] = { 1.0f, 2.0f, 3.0f, 4.0f };        // 非零元素的值
//    int h_columns[] = { 0, 1, 1, 2 };                     // 非零元素对应的列索引
//    int h_rowPointers[] = { 0, 1, 3, 4 };                 // 每行非零元素的起始索引
//
//    // 向量
//    float h_x[] = { 1.0f, 2.0f, 3.0f };                   // 输入向量
//    float h_y[3] = { 0.0f, 0.0f, 0.0f };                  // 输出向量初始化为 0
//
//    // 在设备上分配内存
//    float* d_values, * d_x, * d_y;
//    int* d_columns, * d_rowPointers;
//
//    cudaMalloc((void**)&d_values, nnz * sizeof(float));
//    cudaMalloc((void**)&d_columns, nnz * sizeof(int));
//    cudaMalloc((void**)&d_rowPointers, (rows + 1) * sizeof(int));
//    cudaMalloc((void**)&d_x, cols * sizeof(float));
//    cudaMalloc((void**)&d_y, rows * sizeof(float));
//
//    // 将数据从主机复制到设备
//    cudaMemcpy(d_values, h_values, nnz * sizeof(float), cudaMemcpyHostToDevice);
//    cudaMemcpy(d_columns, h_columns, nnz * sizeof(int), cudaMemcpyHostToDevice);
//    cudaMemcpy(d_rowPointers, h_rowPointers, (rows + 1) * sizeof(int), cudaMemcpyHostToDevice);
//    cudaMemcpy(d_x, h_x, cols * sizeof(float), cudaMemcpyHostToDevice);
//
//    // 定义 alpha 和 beta
//    float alpha = 1.0f;
//    float beta = 0.0f;
//
//    // 创建稀疏矩阵描述符 (cusparseSpMatDescr_t)
//    cusparseSpMatDescr_t matA;
//    cusparseCreateCsr(&matA, rows, cols, nnz,
//        d_rowPointers, d_columns, d_values,
//        CUSPARSE_INDEX_32I, CUSPARSE_INDEX_32I,
//        CUSPARSE_INDEX_BASE_ZERO, CUDA_R_32F);
//
//    // 创建向量描述符 (cusparseDnVecDescr_t)
//    cusparseDnVecDescr_t vecX, vecY;
//    cusparseCreateDnVec(&vecX, cols, d_x, CUDA_R_32F);
//    cusparseCreateDnVec(&vecY, rows, d_y, CUDA_R_32F);
//
//    // 分配临时缓冲区
//    size_t bufferSize = 0;
//    void* dBuffer = nullptr;
//    cusparseSpMV_bufferSize(cusparseHandle, CUSPARSE_OPERATION_NON_TRANSPOSE,
//        &alpha, matA, vecX, &beta, vecY,
//        CUDA_R_32F, CUSPARSE_SPMV_ALG_DEFAULT, &bufferSize);
//    cudaMalloc(&dBuffer, bufferSize);
//
//    // 执行稀疏矩阵-向量乘法 (SpMV)
//    cusparseSpMV(cusparseHandle, CUSPARSE_OPERATION_NON_TRANSPOSE,
//        &alpha, matA, vecX, &beta, vecY,
//        CUDA_R_32F, CUSPARSE_SPMV_ALG_DEFAULT, dBuffer);
//
//    // 将结果从设备复制到主机
//    cudaMemcpy(h_y, d_y, rows * sizeof(float), cudaMemcpyDeviceToHost);
//
//    // 打印结果
//    std::cout << "Result vector y:" << std::endl;
//    for (int i = 0; i < rows; i++) {
//        std::cout << h_y[i] << " ";
//    }
//    std::cout << std::endl;
//
//    // 释放资源
//    cusparseDestroySpMat(matA);
//    cusparseDestroyDnVec(vecX);
//    cusparseDestroyDnVec(vecY);
//    cudaFree(dBuffer);
//    cudaFree(d_values);
//    cudaFree(d_columns);
//    cudaFree(d_rowPointers);
//    cudaFree(d_x);
//    cudaFree(d_y);
//    cusparseDestroy(cusparseHandle);
//
//    return 0;
//}





#include <cuda_runtime.h>
#include <cusparse.h>
#include <iostream>

int main() {
    cusparseHandle_t handle;
    cusparseStatus_t status = cusparseCreate(&handle);

    if (status != CUSPARSE_STATUS_SUCCESS) {
        std::cerr << "CUSPARSE 初始化失败！" << std::endl;
        return -1;
    }

    std::cout << "CUSPARSE 初始化成功！" << std::endl;
    cusparseDestroy(handle);
    return 0;
}
