from __future__ import division

class DBSCAN:
    word_list = []
    topic_list = []
    dataset = []
    EPSILON = 0
    MINPTS = 0


    # Initialize global variables
    def __init__(self, data, EPSILON, MINPTS):
        self.word_list = data["word_list"]
        self.topic_list = data["topic_list"]
        self.dataset = data["matrix"]
        self.EPSILON = EPSILON
        self.MINPTS = MINPTS
        self.runDBSCAN()
        

    # Compute magnitude of a vector
    def magnitude(self, vector):
        mag = 0;
        for item in vector:
            mag += item * item
        mag = math.sqrt(mag)
        return mag;


    # Compute cosine distance between two numeric vectors
    def cosine_dist(self, vector1, vector2):
        assert (len(vector1) == len(vector2)), \
            "Lengths of two vectors should be same"
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


    # Get distance between two instances
    def get_dist(self, inst1, inst2):
        vector1 = inst1[1:len(self.word_list)+1]
        vector2 = inst2[1:len(self.word_list)+1]
        return self.cosine_dist(vector1, vector2)


    def runDBSCAN(self):
        C = 0
        for i in range(len(self.dataset)):
            if isinstance(self.dataset[i][0], int): continue
            self.dataset[i][0] = 0
      
            NeighborPts = self.regionQuery(i)
            if len(NeighborPts) < self.MINPTS:
                self.dataset[i][0] = -1
            else:
                C += 1
                print C, i
                self.expandCluster(i, NeighborPts, C)
                
          
    def expandCluster(self, P, NeighborPts, C):
        self.dataset[P][0] = C
        for i in NeighborPts:
            if not isinstance(self.dataset[i][0], int):
                self.dataset[i][0] = 0
                NeighborPts_i = self.regionQuery(i)
                if len(NeighborPts_i) >= self.MINPTS:
                    NeighborPts.extend(NeighborPts_i)
            if not self.dataset[i][0] > 0:
                self.dataset[i][0] = C
      
      
    def regionQuery(self, P):
        NeighborPts = []
        for i in self.dataset:
            if self.get_dist(i, self.dataset[P]) < self.EPSILON:
                NeighborPts += i
        return NeighborPts

