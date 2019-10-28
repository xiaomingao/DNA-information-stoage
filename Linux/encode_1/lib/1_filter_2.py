# 若是前8位有4个相同碱基相连，即全为0的情况，我们将其值置为254，可以避免4个碱基相连,因为随机序列的序号为1-253，不可能出现254，同时254避免了4个相同碱基相连

import random,os,sys
input_file = sys.argv[1]
f_num = sys.argv[2]
f_num = int(f_num)
file_index = 0
while file_index < f_num:
    # read the data of the first three bytes
    s = input_file + str(file_index)
    with open(s, 'rb') as f:
        temp = f.read(1)
    if temp[0] == 0:
        with open(s, 'rb') as f:
            temp = f.read(36)
        with open(s, 'wb') as f:
            temp = f.write(chr(254).encode('latin1') + temp[1:])
    file_index = file_index + 1
print(file_index)

