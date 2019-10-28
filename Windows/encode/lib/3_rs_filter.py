# 针对最后加的RS码，再额外过筛选器
import random
import os,sys
from collections import Counter
input_file = sys.argv[1]
f_num = sys.argv[2]
f_num = int(f_num)
# 16 RS code
const_length = 16
# 因为是单独存的，应该多少都可以
str_cnt = 64
avoid_str = ['AAAA', 'CCCC', 'GGGG', 'TTTT', 'GCTCTTC', 'GAAGAGC', 'GCTGAGG', 'CCTCAGC', 'CCTAAGC', 'GCTTAGG', 'CCTTAGC', 'GCTAAGG', 'CCTGAGC', 'GCTCAGG', 'CCTCAGA', 'TCTGAGG', 'CCTCAGT', 'ACTGAGG', 'CCTCAGG', 'GCGGCCGC', 'CTTAAAGCGCT', 'AGATAG', 'TGTTGG', 'GAGCTG', 'AGTCTG']
print(len(avoid_str))
def change_str_to_bits_str(s):
    for i in range(len(s)):
        base = ''
        sub_s = s[i]
        for j in range(len(sub_s)):
            if sub_s[j] == 'A':
                base = base + str(0) + str(0)
            elif sub_s[j] == 'C':
                base = base + str(0) + str(1)
            elif sub_s[j] == 'G':
                base = base + str(1) + str(0)
            else:
                base = base + str(1) + str(1)
        avoid_str[i] = base
    return avoid_str
avoid_str = change_str_to_bits_str(avoid_str)

def int_to_bytes(number):
    # 只针对rs码16bits数据
    base = chr(int(number/256)).encode('latin1')
    base = base + chr(number % 256).encode('latin1')
    return base


# generate random bits
def generate_bits(str_cnt):
    arr = []
    for i in range(str_cnt):
        temp = random.getrandbits(const_length)
        while temp in arr:
            temp = random.getrandbits(const_length)
        arr.append(temp)
    return arr

# 判断一个序列中是否有需要完全规避的序列
def is_right(s):
    for i in range(len(avoid_str)):
        if avoid_str[i] in s:
            return False
    return True

# 将整数转换为二进制string
def changeinttobitstring(num):
    s = ''
    base = 0b1
    while num != 0:
        temp = str(num & base)
        s = s + temp
        num = num >> 1
    s = s[::-1]
    for i in range(const_length - len(s)):
        s = str(0) + s
    return s


# 将当前读入的bytes转为整数可以做异或操作，再转为二进制string以备判断是否符合筛选条件
def filter_ACGT(original_int, random_bits, filter_arr):
    s = changeinttobitstring(original_int)
    if is_right(s):
        if original_int in filter_arr:
            for i in range(str_cnt):
                filter_res = original_int ^ random_bits[i]
                if is_right(changeinttobitstring(filter_res)) and filter_res not in filter_arr:
                    index = i
                    filter_arr.append(filter_res)
                    return True, index
            return False, str_cnt
        else:
            filter_arr.append(original_int)
            return True, str_cnt
    else:
        signal = 0
        for i in range(str_cnt):
            filter_res = original_int ^ random_bits[i]
            if is_right(changeinttobitstring(filter_res)) and filter_res not in filter_arr:
                signal = 1
                index = i
                filter_arr.append(filter_res)
                break
        if signal:
            return True, index
        else:
            return False, str_cnt

# define the number of all the encoded files
f_num = 255
file_index = 0
random_bits = generate_bits(str_cnt)
rs_filename = 'rs_file.csv'
filter_arr = []
# os.remove(rs_filename)
while file_index < f_num:
    # read the data of the first three bytes
    s = input_file + str(file_index)
    with open(s, 'rb') as f:
        f.read(36)
        temp_int = int.from_bytes(f.read(2), byteorder='big', signed=False)
    # print('----------------------------------')

    with open(s, 'rb') as f:
        temp_bytes = f.read(38)
    # after get the int value of the 24 bits data, check filter
    res = filter_ACGT(temp_int, random_bits, filter_arr)

    if res[0]:
        if res[1] != str_cnt:
            # 写入的时候高8位是index，避免4个相同的碱基相连
            temp_result = temp_int ^ random_bits[res[1]]
            result = temp_bytes[0:36] + int_to_bytes(temp_result)
            with open(s, 'wb') as f:
                f.write(result)
            with open(rs_filename, 'a+') as f:
                f.write(str(temp_result) + ' ' + str(res[1]) + '\n')
        else:
            with open(s, 'wb') as f:
                f.write(temp_bytes)
        file_index = file_index + 1
    else:
        print('----------------------------------')
        random_bits = generate_bits(str_cnt)
        file_index = 0
        filter_arr = []
        os.remove(rs_filename)
#print(len(filter_arr))
count_result = Counter(filter_arr)
#print(len(count_result))

# save the random bits file
random_file = 'random_bits_rs.csv'
with open(random_file, 'w') as f:
    for i in range(str_cnt):
        f.write(str(random_bits[i]) + '\n')