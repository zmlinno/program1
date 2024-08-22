import numpy as np
import time
import psutil
from joblib import Parallel, delayed

# 设置矩阵规模
matrix_size = 10000
non_zero_elements = 10000
matrix = np.full((matrix_size, matrix_size), '', dtype='<U2')

# 随机填充非零元素
np.random.seed(0)
for _ in range(non_zero_elements):
    row = np.random.randint(0, matrix_size)
    col = np.random.randint(0, matrix_size)
    matrix[row, col] = chr(65 + np.random.randint(0, 26))  # 用A-Z字符表示非零元素

    #创建输入向量x
    x = np.array([i + 1 for i in range(matrix_size)])  # 构建一个长度为 matrix_size 的向量

# 生成CSR编码
def create_csr(matrix):
    nrows, ncols = matrix.shape
    values = []
    col_indices = []
    row_ptr = [0]  # 第一个元素总是0
    
    for i in range(nrows):
        for j in range(ncols):
            if matrix[i, j] != '':
                values.append(matrix[i, j])
                col_indices.append(j)
        row_ptr.append(len(values))
    
    return np.array(values), np.array(col_indices), np.array(row_ptr)

values, col_indices, row_ptr = create_csr(matrix)

# 定义CSR格式的SpMV运算
def spmv_csr(i, values, col_indices, row_ptr, x, y):
    row_start = row_ptr[i]
    row_end = row_ptr[i + 1]
    for j in range(row_start, row_end):
        y[i] += x[col_indices[j]]

# 初始化输出向量 y_csr
y_csr = np.zeros(matrix_size)

# 并行执行CSR格式的SpMV运算
start_time_csr = time.time()
Parallel(n_jobs=-1)(delayed(spmv_csr)(i, values, col_indices, row_ptr, x, y_csr) for i in range(matrix_size))
end_time_csr = time.time()

print("CSR 输出向量 y:", y_csr)
print(f"CSR 并行执行时间: {end_time_csr - start_time_csr:.6f} 秒")

# 监控并输出每个核心的利用率
cpu_percentages = psutil.cpu_percent(interval=1, percpu=True)
print(cpu_percentages)  # 输出每个核心的利用率

# 获取当前进程的内存使用情况
process = psutil.Process()
mem_info = process.memory_info()
print(f"RSS: {mem_info.rss / (1024 ** 2)} MB")  # 输出实际使用的内存 (Resident Set Size)