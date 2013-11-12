#include<fstream>
#include<iostream>
#include<string>
#include<sstream>
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<map>
#include<algorithm>
#include<math.h>
#include<ctime>

using namespace std;


char *DISTANCEFILE;
int MINPTS;
float EPSILON;
int rows, cols, word_size, topic_size;
float *matrix = NULL;
int *status=NULL;


// Allocated memory for distance matrix based on rows read from data file
void initialize()
{		
	matrix = new float[(rows*(rows-1))/2];
	if( !matrix ) cout<<"Error"<<endl;

	cout<<"Number of rows: "<<rows<<" "<<endl;
}



void readDistanceMatrix(int idx)
{
	FILE* f = fopen(DISTANCEFILE,"rb");
	int ok = fread(matrix, sizeof(float),idx, f);
	fclose(f);
	if(ok==idx) cout<<"Success binary read\n";
}



float getDistance(int P1, int P2)
{
  if(P1==P2) return 0;
  else if(P1>P2) return matrix[(P1*(P1-1))/2 + P2];
  else return matrix[(P2*(P2-1))/2 + P1];
}



/*
DBSCAN
*/
vector<int> regionQuery(int P)
{
 vector<int> NeighborPts;
 for(int i=0;i<rows;i++)
    if(getDistance(i, P) < EPSILON) NeighborPts.push_back(i);
 return NeighborPts;
}

vector<int> removeDuplicates(vector<int> NeighborPts_t, vector<int> NeighborPts)
{
 vector<int> temp;
 for(int it = 0; it < NeighborPts_t.size(); it++)
    if(std::find(NeighborPts.begin(), NeighborPts.end(), NeighborPts_t[it])==NeighborPts.end()) 
        temp.push_back(NeighborPts_t[it]);

 return temp;
}


void expandCluster(int P, vector<int> NeighborPts, int C)
{
 status[P] = C;
 for(int it=0; it< NeighborPts.size(); it++)
 {
    if(status[ NeighborPts[it] ]==0) {
        status[ NeighborPts[it] ] = -1;
        vector<int> NeighborPts_t = regionQuery(NeighborPts[it]);
		
        if(NeighborPts_t.size() >= MINPTS) {
            vector<int> temp = removeDuplicates(NeighborPts_t, NeighborPts);
            NeighborPts.insert(NeighborPts.end(), temp.begin(), temp.end());
        }
    }
	
    if(status[ NeighborPts[it] ]<1) status[ NeighborPts[it] ] = C;
 }
 
}


void runDBSCAN()
{
 int C=0;
 status = new int[rows];
 for(int i=0; i<rows; i++) status[i]=0;
	
 for(int i=0;i<rows;i++)
 {
    if(status[i]!=0) continue;
    status[i]=-1;
    vector<int> NeighborPts = regionQuery(i);
    if(NeighborPts.size() < MINPTS) status[i]=-2;
    else {
        C++;
		cout<<C<<" ";
        expandCluster(i, NeighborPts, C);
    }
 }
 cout<<endl;

}


void computeClusters()
{
    map<int, int> clusters;
    for(int i=0;i<rows;i++) clusters[status[i]]++;
    
    for(map<int,int>::iterator it=clusters.begin(); it != clusters.end(); it++)
      cout<<it->first<<" "<<it->second<<endl;
}



int main(int argc, char* argv[])
{
	
    if(argc<4) {
        cout<<"Incorrect Args\n";
        exit(1);
    }
	DISTANCEFILE = argv[1];
    EPSILON = atof(argv[2]);
    MINPTS = atoi(argv[3]);
	rows = 11305; //atoi(argv[4]);
	
    cout<<EPSILON<<" "<<MINPTS<<endl;
   
    clock_t t1 = clock();

	initialize();
    clock_t t2 = clock();
    double elapsed = double(t2-t1) / CLOCKS_PER_SEC;
    cout<<"Dataset: "<<elapsed<<endl;

	int idx = (rows*(rows-1))/2;
	readDistanceMatrix(idx);
	cout<<*min_element(matrix, matrix + idx)<<endl;
	cout<<*max_element(matrix, matrix + idx)<<endl;
    clock_t t3 = clock();
    elapsed = double(t3-t2) / CLOCKS_PER_SEC;
    cout<<"DistanceMatrix: "<<elapsed<<endl;

	//rows = 10000;
    runDBSCAN();
    clock_t t4 = clock();
    elapsed = double(t4-t3) / CLOCKS_PER_SEC;
    cout<<"DBSCAN: "<<elapsed<<endl;

    computeClusters();

	return 0;
}
