import numpy as np
import time
import psutil
from joblib import Parallel, delayed

# 设置较小的矩阵规模
matrix_size = 10000  # 改为 100x100
non_zero_elements = 1000000  # 设置适量的非零元素数量
matrix = np.full((matrix_size, matrix_size), '', dtype='<U2')

# 随机填充非零元素
np.random.seed(0)
for _ in range(non_zero_elements):
    row = np.random.randint(0, matrix_size)
    col = np.random.randint(0, matrix_size)
    matrix[row, col] = chr(65 + np.random.randint(0, 26))  # 用A-Z字符表示非零元素

# 初始化b8c编码的存储
b8c_encoded = []

# 执行b8c编码
for row_idx in range(0, matrix_size, 4):  # 每4行处理一次
    for col_idx in range(0, matrix_size, 8):  # 每8列处理一次
        sub_matrix = matrix[row_idx:row_idx + 4, col_idx:col_idx + 8]
        non_zero_positions = np.argwhere(sub_matrix != '')

        if non_zero_positions.size > 0:
            row_base = row_idx
            col_base = col_idx
            row_deltas = non_zero_positions[:, 0]
            col_deltas = non_zero_positions[:, 1]

            b8c_encoded.append({
                'RowBase': row_base,
                'ColBase': col_base,
                'Values': [sub_matrix[tuple(pos)] for pos in non_zero_positions],
                'RowDelta': row_deltas.tolist(),
                'ColDelta': col_deltas.tolist()
            })
        # 添加调试信息
        #print(f"Processing super-row starting at row {row_idx}, column {col_idx}")

# 输出b8c编码结果
x = np.array([i + 1 for i in range(matrix_size)])  # 构建100维输入向量
y = np.zeros(matrix_size)  # 初始化输出向量 y

# 定义一个函数来处理每个超级行的SpMV计算
def spmv_b8c(encoded_row, x, y):
    row_base = encoded_row['RowBase']
    col_base = encoded_row['ColBase']
    values = encoded_row['Values']
    row_deltas = encoded_row['RowDelta']
    col_deltas = encoded_row['ColDelta']

    for i in range(len(values)):
        row_index = row_base + row_deltas[i]
        col_index = col_base + col_deltas[i]
        # 确保索引不超出范围
        if 0 <= row_index < len(y) and 0 <= col_index < len(x):
            #y[row_index] += x[col_index]
            #计算贡献值
            contribution = x[col_index] * (ord(values[i]) - 65 + 1)
            #y[row_index] += x[col_index] * (ord(values[i])-65+1)
            y[row_index] += contribution 
            print(f"y[{row_index}] updated with x[{col_index}] * {ord(values[i]) - 65 + 1} = {contribution}")
        else:
            print(f"Warning: Index out of bounds. row_index: {row_index}, col_index: {col_index}")

# 并行执行 SpMV 运算
start_time = time.time()
Parallel(n_jobs=-1)(delayed(spmv_b8c)(encoded_row, x, y) for encoded_row in b8c_encoded)
end_time = time.time()

print("输出向量 y:", y)
print(f"b8c 并行执行时间: {end_time - start_time:.6f} 秒")


# 获取每个核心的利用率
cpu_percentages = psutil.cpu_percent(interval=1, percpu=True)
print(cpu_percentages)  # 输出每个核心的利用率

# 获取当前进程的内存使用情况
process = psutil.Process()
mem_info = process.memory_info()
print(f"RSS: {mem_info.rss / (1024 ** 2)} MB")  # 输出实际使用的内存 (Resident Set Size)