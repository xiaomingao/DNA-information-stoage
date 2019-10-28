import sys
input_file = sys.argv[1]
out_file = sys.argv[2]
f_num = sys.argv[3]
f_num = int(f_num)

const_length = 38 * 8
filename = out_file
file = open(filename, 'w')

def change_bits_str_to_str(s):
    base = ''
    index = 0
    while index < len(s):
        if s[index: index + 2] == '00':
            base = base + 'A'
        elif s[index:index + 2] == '01':
            base = base + 'C'
        elif s[index: index + 2] == '10':
            base = base + 'G'
        else:
            base = base + 'T'
        index = index + 2
    return base

# 将整数转换为二进制string
def change_int_to_bitstring(num):
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

for i in range(f_num):
    temp_file = input_file + str(i)
    with open(temp_file, 'rb') as f:
        temp_int = int.from_bytes(f.read(38), byteorder='big', signed=False)
        temp_bits_str = change_int_to_bitstring(temp_int)
        #print(temp_bits_str)
        temp_str = change_bits_str_to_str(temp_bits_str)
    file.write(temp_str[0:4] + ',' + temp_str[4:144] + ','+ temp_str[144:152] + '\n')
file.close()
