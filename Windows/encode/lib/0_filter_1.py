# 对最高8位后面的所有位进行处理，将4个连着的也加入完全规避序列进行处理
#python 0_filter_1.py ./temp/encode_file
import random,sys
# define the number of all the encoded files
input_file = sys.argv[1]
out_file = sys.argv[2]
f_num = sys.argv[3] #z
f_num = int(f_num)
print(f_num)




# 280
const_length = (36 - 1) * 8
# random_bits num is from 1 to 14, then the first byte data can satisfy the first filter condition
str_cnt = 253
avoid_str = ['AAAA', 'CCCC', 'GGGG', 'TTTT', 'GCTCTTC', 'GAAGAGC', 'GCTGAGG', 'CCTCAGC', 'CCTAAGC', 'GCTTAGG', 'CCTTAGC', 'GCTAAGG', 'CCTGAGC', 'GCTCAGG', 'CCTCAGA', 'TCTGAGG', 'CCTCAGT', 'ACTGAGG', 'CCTCAGG', 'GCGGCCGC', 'CTTAAAGCGCT', 'AGATAG', 'TGTTGG', 'GAGCTG', 'AGTCTG']
def change_str_to_bits_str(s): #完成从DNA序列到01的转化
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

def int_to_bytes(number):  #将位数转为字节数
    # all the data except the first byte
    for i in range(int(const_length / 8)):
        if i == 0:
            base = chr(int(number/pow(256, int(const_length / 8) - 1))).encode('latin1')
            number = number - pow(256, int(const_length / 8) - 1) * int(number / pow(256, int(const_length / 8) - 1))
        elif i == int(const_length / 8) - 1:
            base = base + chr(number % 256).encode('latin1')
        else:
            base = base + chr(int(number / pow(256, int(const_length / 8) - 1 - i))).encode('latin1')
            number = number - pow(256, int(const_length / 8) - 1 - i) * int(number / pow(256, int(const_length / 8) - 1- i))
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
def filter_ACGT(original_int, random_bits):
    s = changeinttobitstring(original_int)
    if is_right(s):
        return True, str_cnt
    else:
        signal = 0
        for i in range(str_cnt):
            if is_right(changeinttobitstring(original_int ^ random_bits[i])):
                signal = 1
                index = i
                break
        if signal:
            return True, index
        else:
            return False, str_cnt



# 将全为零的8位提到最前以方便操作
for i in range(f_num):
    s = input_file + str(i)
    new_s = out_file + str(i)
    with open(s, 'rb') as f:
        temp_bytes = f.read(36)
    # print(temp_bytes[3])
    with open(new_s, 'wb') as f:
        f.write(chr(temp_bytes[3]).encode('latin1') + temp_bytes[0:3] + temp_bytes[4:36])

# 过筛选器
file_index = 0
random_bits = generate_bits(str_cnt)
while file_index < f_num:
    # read the data of the first three bytes
    s = out_file + str(file_index)
    with open(s, 'rb') as f:
        f.read(1)
        temp_int = int.from_bytes(f.read(35), byteorder='big', signed=False)
    # print('----------------------------------')

    with open(s, 'rb') as f:
        temp_bytes = f.read(36)
    # after get the int value of the 24 bits data, check filter
    res = filter_ACGT(temp_int, random_bits)
    if res[0]:
        if res[1] != str_cnt:
            # 写入的时候高8位是index+1，避免4个相同的碱基相连
            temp_result = temp_int ^ random_bits[res[1]]
            result = chr(res[1] + 1).encode('latin1') + int_to_bytes(temp_result)
            with open(s, 'wb') as f:
                f.write(result)
        else:
            with open(s, 'wb') as f:
                f.write(temp_bytes)
        file_index = file_index + 1
    else:
        print('----------------------------------')
        random_bits = generate_bits(str_cnt)
        file_index = 0

# save the random bits file
random_file = 'random_bits_2.csv'
with open(random_file, 'w') as f:
    for i in range(str_cnt):
        f.write(str(random_bits[i]) + '\n')
