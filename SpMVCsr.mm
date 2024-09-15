//
//  NSObject+SpMVCsr.m
//  SpMVTest
//
//  Created by 张木林 on 9/11/24.
//


//#import<Foundation/Foundation.h>
//#ifdef __APPLE__
//#import<Metal/Metal.h>
//
//#include<iostream>
//#include<vector>
//#include<chrono>
//#endif
//using namespace std;
//using namespace std::chrono;
//// 定义 CSR 矩阵的结构
//struct CSRMatrix {
//    int rows;
//    std::vector<int> rowPointers;
//    std::vector<int> colIndices;
//    std::vector<float> values;
//};
//
//// 定义向量
//struct Vector {
//    int size;
//    std::vector<float> data;
//};
//
//// Metal 相关类
//class MetalCSRMultiplication {
//private:
//    id<MTLDevice> device;
//    id<MTLLibrary> library;
//    id<MTLCommandQueue> commandQueue;
//    id<MTLComputePipelineState> pipelineState;
//
//public:
//    MetalCSRMultiplication() {
//        // 创建 Metal 设备
//        device = MTLCreateSystemDefaultDevice();
//        if (!device) {
//            cerr << "Metal is not supported on this system." << endl;
//            exit(1);
//        }
//
//        // 加载 Metal Shader
//        library = [device newDefaultLibrary];
//        if (!library) {
//            cerr << "Failed to create Metal library." << endl;
//            exit(1);
//        }
//
//        id<MTLFunction> function = [library newFunctionWithName:@"csrMatrixVectorMultiply"];
//        if (!function) {
//            cerr << "Failed to load Metal function." << endl;
//            exit(1);
//        }
//
//        pipelineState = [device newComputePipelineStateWithFunction:function error:nil];
//        commandQueue = [device newCommandQueue];
//    }
//
//    // 计算 CSR 矩阵和向量乘法
//    void multiplyMatrix(const CSRMatrix& matrix, const Vector& vector, std::vector<float>& result) {
//        // 创建 Metal 缓冲区
//        id<MTLBuffer> rowPointersBuffer = [device newBufferWithBytes:matrix.rowPointers.data()
//                                                              length:(matrix.rows + 1) * sizeof(int)
//                                                             options:MTLResourceStorageModeShared];
//
//        id<MTLBuffer> colIndicesBuffer = [device newBufferWithBytes:matrix.colIndices.data()
//                                                             length:matrix.colIndices.size() * sizeof(int)
//                                                            options:MTLResourceStorageModeShared];
//
//        id<MTLBuffer> valuesBuffer = [device newBufferWithBytes:matrix.values.data()
//                                                         length:matrix.values.size() * sizeof(float)
//                                                        options:MTLResourceStorageModeShared];
//
//        id<MTLBuffer> vectorBuffer = [device newBufferWithBytes:vector.data.data()
//                                                         length:vector.data.size() * sizeof(float)
//                                                        options:MTLResourceStorageModeShared];
//
//        id<MTLBuffer> resultBuffer = [device newBufferWithLength:result.size() * sizeof(float)
//                                                         options:MTLResourceStorageModeShared];
//
//        // 创建命令缓冲区和计算命令编码器
//        id<MTLCommandBuffer> commandBuffer = [commandQueue commandBuffer];
//        id<MTLComputeCommandEncoder> computeEncoder = [commandBuffer computeCommandEncoder];
//
//        [computeEncoder setComputePipelineState:pipelineState];
//        [computeEncoder setBuffer:rowPointersBuffer offset:0 atIndex:0];
//        [computeEncoder setBuffer:colIndicesBuffer offset:0 atIndex:1];
//        [computeEncoder setBuffer:valuesBuffer offset:0 atIndex:2];
//        [computeEncoder setBuffer:vectorBuffer offset:0 atIndex:3];
//        [computeEncoder setBuffer:resultBuffer offset:0 atIndex:4];
//
//        // 配置线程
//        MTLSize gridSize = MTLSizeMake(matrix.rows, 1, 1);
//        MTLSize threadGroupSize = MTLSizeMake(1, 1, 1);
//        [computeEncoder dispatchThreads:gridSize threadsPerThreadgroup:threadGroupSize];
//
//        // 提交命令
//        [computeEncoder endEncoding];
//        [commandBuffer commit];
//        [commandBuffer waitUntilCompleted];
//
//        // 获取结果
//        float* resultPointer = (float*)[resultBuffer contents];
//        for (int i = 0; i < matrix.rows; i++) {
//            result[i] = resultPointer[i];
//        }
//    }
//};
//
////使用// 使用 MTLCopyAllDevices 获取系统上的 Metal 设备
//id<MTLDevice>getMetalDevice()
//{
//    NSArray<id<MTLDevice>>* devices = MTLCopyAllDevices();
//    if(devices.count == 0)
//    {
//        std::cerr<<"No Metal devices found."<<std::endl;
//        return nil;
//    }
//    return devices[0];
//}
// 测试代码
//int main() {
//    id<MTLDevice>device = getMetalDevice();
//    if(!device)
//    {
//        std::cerr<<"Metal is not supported on this system."<<std::endl;
//        return 1;
//    }
//    // 创建 Metal 计算对象
//    MetalCSRMultiplication csrMultiplier;
//
//    // CSR 矩阵数据
//    CSRMatrix matrix;
//    matrix.rows = 3;
//    matrix.rowPointers = {0, 1, 3, 4}; // 指向每行开始的索引
//    matrix.colIndices = {0, 1, 2, 3};  // 列索引
//    matrix.values = {10, 20, 30, 40};  // 非零值
//
//    // 向量数据
//    Vector vector;
//    vector.size = 4;
//    vector.data = {1, 2, 3, 4};
//
//    // 结果数组
//    std::vector<float> result(3, 0.0);
//
//    // 调用 Metal 进行矩阵-向量乘法
//    csrMultiplier.multiplyMatrix(matrix, vector, result);
//
//    // 打印结果
//    cout << "Matrix-Vector Multiplication Result:" << endl;
//    for (int i = 0; i < 3; i++) {
//        cout << "result[" << i << "] = " << result[i] << endl;
//    }
//
//    return 0;
//}

