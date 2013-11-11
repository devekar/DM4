import csv
import timeit
import sys

#
# Convert CSV data matrix into space separated matrix
#

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
        if ok: matrix.append(l[:len(word_list)])
    return matrix, len(word_list), len(topic_list)

    
def writeDMForCPP(matrix, wsize, tsize, filename):
    f = open(filename,"w")
    f.write(str(len(matrix)) + '\n')
    f.write(str(wsize) + '\n')
    f.write(str(tsize) + '\n')
    for row in matrix:
        f.write(' '.join(str(s) for s in row) + '\n')
    f.close()
    
    
########
# MAIN #
########
    
time = -timeit.default_timer()

matrix, wsize, tsize = parseDM()

filename = None
if len(sys.argv) > 1: 
    filename = "CPPmatrixBoolean.txt"
    for idx, row in enumerate(matrix):
        matrix[idx] = [1 if i else 0 for i in row]
else: filename = "CPPmatrixActual.txt"
    
        
writeDMForCPP(matrix, wsize, tsize, filename)

time += timeit.default_timer()
print "Total time: ", str(time)