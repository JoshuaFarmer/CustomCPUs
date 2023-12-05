import sys

labels = []
labelNames = []

def getRegID(chr):
		if chr.upper() == "PC":
			return 7
		elif chr.upper() == "SP":
			return 6
		elif chr.upper() == "SR":
			return 8
		return ord(chr.upper()) - 65

def getLabel(nam):
	return labels[labelNames.index(nam)]

assembled = []

def findLabels(dat):
	j = 0
	while j < len(dat):
		if dat[j].upper() == "(LABEL)":
			j += 1
			labels.append(j)
			labelNames.append(dat[j])
			print("created label at: " + str(j))
		j += 1

def make(dat):
	findLabels(dat)
	i = 0
	while i < len(dat) - 1:
		if dat[i].upper() == "NO-OP":
			assembled.append(0)
		elif dat[i].upper() == "HALT":
			assembled.append(1)
			print("created exit point...")
		elif dat[i].upper() == "SET":
			assembled.append(2)
			i += 1
			assembled.append(getRegID(dat[i]))
			i += 1
			assembled.append(dat[i])
			print(f"set {dat[i-1]} to {dat[i]}")
		elif dat[i].upper() == "ADD":
			assembled.append(3)
			i += 1
			assembled.append(getRegID(dat[i]))
			i += 1
			assembled.append(getRegID(dat[i]))
		elif dat[i].upper() == "CPY":
			assembled.append(4)
			i += 1
			assembled.append(getRegID(dat[i]))
			i += 1
			assembled.append(getRegID(dat[i]))
		elif dat[i].upper() == "ST":
			assembled.append(5)
			i += 1
			assembled.append(getRegID(dat[i]))
		elif dat[i].upper() == "LD":
			assembled.append(6)
			i += 1
			assembled.append(getRegID(dat[i]))
		elif dat[i].upper() == "OUT":
			assembled.append(7)
			i += 1
			assembled.append(getRegID(dat[i]))
		elif dat[i].upper() == "PSH":
			assembled.append(8)
			i += 1
			assembled.append(getRegID(dat[i]))
		elif dat[i].upper() == "POP":
			assembled.append(9)
			i += 1
			assembled.append(getRegID(dat[i]))

		elif dat[i].upper() == "JMP":
			assembled.append(2)
			assembled.append(7)
			i += 1
			if dat[i].isnumeric():
				assembled.append(int(dat[i]))
			else:
				assembled.append(getLabel(dat[i]) - 1)
		elif dat[i].upper() == "SUB":
			assembled.append(10)
			i += 1
			assembled.append(getRegID(dat[i]))
			i += 1
			assembled.append(getRegID(dat[i]))
		elif dat[i].upper() == "CMP":
			assembled.append(11)
			i += 1
			assembled.append(getRegID(dat[i]))
			i += 1
			assembled.append(getRegID(dat[i]))
		elif dat[i].upper() == "JZ":
			assembled.append(12)
			i += 1
			if dat[i].isnumeric():
				assembled.append(int(dat[i]))
			else:
				assembled.append(getLabel(dat[i]) - 1)
		elif dat[i].upper() == "JNZ":
			assembled.append(13)
			i += 1
			if dat[i].isnumeric():
				assembled.append(int(dat[i]))
			else:
				assembled.append(getLabel(dat[i]) - 1)
		elif dat[i].upper() == "JE":
			assembled.append(14)
			i += 1
			if dat[i].isnumeric():
				assembled.append(int(dat[i]))
			else:
				assembled.append(getLabel(dat[i]) - 1)
		elif dat[i].upper() == "JL":
			assembled.append(15)
			i += 1
			if dat[i].isnumeric():
				assembled.append(int(dat[i]))
			else:
				assembled.append(getLabel(dat[i]) - 1)
		elif dat[i].upper() == "JG":
			assembled.append(16)
			i += 1
			if dat[i].isnumeric():
				assembled.append(int(dat[i]))
			else:
				assembled.append(getLabel(dat[i]) - 1)
		elif dat[i].upper() == "CALL":
			# push PC
			assembled.append(8)
			assembled.append(7)
			# jmp
			assembled.append(2)
			assembled.append(7)
			i += 1
			assembled.append(getLabel(dat[i]) - 1)
			print("created call to: " + str(getLabel(dat[i])))
		elif dat[i].upper() == "RET":
			# pop PC
			assembled.append(9)
			assembled.append(7)
		elif dat[i].upper() == "IN":
			assembled.append(17)
			i += 1
			assembled.append(getRegID(dat[i]))
		elif dat[i].upper() == "CMPV":
			assembled.append(18)
			i += 1
			assembled.append(getRegID(dat[i]))
		elif dat[i] == ";":
			i+=1
			while dat[i] != ";":
				assembled.append(0)
				i += 1
			pass
		i += 1

if len(sys.argv) > 1:
	raw = open(sys.argv[1]).read().split("\n")
	dat = []
	for line in raw:
		for sub in line.split(" "):
			dat.append(sub.strip("\t"))
	make(dat)
	nam = "out.bin"
	if len(sys.argv) > 2:
		nam = sys.argv[2]
	open(nam, "w").write("")
	for line in assembled:
		print(line)
		open(nam, "a").write(str(line) + "\n")