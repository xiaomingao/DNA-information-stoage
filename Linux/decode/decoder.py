#python3 encode.py ./test/test.txt ./encode.csv
#是否把几个固定计算机文件的路径作为参数输入？然后考到定向的位置？
import sys,os,shutil

input_file = sys.argv[1]
out_file = sys.argv[2]

shutil.rmtree("./temp")
os.mkdir("./temp")
#shutil.copy(input_file,"./decode/decode_file")


count=0;
for file in os.listdir(input_file):
	count=count+1
f_num=count-2 ##此处确立隐藏文件
#f_num=count-1
print(f_num)

#f_num=256

f_num =str(f_num)

cmd = "python3 ./lib/6_RSdecode.py "+input_file+"/encode_file_cod ./decode/decode_file_cod "+f_num
os.system(cmd)
cmd = "python3 ./lib/7_filter_decode.py ./decode/decode_file_cod "+f_num
os.system(cmd)
#以下不使用
#cmd = "python3 ./lib/8_check_encode.py ./decode/encode_file_cod "+f_num
#os.system(cmd)


shutil.copy("./encode/encode_file_meta.txt", "./decode/decode_file_meta.txt")

s = './decode/decode_file_meta.txt'
f1 = open(s, 'r')
old_lst = f1.readline()
#print(old_lst)
ls = old_lst.split(" ", 1)
lss="./decode/decode_file "+ls[1]
print(ls)
f1.close()
f2 = open(s, 'w')
f2.write(lss)
f2.close()


cmd = "./lib/repair ./decode/decode_file"
os.system(cmd)
cmd = "./lib/decoder ./decode/decode_file"
os.system(cmd)
#shutil.copy("./decode/decode_file", out_file)
