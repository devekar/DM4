import csv
import math



def get_mean(vector):
	return sum(vector) / len(vector)

def std_dev(l):
	m = get_mean(l)
	sd = 0 
	for item in l:
		sd += (item - m)**2
	return math.sqrt(sd / len(l))


cluster_sizes = []
f = open("cluster_sizes_1.txt", 'rb')
reader = csv.reader(f, delimiter=',')
for row in reader:
	cluster_sizes.append(map(int, row))

for item in cluster_sizes:
	mean = get_mean(item)
	sd = std_dev(item)
	print mean, sd

