
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>



//// CUDA内核：稀疏矩阵加法（COO格式）
//__global__ void sparseMatrixAddKernel(int* row_a, int* col_a, int* val_a, int nonZeroA,
//    int* row_b, int* col_b, int* val_b, int nonZeroB,
//    int* row_c, int* col_c, int* val_c, int* nonZeroC) {
//    int i = threadIdx.x; // 线程索引
//
//    // 检查当前线程是否在合法范围内
//    if (i < nonZeroA) {
//        for (int j = 0; j < nonZeroB; j++) {
//            if (row_a[i] == row_b[j] && col_a[i] == col_b[j]) {
//                // 位置相同，执行加法
//                val_c[i] = val_a[i] + val_b[j];
//                row_c[i] = row_a[i];
//                col_c[i] = col_a[i];
//                atomicAdd(nonZeroC, 1); // 使用原子操作计数非零元素个数
//            }
//        }
//    }
//}
//
////运行COO格式的稀疏矩阵加法
//void runCOO()
//{
//    //模拟1000*1000的稀疏矩阵，每个矩阵有5000个非0元素
//    const int nonZeroA = 5000, nonZeroB = 5000;
//    int* row_a = (int*)malloc(nonZeroA * sizeof(int));
//    int* col_a = (int*)malloc(nonZeroA * sizeof(int));
//    int* val_a = (int*)malloc(nonZeroA * sizeof(int));
//
//    int* row_b = (int*)malloc(nonZeroB * sizeof(int));
//    int* col_b = (int*)malloc(nonZeroB * sizeof(int));
//    int* val_b = (int*)malloc(nonZeroB * sizeof(int));
//
//    int* row_c = (int*)malloc(nonZeroA * sizeof(int));
//    int* col_c = (int*)malloc(nonZeroA * sizeof(int));
//    int* val_c = (int*)malloc(nonZeroA * sizeof(int));
//
//    int nonZeroC = 0;
//
//    //初始化A和B稀疏矩阵的元素
//    for (int i = 0; i < nonZeroA; i++)
//    {
//        row_a[i] = i % 1000;
//        col_a[i] = i % 1000;
//        val_a[i] = i % 1000;
//    }
//
//    for (int i = 0; i < nonZeroB; i++)
//    {
//        row_b[i] = i % 1000;
//        col_b[i] = i % 1000;
//        val_b[i] = i % 1000;
//    }
//
//    cudaError_t cudaStatus = addWithCudaCOO(row_c, col_c, row_a, col_a, val_a, nonZeroA,
//        row_b, col_b, val_b, nonZeroB, nonZeroC);
//
//    if (cudaStatus != cudaSuccess)
//    {
//        fprintf(stderr, "addWithCuda failed");
//        return;
//    }
//}
//
//// 主机函数：调用CUDA内核
//cudaError_t addWithCuda(int* row_c, int* col_c, int* val_c,
//    const int* row_a, const int* col_a, const int* val_a, int nonZeroA,
//    const int* row_b, const int* col_b, const int* val_b, int nonZeroB, int* nonZeroC) {
//    int* dev_row_a = 0, * dev_col_a = 0, * dev_val_a = 0;
//    int* dev_row_b = 0, * dev_col_b = 0, * dev_val_b = 0;
//    int* dev_row_c = 0, * dev_col_c = 0, * dev_val_c = 0;
//    int* dev_nonZeroC = 0;
//    cudaError_t cudaStatus;
//
//    // 选择CUDA设备
//    cudaStatus = cudaSetDevice(0);
//    if (cudaStatus != cudaSuccess) {
//        fprintf(stderr, "cudaSetDevice failed! Do you have a CUDA-capable GPU installed?\n");
//        goto Error;
//    }
//
//    // 分配GPU内存
//    cudaMalloc((void**)&dev_row_a, nonZeroA * sizeof(int));
//    cudaMalloc((void**)&dev_col_a, nonZeroA * sizeof(int));
//    cudaMalloc((void**)&dev_val_a, nonZeroA * sizeof(int));
//
//    cudaMalloc((void**)&dev_row_b, nonZeroB * sizeof(int));
//    cudaMalloc((void**)&dev_col_b, nonZeroB * sizeof(int));
//    cudaMalloc((void**)&dev_val_b, nonZeroB * sizeof(int));
//
//    cudaMalloc((void**)&dev_row_c, nonZeroA * sizeof(int)); // 假设C最多有nonZeroA个非零元素
//    cudaMalloc((void**)&dev_col_c, nonZeroA * sizeof(int));
//    cudaMalloc((void**)&dev_val_c, nonZeroA * sizeof(int));
//    cudaMalloc((void**)&dev_nonZeroC, sizeof(int));
//
//    // 将稀疏矩阵A和B的数据从主机复制到设备
//    cudaMemcpy(dev_row_a, row_a, nonZeroA * sizeof(int), cudaMemcpyHostToDevice);
//    cudaMemcpy(dev_col_a, col_a, nonZeroA * sizeof(int), cudaMemcpyHostToDevice);
//    cudaMemcpy(dev_val_a, val_a, nonZeroA * sizeof(int), cudaMemcpyHostToDevice);
//
//    cudaMemcpy(dev_row_b, row_b, nonZeroB * sizeof(int), cudaMemcpyHostToDevice);
//    cudaMemcpy(dev_col_b, col_b, nonZeroB * sizeof(int), cudaMemcpyHostToDevice);
//    cudaMemcpy(dev_val_b, val_b, nonZeroB * sizeof(int), cudaMemcpyHostToDevice);
//
//    // 初始化非零元素计数器
//    cudaMemset(dev_nonZeroC, 0, sizeof(int));
//
//    // 启动CUDA内核
//    sparseMatrixAddKernel << <1, nonZeroA >> > (dev_row_a, dev_col_a, dev_val_a, nonZeroA,
//        dev_row_b, dev_col_b, dev_val_b, nonZeroB,
//        dev_row_c, dev_col_c, dev_val_c, dev_nonZeroC);
//
//    // 检查内核启动的错误
//    cudaStatus = cudaGetLastError();
//    if (cudaStatus != cudaSuccess) {
//        fprintf(stderr, "addKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
//        goto Error;
//    }
//
//    // 同步设备
//    cudaStatus = cudaDeviceSynchronize();
//    if (cudaStatus != cudaSuccess) {
//        fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching kernel!\n", cudaStatus);
//        goto Error;
//    }
//
//    // 将结果从设备复制回主机
//    cudaMemcpy(row_c, dev_row_c, nonZeroA * sizeof(int), cudaMemcpyDeviceToHost);
//    cudaMemcpy(col_c, dev_col_c, nonZeroA * sizeof(int), cudaMemcpyDeviceToHost);
//    cudaMemcpy(val_c, dev_val_c, nonZeroA * sizeof(int), cudaMemcpyDeviceToHost);
//    cudaMemcpy(nonZeroC, dev_nonZeroC, sizeof(int), cudaMemcpyDeviceToHost);
//
//Error:
//    cudaFree(dev_row_a);
//    cudaFree(dev_col_a);
//    cudaFree(dev_val_a);
//    cudaFree(dev_row_b);
//    cudaFree(dev_col_b);
//    cudaFree(dev_val_b);
//    cudaFree(dev_row_c);
//    cudaFree(dev_col_c);
//    cudaFree(dev_val_c);
//    cudaFree(dev_nonZeroC);
//
//    return cudaStatus;
//}
//
//// 主函数
//int main() {
//    const int nonZeroA = 3, nonZeroB = 3;
//    int row_a[nonZeroA] = { 0, 1, 2 };
//    int col_a[nonZeroA] = { 0, 1, 2 };
//    int val_a[nonZeroA] = { 10, 20, 30 };
//
//    int row_b[nonZeroB] = { 0, 1, 2 };
//    int col_b[nonZeroB] = { 0, 1, 2 };
//    int val_b[nonZeroB] = { 5, 15, 25 };
//
//    int row_c[nonZeroA], col_c[nonZeroA], val_c[nonZeroA];
//    int nonZeroC = 0;
//
//    // 调用CUDA加法函数
//    cudaError_t cudaStatus = addWithCuda(row_c, col_c, val_c, row_a, col_a, val_a, nonZeroA,
//        row_b, col_b, val_b, nonZeroB, &nonZeroC);
//
//    if (cudaStatus != cudaSuccess) {
//        fprintf(stderr, "addWithCuda failed!");
//        return 1;
//    }
//
//    // 打印结果矩阵C
//    printf("Resulting sparse matrix C (in COO format):\n");
//    for (int i = 0; i < nonZeroC; i++) {
//        printf("C(%d, %d) = %d\n", row_c[i], col_c[i], val_c[i]);
//    }
//
//    return 0;
//}


