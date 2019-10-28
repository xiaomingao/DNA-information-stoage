#python3 encode.py ./file ./encode.csv
import sys,os,shutil

input_file = sys.argv[1]
out_file = sys.argv[2]
print("1111")
shutil.rmtree("./temp")
os.mkdir("./temp")
print("1111")
shutil.rmtree("./encode")
os.mkdir("./encode")
print("1111")
shutil.copy(input_file, "./temp/encode_file")
print("1111")
os.system("F:\CODE\encode\lib\encode ./temp/encode_file")

count=0;
for file in os.listdir("./temp"):
        count=count+1
f_num=count-2
print("1111")
f_num =str(f_num)

cmd = "python ./lib/0_filter_1.py ./temp/encode_file_cod ./encode/encode_file_cod "+f_num
os.system(cmd)
print(1)
cmd = "python ./lib/1_filter_2.py ./encode/encode_file_cod "+f_num
os.system(cmd)
print(1)
cmd = "python ./lib/2_RSencode.py ./encode/encode_file_cod "+f_num
os.system(cmd)
cmd = "python ./lib/3_rs_filter.py ./encode/encode_file_cod "+f_num
os.system(cmd)
cmd = "python ./lib/4_check_same.py ./encode/encode_file_cod "+f_num
os.system(cmd)
cmd = "python ./lib/5_write_csv.py ./encode/encode_file_cod "+out_file+" "+f_num
os.system(cmd)
shutil.copy("./temp/encode_file_meta.txt", "./encode/encode_file_meta.txt")

