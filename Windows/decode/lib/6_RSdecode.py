from reedsolo import RSCodec
import random,sys
# define the number of all the encoded files

input_file = sys.argv[1]
out_file = sys.argv[2]
f_num = sys.argv[3] #编码encode结束后得到的所有文件数
f_num = int(f_num)
rs = RSCodec(2)
def int_to_bytes(number):
    # 只针对rs码16bits数据
    base = chr(int(number/256)).encode('latin1')
    base = base + chr(number % 256).encode('latin1')
    return base

# 首先还原RS码本身
random_rs_name = 'random_bits_rs.csv'  #输入位置
rs_file = 'rs_file.csv'
random_bits = []
f = open(random_rs_name, 'r') #这里是打开校验文件
for line in f:
    random_bits.append(int(line.rstrip()))
f.close() 
# print(random_bits) 上面是统计random_bits_rs.csv 文件的所有序列的加和总长

filter_rs_num = [] #存储数字？
filter_rs_index = []
f = open(rs_file, 'r') #这里是打开纠错文件，纠错文件是单独保存并且没有经过DNA转码的！
for line in f:
    temp = str(line.rstrip()).split(' ')
    filter_rs_num.append(int(temp[0]))
    filter_rs_index.append(int(temp[1]))
f.close()
# print(filter_rs_num) 引物序列与信息序列是分割开的？
# print(filter_rs_index)


#如果37、38位的数字在索引文件中，使用索引内容替换后两位。这样才能完成RS解码。实际上使用特定非合成DNA？
for i in range(f_num):
    s = input_file + str(i) #s表示单独的一个序列文件
    new_s = out_file+ str(i) #表示解码的结果
    with open(s, 'rb') as f:   #读取文件的第37和38位，判断其是否出现在index中，如果没有出现，就不需要修正，否则进行下面的修正，也就是用index来替代
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
            temp_bytes = f.read(38)  #
        with open(new_s, 'wb') as f:
            f.write(temp_bytes)

# RS decode #正式将修正过的序列按照
for i in range(f_num):
    s = out_file + str(i)
    f1 = open(s, 'rb')
    old_lst = f1.read(38)
    f1.close()
    f2 = open(s, 'wb')
   # print(old_lst)
   # array_msg = bytearray(old_lst)
   # print(array_msg)
    try:
        new_lst = rs.decode(old_lst)
    except:
        continue
    else:
        f2.write(new_lst)
    f2.close()
