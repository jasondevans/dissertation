import sys

# Decryption of the very basic / simplistic data file encryption.

filestr = sys.argv[1]
xindex = filestr.rfind('.')
ofilestr = filestr[0:xindex] + '--Decrypted' + filestr[xindex:]
outputfile = open(ofilestr, 'w+')

# newfilestr = 'Data -- Main\\ExperimentDataC' + sys.argv[1][24:]
# outputfile = open(newfilestr, 'w+')

with open(filestr, 'r') as f:
	mystr = f.read()
	str_list = []
	for char in mystr:
		str_list.append(chr(ord(char) ^ ord('q')))
	decrypt_str = ''.join(str_list)
	outputfile.write(decrypt_str)
