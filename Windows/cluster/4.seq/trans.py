#!/usr/bin/python
#python trans.py AAAAAA output.txt
import sys
seq = sys.argv[1];
file1 = open(sys.argv[2], 'wb')
def change_str_to_bit_str(s):
	base = ''
	index = 0
	while index < len(s):
		if s[index] == 'A':
			base = base + '00'
		elif s[index] == 'C':
			base = base + '01'
		elif s[index] == 'G':
			base = base + '10'
		else:
			base = base + '11'
		index = index + 1
	return base
	

temp_str = change_str_to_bit_str(seq)
#print(temp_str)
temp_str =int(temp_str,2)
#print(temp_str)
s=temp_str.to_bytes(38,byteorder = 'big',signed=False)
#print(s)
file1.write(s)
file1.close()