// CUDA内核：稀疏矩阵加法（CSR格式）
__global__ void sparseMatrixAddKernelCSR(int* row_ptr_a, int* col_a, int* val_a, int nonZeroA,
    int* row_ptr_b, int* col_b, int* val_b, int nonZeroB,
    int* row_ptr_c, int* col_c, int* val_c, int* nonZeroC) {
    int row = blockIdx.x * blockDim.x + threadIdx.x;

    // 确保线程在合法的范围内
    if (row < 1000) {
        int startA = row_ptr_a[row];
        int endA = row_ptr_a[row + 1];

        int startB = row_ptr_b[row];
        int endB = row_ptr_b[row + 1];

        int idxC = row_ptr_c[row];

        // 遍历A和B的非零元素并执行加法
        while (startA < endA && startB < endB) {
            if (col_a[startA] == col_b[startB]) {
                val_c[idxC] = val_a[startA] + val_b[startB];
                col_c[idxC] = col_a[startA];
                startA++;
                startB++;
            }
            else if (col_a[startA] < col_b[startB]) {
                val_c[idxC] = val_a[startA];
                col_c[idxC] = col_a[startA];
                startA++;
            }
            else {
                val_c[idxC] = val_b[startB];
                col_c[idxC] = col_b[startB];
                startB++;
            }
            idxC++;
        }

        // 将剩余的非零元素加到C中
        while (startA < endA) {
            val_c[idxC] = val_a[startA];
            col_c[idxC] = col_a[startA];
            startA++;
            idxC++;
        }

        while (startB < endB) {
            val_c[idxC] = val_b[startB];
            col_c[idxC] = col_b[startB];
            startB++;
            idxC++;
        }

        row_ptr_c[row + 1] = idxC; // 更新行指针
    }
}

