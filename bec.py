import numpy as np
import time
repeats = 100
matrix = np.full((16,16),'',dtype='<U2')


matrix[0, 0] = 'a'
matrix[0, 1] = 'b'
matrix[1, 1] = 'f'
matrix[3, 0] = 'h'
matrix[3, 1] = 'j'
matrix[1, 7] = 'i'

#print(matrix)

#初始化b8c编码的存储
b8c_encoded = []

#执行b8c编码
for row_idx in range(0,16,4):
    for col_idx in range(0,16,8):

        #提取子矩阵
        sub_matrix = matrix[row_idx:row_idx+4,col_idx:col_idx+8]

        #找到非0元素
        non_zero_positions = np.argwhere(sub_matrix != '')

        #如果有非0元素，进行编码
        if non_zero_positions.size>0:

            #RowBase和ColBase
            row_base = row_idx
            col_base = col_idx

            #计算RowDelta和ColDelta
            row_deltas = non_zero_positions[:,0]
            col_deltas = non_zero_positions[:,1]


            #存储超级行
            b8c_encoded.append({
                'RowBase':row_base,
                'ColBase':col_base,
                'Values':[sub_matrix[tuple(pos)]for pos in non_zero_positions],
                'RowDelta':row_deltas.tolist(),
                'ColDelta':col_deltas.tolist()
            })

            #输出b8c编码结果



            x = np.array([1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16])

            #初始化输出向量y(长度为16)
            y = np.zeros(16)

            #进行SpMV运算
            for encoded_row in b8c_encoded:
                row_base = encoded_row['RowBase']
                col_base = encoded_row['ColBase']
                values = encoded_row['Values']
                row_deltas = encoded_row['RowDelta']
                col_deltas = encoded_row['ColDelta']

                for i in range(len(values)):
                    row_index = row_base + row_deltas[i]
                    col_index = col_base + col_deltas[i] 
                    y[row_index]+= x[col_index] 

                    #输出结果向量y
                    print("输出向量:",y)

                    start_time = time.time()
           


                    end_time = time.time()
                    print(f"b8c 执行时间: {end_time - start_time:.6f}秒")      

    

