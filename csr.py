import numpy as np
from scipy.sparse import csr_matrix
import time
repeats = 100

#定义一个与b8c编码相同的稀疏矩阵

data = [1,2,3,4,5,6]
rows = [0,0,1,1,3,3]
cols = [0,1,1,7,0,1]
matrix_csr = csr_matrix((data,(rows,cols)),shape = (16,16))


#输入向量x
x = np.array([1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16])


#执行SvMP计算
y_csr = matrix_csr.dot(x)

print("CSR 输出向量 y:",y_csr)

start_time = time.time()
for _ in range(repeats):
    y_csr = matrix_csr.dot(x)
avg_time_csr = (time.time()-start_time)/repeats
end_time = time.time()
print(f"CSR 执行时间: {end_time - start_time:.10f}秒")
