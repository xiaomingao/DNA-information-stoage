# rs decode 之后，还原到encode时候的序列
import random,sys
input_file = sys.argv[1]
f_num = sys.argv[2] #编码encode结束后得到的所有文件数
f_num = int(f_num)
file_index = 0

def int_to_bytes(number, length):
    # all the data except the first byte
    for i in range(int(length / 8)):
        if i == 0:
            base = chr(int(number/pow(256, int(length / 8) - 1))).encode('latin1')
            number = number - pow(256, int(length / 8) - 1) * int(number / pow(256, int(length / 8) - 1))
        elif i == int(length / 8) - 1:
            base = base + chr(number % 256).encode('latin1')
        else:
            base = base + chr(int(number / pow(256, int(length / 8) - 1 - i))).encode('latin1')
            number = number - pow(256, int(length / 8) - 1 - i) * int(number / pow(256, int(length / 8) - 1- i))
    return base

# read random bits files, 1代表4个碱基相连， 2代表完全规避序列
# random_1 = 'random_bits_1.csv'
random_2 = 'random_bits_2.csv'

# random_bits_1 = []
random_bits_2 = []
'''
f = open(random_1, 'r')
for line in f:
    random_bits_1.append(int(line.rstrip()))
f.close()
'''
f = open(random_2, 'r')
for line in f:
    random_bits_2.append(int(line.rstrip()))
f.close()
# print(random_bits_2)
# print(len(random_bits_2))

cnt = 0

for i in range(f_num):
    s = input_file + str(i)
    with open(s, 'rb') as f:
        temp_int = int.from_bytes(f.read(1), byteorder='big', signed=False)
        # print(temp_int)
    with open(s, 'rb') as f:
        temp_bytes = f.read(36)
    if temp_int == 254:
        # print('**************')
        cnt = cnt + 1
        with open(s, 'wb') as f:
            f.write(temp_bytes[1:4] + chr(0).encode('latin1') + temp_bytes[4:36])
    else:
        temp_2 = temp_int - 1
        temp_2_int = int.from_bytes(temp_bytes[1:36], byteorder='big', signed=False)
        try:
            temp_res_int = temp_2_int ^ random_bits_2[temp_2] ##try?
        except:
            continue
        else:
            temp_2_res = int_to_bytes(temp_res_int, 35 * 8)
        '''
        temp_1_int = int.from_bytes(temp_2_res[0:3], byteorder='big', signed=False)
        if temp_1 > -1:
            temp_1_int = temp_1_int ^ random_bits_1[temp_1]
        temp_1_res = int_to_bytes(temp_1_int, 3 * 8)
        '''
        with open(s, 'wb') as f:
                f.write(temp_2_res[0:3] + chr(0).encode('latin1') + temp_2_res[3:35])

print(cnt)