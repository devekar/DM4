'''
Created on Nov 4, 2013

@author: Akshay
'''

from __future__ import division
import timeit
import math
import itertools
import random
import copy

class KMeans(object):
    K = 2
    word_list = None
    topic_list = None
    dataset = None
    magnitudes = None
    clusters = []
    cluster_means = []
    epsilon = 0.00001

    def __init__(self, k, data):
        self.K = k
        self.word_list = data["word_list"]
        self.topic_list = data["topic_list"]
        self.dataset = data["matrix"]
        self.magnitudes = [None]*len(self.dataset)
        time1 = timeit.default_timer()
        self.computeMagnitudes()
        time2 = timeit.default_timer()
        print "Magnitudes computed in time: ", str(time2 - time1)
        
        
    def computeMagnitudes(self):
        for idx, i in enumerate(self.dataset):
            self.magnitudes[idx] = self.magnitude(i[1:len(self.word_list)+1])


    '''Compute magnitude of a vector'''
    def magnitude(self, vector):
        mag = 0;
        for item in vector:
            mag += item * item
        mag = math.sqrt(mag)
        return mag;
    
    
    '''Compute cosine distance between two numeric vectors'''
    def cosine_dist(self, record_idx, cluster_idx):
        dot_product = 0;
        vector1 = self.dataset[record_idx][1:len(self.word_list) + 1]
        vector2 = self.cluster_means[cluster_idx]
        for i1, i2 in itertools.izip(vector1, vector2):
            dot_product += i1 * i2;
        mag1 = 1
        mag2 = 1
        if dot_product > 0:
            mag1 = self.magnitudes[record_idx];
            mag2 = self.magnitude(self.cluster_means[cluster_idx]);
        cosine_sim = dot_product / (mag1 * mag2)
        cosine_dist = 1 - cosine_sim 
        return cosine_dist
    
    
    def cosine_dist_means(self, vector1, vector2):
        dot_product = 0;
        for i1, i2 in itertools.izip(vector1, vector2):
            dot_product += i1 * i2;
        mag1 = 1
        mag2 = 1
        if dot_product > 0:
            mag1 = self.magnitude(vector1);
            mag2 = self.magnitude(vector2);
        cosine_sim = dot_product / (mag1 * mag2)
        cosine_dist = 1 - cosine_sim 
        return cosine_dist
    
    def get_mean(self, vector):
        return sum(vector) / len(vector)
    
    
    def get_mean_vector(self, list):
        vectors = []
        for item in list:
            trimmed = item[1:len(self.word_list)+1]
            vectors.append(trimmed)
        return map(self.get_mean, zip(*vectors))
    
    def get_cluster_mean_vector(self, list):
        return map(self.get_mean, zip(*list))
    
    
    '''Main Algorithm'''
    def get_clusters(self):
        kmeans_start = timeit.default_timer()
        '''Start with random cluster means'''
        samples = random.sample(self.dataset, 1000)
        sample_mean = self.get_mean_vector(samples)
        self.cluster_means = [[] for i in xrange(self.K)]
        for i in xrange(self.K):
            for j in xrange(len(self.word_list)):
                self.cluster_means[i].append(sample_mean[j] + random.random())
        '''Main loop'''
        for i in xrange(1000):
            print "Starting iteration " + str(i)
            '''Assign points to the nearest cluster'''
            self.clusters = [[] for j in xrange(self.K)]
            for idx,point in enumerate(self.dataset):
                distances = []
                for k in xrange(self.K):
                    distances.append(self.cosine_dist(idx, k))
                closest_cluster = distances.index(min(distances))
                self.clusters[closest_cluster].append(point)
            print [len(a) for a in self.clusters]
            '''Recompute centroids for each cluster'''
            old_cluster_means = copy.copy(self.cluster_means)
            for k in xrange(self.K):
                self.cluster_means[k] = self.get_mean_vector(self.clusters[k])
                #print "Mean " + str(k) + ": " + str(self.cluster_means[k])
            '''Check if means changed significantly'''
            diff = [self.cosine_dist_means(new, old) for new,old in zip(self.cluster_means, old_cluster_means)]
            print "diff = " + str(diff)
            if sum(diff) <= self.epsilon:
                print "Converged in " + str(i+1) + " iterations"
                break
        kmeans_stop = timeit.default_timer()
        print "K-means converged in time: " + str(kmeans_stop - kmeans_start) + " seconds"