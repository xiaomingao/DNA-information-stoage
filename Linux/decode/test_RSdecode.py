from reedsolo import RSCodec
# define the number of all the encoded files
f_num = 256
rs = RSCodec(2)
def int_to_bytes(number):
    # 只针对rs码16bits数据
    base = chr(int(number/256)).encode('latin1')
    base = base + chr(number % 256).encode('latin1')
    return base

# 首先还原RS码本身
random_rs_name = 'random_bits_rs.csv'
rs_file = 'rs_file.csv'
random_bits = []
f = open(random_rs_name, 'r')
for line in f:
    random_bits.append(int(line.rstrip()))
f.close()
# print(random_bits)

filter_rs_num = []
filter_rs_index = []
f = open(rs_file, 'r')
for line in f:
    temp = str(line.rstrip()).split(' ')
    filter_rs_num.append(int(temp[0]))
    filter_rs_index.append(int(temp[1]))
f.close()
# print(filter_rs_num)
# print(filter_rs_index)

for i in range(f_num):
    s = './encode/encode_file_cod' + str(i)
    new_s = './temp/file_1.rar_cod' + str(i)
    with open(s, 'rb') as f:
        f.read(36)	
        temp_int = int.from_bytes(f.read(2), byteorder='big', signed=False)
    if temp_int in filter_rs_num:
        filter_random_bits = random_bits[filter_rs_index[filter_rs_num.index(temp_int)]]
        temp_res = temp_int ^ filter_random_bits
        with open(s, 'rb') as f:
            temp_bytes = f.read(36)
        with open(new_s, 'wb') as f:
            f.write(temp_bytes + int_to_bytes(temp_res))
    else:
        with open(s, 'rb') as f:
            temp_bytes = f.read(38)
        with open(new_s, 'wb') as f:
            f.write(temp_bytes)

# RS decode
for i in range(f_num):
    s = './decode/decode_file_cod' + str(i)
    f1 = open(s, 'rb')
    old_lst = f1.read(38)
    f1.close()
    f2 = open(s, 'wb')
    new_lst = rs.decode(old_lst)
    f2.write(new_lst)
    f2.close()
