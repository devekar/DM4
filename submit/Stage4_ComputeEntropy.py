from __future__ import division
from collections import defaultdict
import operator
import timeit
import math
import itertools
import os
import csv
import copy
import sys
# import matplotlib.pyplot as plt

def parseDM(filepath = r'data_matrix.csv'):
    dataMatrix = []

    matrix = []
    word_list = []
    topic_list = []
    place_list = []
    with open(filepath, 'rb') as csv_file:
        reader = csv.reader(csv_file, delimiter=',', quotechar='"')
        for row in reader:
            dataMatrix.append(row)

    for item in dataMatrix[1]:
        if "_" not in item:
            word_list.append(item)
        elif "t_" in item:
            topic_list.append(item[2:])
        elif "p_" in item:
            place_list.append(item[2:])

    word_list = word_list[1:] # Remove 'Article #'
    words_topics_size = len(topic_list) + len(word_list)

    for row in dataMatrix[2:]:
        l = map( int, row[1:1 + words_topics_size])
        ok = False
        for i in l[ len(word_list):]:
            if i>0:
                ok = True; break
        if ok: matrix.append(l)
    return matrix, len(word_list), len(topic_list)
   
    
'''compute entropy'''
def computeEntropy(clusters, wsize, tsize, labels):
    K = len(clusters)
    cluster_entropies = [[0 for i in xrange(tsize)] for j in xrange(K)]
    cluster_labeled_sizes = []
    for k in xrange(K):
        for point in clusters[k]:
            vec = copy.copy(labels[point])
            if sum(vec) > 0:
                vec = [a/sum(vec) for a in vec]
                cluster_entropies[k] = [a+b for a,b in zip(cluster_entropies[k], vec)]
        cluster_labeled_sizes.append(sum(cluster_entropies[k]))
        if sum(cluster_entropies[k]) > 0: 
            cluster_entropies[k] = [a/sum(cluster_entropies[k]) for a in cluster_entropies[k]]
        cluster_entropies[k] = [-1*p*math.log(p,2) if p>0 else 0 for p in cluster_entropies[k]]
        cluster_entropies[k] = sum(cluster_entropies[k])
    cluster_weights = [a/sum(cluster_labeled_sizes) for a in cluster_labeled_sizes]
    entropy = sum([a*b for a,b in zip(cluster_weights, cluster_entropies)])
    return entropy
    


def readClusters(filename):
    f = open(filename)
    lines = f.readlines()
    f.close()
    clusters = []
    for line in lines:
        l = line.split()
        if l:
            clusters.append( map(int, l) )

    return clusters
    
    
def readDirectory(PATH, wsize, tsize, matrix):
    d = defaultdict(int)
    x = []
    y = []
    for dir_entry in sorted(os.listdir(PATH)):   #each file
        dir_entry_path = os.path.join(PATH, dir_entry)
        clusters = readClusters(dir_entry_path)
        entropy = computeEntropy(clusters, wsize, tsize, matrix)
        print [len(c) for c in clusters]
        print "Entropy for", len(clusters), "clusters:", entropy
        print ""
        d[len(clusters)] = entropy
        
    sorted_d = sorted(d.iteritems(), key=operator.itemgetter(0))
    for key, value in sorted_d:
        x.append(key)
        y.append(value)
        
    print "Cluster Entropies:", sorted_d
    return x, y    
        
    
    
########
# MAIN #
########

if len(sys.argv) < 2:
    print "Incorrect Args."
    print "Usage: <Directory with output files from Stage 3>"
    sys.exit(1)

time = -timeit.default_timer()    
    
matrix, wsize, tsize = parseDM()
for idx, row in enumerate(matrix):
    matrix[idx] = row[wsize:]
print "Labels read"
    
x, y = readDirectory(sys.argv[1], wsize, tsize, matrix)            

time += timeit.default_timer()
print "Total time: ", str(time)

'''
plt.plot(x,y, color="red")
plt.title('Entropy plot for number of clusters')
plt.xlabel('Number of clusters')
plt.ylabel('Entropy')
plt.legend()
plt.show()
'''