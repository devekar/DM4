from __future__ import division
import csv
import math
import sys


def get_mean(vector):
	return sum(vector) / len(vector)

def std_dev(l):
	m = get_mean(l)
	sd = 0 
	for item in l:
		sd += (item - m)**2
	return math.sqrt(sd / len(l))


filename = sys.argv[1]
cluster_sizes = []
f = open(filename, 'rb')
reader = csv.reader(f, delimiter=',')
for row in reader:
	cluster_sizes.append(map(int, row))

for item in cluster_sizes:
	mean = get_mean(item)
	sd = std_dev(item)
	print len(item), mean, sd

