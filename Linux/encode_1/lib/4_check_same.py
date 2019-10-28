from collections import Counter
import sys
input_file = sys.argv[1]
f_num = sys.argv[2]
f_num = int(f_num)
result = []

for i in range(f_num):
    s = input_file + str(i)
    with open(s, 'rb') as f:
        result.append(f.read(38))

count_result = Counter(result)
if len(count_result) == f_num:
    print('Yes')
else:
    print('No')

