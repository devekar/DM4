#include<fstream>
#include<iostream>
#include<string>
#include<sstream>
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<map>
#include<list>
#include<algorithm>
#include<math.h>
#include<ctime>

#define MAX_DIST 20000

using namespace std;

list<vector<int> > clusterList;

char *DISTANCEFILE;
int MINPTS;
float EPSILON;
int rows;
float *matrix = NULL;
clock_t t1, t2, t3, t4;

// Allocated memory for distance matrix based on rows read from data file
void initialize()
{		
	matrix = new float[(rows*(rows-1))/2];
	if( !matrix ) cout<<"Error"<<endl;
	 
	cout<<"Number of rows: "<<rows<<" "<<endl;
}

// read the distance matrix from file in binary format
void readDistanceMatrix(int idx)
{
	FILE* f = fopen(DISTANCEFILE,"rb");
	int ok = fread(matrix, sizeof(float),idx, f);
	fclose(f);
	if(ok==idx) cout<<"Success binary read\n";
}

// return distance stored in distance matrix
inline float getDistance(int P1, int P2)
{
	if(P1==P2) return 0;
	else if(P1>P2) return matrix[(P1*(P1-1))/2 + P2];
	else return matrix[(P2*(P2-1))/2 + P1];
}


// Find the pair of records(clusters) with minimum distance
// P1 is larger than P2
inline void find_min_distance(int *points)
{
 int i,j, idx=0;
 float min_t = MAX_DIST;
 for(i=0;i<rows;i++)
   for(j=0;j<i;j++, idx++)
     if(matrix[idx] < min_t) {
		min_t=matrix[idx];
		points[0] = i; points[1]=j;
	}
}

// Update distance
inline void setDistance(int P1, int P2, float dist)
{
  if(P1==P2) return;
  else if(P1>P2) matrix[(P1*(P1-1))/2 + P2] = dist;
  else matrix[(P2*(P2-1))/2 + P1] = dist;
}

// Initialize list of clusters
void constructListOfvectors()
{
 for(int i=0; i <rows; i++) {
	vector<int> t;
	t.push_back(i);
    clusterList.push_back(t);
 }
}


// Update list of clusters when combining 2 clusters
void updateList(int P1, int P2)
{
  list<vector<int> >::iterator P1_it;
  list<vector<int> >::iterator P2_it;
  for(list<vector<int> >::iterator listit=clusterList.begin(); listit != clusterList.end(); listit++) {
     vector<int> v = *(listit);
	 if(find(v.begin(), v.end(), P1)!=v.end()) P1_it = listit;
	 if(find(v.begin(), v.end(), P2)!=v.end()) P2_it = listit;
  }
  
  (*P2_it).insert( (*P2_it).end(), (*P1_it).begin(), (*P1_it).end() );
  clusterList.erase(P1_it);
}

// P1 is larger than P2
// Keep the link's min/max distances in P2 
void updateMatrixAndStatus(int *points)
{
  int P1 = points[0], P2=points[1];
  float new_dist;
  for(int i=0; i<rows; i++) {
    new_dist = max( getDistance(i, P1), getDistance(i, P2) );  //choose min/max link
	setDistance(i, P2, new_dist);
  }
  
  //hide P1's distances
  for(int i=0;i <rows; i++)
    setDistance(i, P1, MAX_DIST);
	
  updateList(P1, P2);
}


// Display clusters
void computeHierarchicalCluster(int C)
{
	stringstream s; s<<C;
	string fname = DISTANCEFILE + s.str() + ".cluster";
	cout<<fname<<endl;
	ofstream of(fname.c_str());
	int c=0;
	for(list<vector<int> >::iterator listit=clusterList.begin(); listit != clusterList.end(); listit++) {
		for(int i=0; i< (*listit).size(); i++)
			of<<(*listit)[i]<<" ";
		of<<endl;
		c++;
	}
	

	clock_t t = clock();
    double elapsed = double(t - t3) / CLOCKS_PER_SEC;
	cout<<"Clustering time for "<<C<<" clusters: "<<elapsed<<endl;
}

// Main function for Hierarchical Clustering
void hierarchical()
{
 constructListOfvectors();

 int currentC = rows;
 int *points=new int[2];
 while(1) {
	find_min_distance(points);
	updateMatrixAndStatus(points);
	currentC--;
	if(currentC==256 || currentC==128 || currentC==64 || currentC==32 || currentC==16 || currentC==8 || currentC==4) 
		computeHierarchicalCluster(currentC);
	if(currentC==4) break;
 }

}






int main(int argc, char* argv[])
{
    if(argc<2) {
        cout<<"Incorrect Args\n";
		cout<<"Usage: <Distance-matrix from Stage 2>";
        exit(1);
    }

	DISTANCEFILE = argv[1];
	rows = 11305; //atoi(argv[1]);
	
    t1 = clock();

	initialize();
    t2 = clock();
    double elapsed = double(t2-t1) / CLOCKS_PER_SEC;
    cout<<"Initialization time: "<<elapsed<<endl;
	
	int idx = (rows*(rows-1))/2;
	readDistanceMatrix(idx);
	cout<<"Min distance: "<<*min_element(matrix, matrix + idx)<<endl;
	cout<<"Max distance: "<<*max_element(matrix, matrix + idx)<<endl;

    t3 = clock();
    elapsed = double(t3-t2) / CLOCKS_PER_SEC;
    cout<<"DistanceMatrix Read time: "<<elapsed<<endl;
	
	hierarchical();
    t4 = clock();
    elapsed = double(t4 - t1) / CLOCKS_PER_SEC;
    cout<<"Total time: "<<elapsed<<endl;

	return 0;
}
