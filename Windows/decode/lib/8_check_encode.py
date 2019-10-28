# compare with the encoded files
f_num = 10752
for i in range(f_num):
    s1 = '../../encode-01/file_1.rar_cod' + str(i)
    s2 = './decode_result/file_1.rar_cod' + str(i)
    f1 = open(s1, 'rb')
    f2 = open(s2, 'rb')
    temp1 = f1.read(36)
    temp2 = f2.read(36)
    if temp1 != temp2:
        print(str(i))
        print(temp1)
        print(temp2)
    f1.close()
    f2.close()