from __future__ import division
from collections import defaultdict
import itertools
import math
import sys
import timeit

class DBSCAN:
    word_list = None
    topic_list = None
    dataset = None
    EPSILON = None
    MINPTS = None
    clusters = [[None]]
    magnitudes = None

    # Initialize global variables
    def __init__(self, data, EPSILON, MINPTS):
        self.word_list = data["word_list"]
        self.topic_list = data["topic_list"]
        self.dataset = data["matrix"]
        self.EPSILON = EPSILON
        self.MINPTS = MINPTS
        self.magnitudes = [None]*len(self.dataset)

        time1 = timeit.default_timer()
        self.computeMagnitudes()
        time2 = timeit.default_timer()
        print "Magnitudes computed in time: ", str(time2 - time1)
        self.runDBSCAN()
        time3 = timeit.default_timer()
        print "Time to cluster: ", str(time3 - time2)
        print "EPSILON= " + str(EPSILON) 
        print "MINPTS= " + str(MINPTS)


    def computeMagnitudes(self):
        for idx, i in enumerate(self.dataset):
            self.magnitudes[idx] = self.magnitude(i[1:len(self.word_list)+1])


    # Compute magnitude of a vector
    def magnitude(self, vector):
        mag = 0;
        for item in vector:
            mag += item * item
        mag = math.sqrt(mag)
        return mag;


    # Compute cosine distance between two numeric vectors
    def cosine_dist(self, P1, P2):
        dot_product = 0;
        vector1 = self.dataset[P1][1:len(self.word_list) + 1]
        vector2 = self.dataset[P2][1:len(self.word_list) + 1]
        for i1, i2 in itertools.izip(vector1, vector2):
            dot_product += i1 * i2;
        mag1 = 1
        mag2 = 1
        if dot_product > 0:
            mag1 = self.magnitudes[P1];
            mag2 = self.magnitudes[P2];
        cosine_sim = dot_product / (mag1 * mag2)
        cosine_dist = 1 - cosine_sim 
        return cosine_dist

    # TODO: UNUSED CODE
    # Get distance between two instances
    def get_dist(self, P1, P2):
        return self.cosine_dist(P1, P2)


    def runDBSCAN(self):
        C = 0
        print "Cluster# Datapoint#"
        for i in range(len(self.dataset[:100])):
            if isinstance(self.dataset[i][0], int): continue  # Univisted points have string "Article #" in this field
            self.dataset[i][0] = 0 # Mark as visited      
            NeighborPts = self.regionQuery(i)

            if len(NeighborPts) < self.MINPTS:
                self.dataset[i][0] = -1 # Mark as noise
            else:
                C += 1   # next cluster
                print C, i   #print current cluster and row/point to be expanded
                self.expandCluster(i, NeighborPts, C)

        # Display clusters
        for idx, i in enumerate(self.clusters):
            print idx,"- " , len(i)#, ": ", i

        # Display Noise
        sys.stdout.write("Noise: "); noiseCnt = 0
        for idx, i in enumerate(self.dataset):
            if i[0]==-1: noiseCnt += 1
                #sys.stdout.write(str(idx) + " ")
        sys.stdout.write(str(noiseCnt) + "\n")
                

    def expandCluster(self, P, NeighborPts, C):
        self.dataset[P][0] = C
        for i in NeighborPts:
            if not isinstance(self.dataset[i][0], int): # An unvisited point
                self.dataset[i][0] = 0
                NeighborPts_i = self.regionQuery(i)
                if len(NeighborPts_i) >= self.MINPTS:
                    l = [x for x in NeighborPts_i if x not in NeighborPts]
                    NeighborPts.extend(l)
            if not self.dataset[i][0] > 0:   # An unclustered point(can be noise)
                self.dataset[i][0] = C

        self.clusters.append(NeighborPts) # keep record of this cluster
      
    
    # Return points in its EPSILON neighborhood
    def regionQuery(self, P):
        NeighborPts = []
        for i in range(len(self.dataset)):
            if self.cosine_dist(i, P) < self.EPSILON:
                NeighborPts.append(i)
        return NeighborPts

