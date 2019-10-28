from reedsolo import RSCodec
import sys
input_file = sys.argv[1]
f_num = sys.argv[2]
f_num = int(f_num)
# define the number of all the encoded files
# RS
rs = RSCodec(2)

for i in range(f_num):
    s = input_file + str(i)
    with open(s, 'rb') as f:
        temp = f.read(36)
    lst = temp
    new_lst = rs.encode(lst)
    with open(s, 'wb') as f:
        f.write(new_lst)


