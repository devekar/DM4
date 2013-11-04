from dbscan1 import DBSCAN
import csv
import timeit
import sys

def parseDM(filepath = r'data_matrix.csv'):
    dataMatrix = []

    matrix = []
    word_list = []
    topic_list = []
    with open(filepath, 'rb') as csv_file:
        reader = csv.reader(csv_file, delimiter=',', quotechar='"')
        for row in reader:
            dataMatrix.append(row)

    for item in dataMatrix[1]:
        if "_" not in item:
            word_list.append(item)
        elif "t_" in item:
            topic_list.append(item[2:])

    word_list = word_list[1:] # Remove 'Article #'
    words_topics_size = len(topic_list) + len(word_list)

    for row in dataMatrix[2:]:
        matrix.append( [row[0]] + map(int, row[1:1 + words_topics_size]) )
    return {"topic_list":topic_list, "word_list": word_list, "matrix": matrix}



########
# MAIN #
########

time = -timeit.default_timer()
time1 = time
dataMatrix = parseDM()
time += timeit.default_timer()
print "\nTime to read: ", str(time)

EPSILON = float(sys.argv[1])
MINPTS = int(sys.argv[2])

d = DBSCAN(dataMatrix, EPSILON, MINPTS)
time1 += timeit.default_timer()
print "Total time: ", str(time1)