// 主机函数：调用CUDA内核（CSR格式）
cudaError_t addWithCudaCSR(int* row_ptr_c, int* col_c, int* val_c,
    const int* row_ptr_a, const int* col_a, const int* val_a, int nonZeroA,
    const int* row_ptr_b, const int* col_b, const int* val_b, int nonZeroB, int* nonZeroC) {
    int* dev_row_ptr_a = 0, * dev_col_a = 0, * dev_val_a = 0;
    int* dev_row_ptr_b = 0, * dev_col_b = 0, * dev_val_b = 0;
    int* dev_row_ptr_c = 0, * dev_col_c = 0, * dev_val_c = 0;
    int* dev_nonZeroC = 0;
    cudaError_t cudaStatus;

    // 分配GPU内存
    cudaMalloc((void**)&dev_row_ptr_a, 1001 * sizeof(int));
    cudaMalloc((void**)&dev_col_a, nonZeroA * sizeof(int));
    cudaMalloc((void**)&dev_val_a, nonZeroA * sizeof(int));

    cudaMalloc((void**)&dev_row_ptr_b, 1001 * sizeof(int));
    cudaMalloc((void**)&dev_col_b, nonZeroB * sizeof(int));
    cudaMalloc((void**)&dev_val_b, nonZeroB * sizeof(int));

    cudaMalloc((void**)&dev_row_ptr_c, 1001 * sizeof(int));
    cudaMalloc((void**)&dev_col_c, nonZeroA * sizeof(int));
    cudaMalloc((void**)&dev_val_c, nonZeroA * sizeof(int));

    // 将稀疏矩阵A和B的数据从主机复制到设备
    cudaMemcpy(dev_row_ptr_a, row_ptr_a, 1001 * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(dev_col_a, col_a, nonZeroA * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(dev_val_a, val_a, nonZeroA * sizeof(int), cudaMemcpyHostToDevice);

    cudaMemcpy(dev_row_ptr_b, row_ptr_b, 1001 * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(dev_col_b, col_b, nonZeroB * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(dev_val_b, val_b, nonZeroB * sizeof(int), cudaMemcpyHostToDevice);

    // 启动CUDA内核
    int blocks = (1000 + BLOCK_SIZE - 1) / BLOCK_SIZE;
    sparseMatrixAddKernelCSR << <blocks, BLOCK_SIZE >> > (dev_row_ptr_a, dev_col_a, dev_val_a, nonZeroA,
        dev_row_ptr_b, dev_col_b, dev_val_b, nonZeroB,
        dev_row_ptr_c, dev_col_c, dev_val_c, dev_nonZeroC);

    // 检查内核启动的错误
    cudaStatus = cudaGetLastError();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "CSR kernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
        goto Error;
    }

    // 同步设备
    cudaStatus = cudaDeviceSynchronize();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching kernel!\n", cudaStatus);
        goto Error;
    }

    // 将结果从设备复制回主机
    cudaMemcpy(row_ptr_c, dev_row_ptr_c, 1001 * sizeof(int), cudaMemcpyDeviceToHost);
    cudaMemcpy(col_c, dev_col_c, nonZeroA * sizeof(int), cudaMemcpyDeviceToHost);
    cudaMemcpy(val_c, dev_val_c, nonZeroA * sizeof(int), cudaMemcpyDeviceToHost);

Error:
    cudaFree(dev_row_ptr_a);
    cudaFree(dev_col_a);
    cudaFree(dev_val_a);
    cudaFree(dev_row_ptr_b);
    cudaFree(dev_col_b);
    cudaFree(dev_val_b);
    cudaFree(dev_row_ptr_c);
    cudaFree(dev_col_c);
    cudaFree(dev_val_c);

    return cudaStatus;
}

// 主函数：运行CSR稀疏矩阵加法
void runCSR() {
    const int nonZeroA = 5000, nonZeroB = 5000;
    int row_ptr_a[1001] = { 0 }, row_ptr_b[1001] = { 0 }, row_ptr_c[1001] = { 0 };
    int* col_a = (int*)malloc(nonZeroA * sizeof(int));
    int* val_a = (int*)malloc(nonZeroA * sizeof(int));
    int* col_b = (int*)malloc(nonZeroB * sizeof(int));
    int* val_b = (int*)malloc(nonZeroB * sizeof(int));
    int* col_c = (int*)malloc(nonZeroA * sizeof(int));
    int* val_c = (int*)malloc(nonZeroA * sizeof(int));

    // 初始化A和B稀疏矩阵的元素
    for (int i = 0; i < nonZeroA; i++) {
        col_a[i] = i % 1000;
        val_a[i] = i + 1;
    }

    for (int i = 0; i < nonZeroB; i++) {
        col_b[i] = i % 1000;
        val_b[i] = i + 2;
    }

    // 调用CUDA加法函数
    cudaError_t cudaStatus = addWithCudaCSR(row_ptr_c, col_c, val_c, row_ptr_a, col_a, val_a, nonZeroA,
        row_ptr_b, col_b, val_b, nonZeroB, &nonZeroA);

    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "addWithCudaCSR failed!");
        return;
    }

    // 打印结果矩阵C
    printf("Resulting sparse matrix C (in CSR format):\n");
    for (int i = 0; i < row_ptr_c[1000]; i++) {
        printf("C(row %d) = %d\n", i, val_c[i]);
    }
}