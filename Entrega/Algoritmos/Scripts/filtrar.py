entrada = "pancake16.txt"
corridas = "e.txt"

with open(corridas, "r") as g:
	todo = g.read()

todo = todo.split("\n")

todo = [ x for x in todo if not "na" in x]

result = []


def esta(li):
	for x in todo:
		if li in x:
			return True
	return False


with open(entrada, "r") as f:
	for line in f:
		cont = line.strip()
		if esta(cont):
			continue
		result.append(cont)

with open("f_"+entrada,"w+") as f:
	for p in result:
		f.write(p+"\n")


with open("f_"+corridas,"w+") as f:
	for p in todo:
		f.write(p+"\n")
		

