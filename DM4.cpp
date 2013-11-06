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

int MINPTS;
float EPSILON;
char FILENAME[] = "CPPmatrix.txt";
int rows, cols;
int *dataset= NULL;
float *magnitudes = NULL;
float *matrix = NULL;
int *status=NULL;

int* parse()
{
	string line;
	ifstream infile(FILENAME);
	getline(infile, line);
	rows = atoi(line.c_str());
	
	getline(infile, line);
	cols = atoi(line.c_str());
	
	dataset = new int[rows*cols];
    status = new int[rows];
	magnitudes = new float[rows];
	matrix = new float[(rows*(rows-1))/2];
	if(!dataset || !status || !magnitudes || !matrix) cout<<"Error"<<endl;
		
	int i = 0;
	while(getline(infile, line))
	{
		stringstream iss(line);
		for(int j=0; j<cols; j++)
			iss>>dataset[i*cols + j];
		i++;
        if(i==rows) break;
	}
	cout<<i<<" "<<rows<<" "<<cols<<endl;
	infile.close();
}


float getMagnitude(int row)
{
	float sum=0;
	for(int j=0; j<cols; j++)
		sum += dataset[row*cols + j] * dataset[row*cols + j];
	return sqrt(sum);
}

void computeMagnitudes()
{
	for(int i=0;i < rows; i++)
        magnitudes[i] = getMagnitude(i);
}

float getCosineDist(int P1, int P2)
{
  float dotprod = 0;
  for(int j=0; j<cols; j++)
    dotprod += dataset[P1*cols + j]*dataset[P2*cols + j];
  if(dotprod==0) return 1;
  return 1 - dotprod/(magnitudes[P1]*magnitudes[P2]);
}

void computeDistances()
{
  int j, idx=0;
  for(int i=0; i< rows; i++){
    for(j=0;j<i;j++) matrix[idx++]=getCosineDist(i, j);
  }
  cout<<"Computed distances: "<<idx<<endl;
}


float getDistance(int P1, int P2)
{
  if(P1==P2) return 0;
  else if(P1>P2) return matrix[(P1*(P1-1))/2 + P2];
  else return matrix[(P2*(P2-1))/2 + P1];
}


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
    if(!status[it]) {
        status[it] = -1;
        vector<int> NeighborPts_t = regionQuery(it);
        if(NeighborPts_t.size() >= MINPTS) {
            vector<int> temp = removeDuplicates(NeighborPts_t, NeighborPts);
            NeighborPts.insert(NeighborPts.end(), temp.begin(), temp.end());
        }
    }
    if(status[it]<1) status[it] = C;
 }
}


void runDBSCAN()
{
 int C=0;
 for(int i=0;i<rows;i++) status[i]=0;

 for(int i=0;i<rows;i++)
 {
    if(status[i]) continue;
    status[i]=-1;
    vector<int> NeighborsPts = regionQuery(i);
    if(NeighborsPts.size() < MINPTS) status[i]=-2;
    else {
        C++;
        if(C%5==0) cout<<C<<endl;
        expandCluster(i, NeighborsPts, C);
    }
 }

}

void computeClusters()
{
    map<int, int> clusters;
    for(int i=0;i<rows;i++)
    {
     clusters[status[i]]++;
    }

    for(map<int,int>::iterator it=clusters.begin(); it != clusters.end(); it++)
      cout<<it->first<<" "<<it->second<<endl;
}


int main(int argc, char* argv[])
{
    if(argc<3) {
        cout<<"Incorrect Args\n";
        exit(1);
    }
    EPSILON = atof(argv[1]);
    MINPTS = atoi(argv[2]);
    cout<<EPSILON<<" "<<MINPTS<<endl;
    
    clock_t t1 = clock();

	parse();
    clock_t t2 = clock();
    double elapsed = double(t2-t1) / CLOCKS_PER_SEC;
    cout<<"Parse: "<<elapsed<<endl;

    computeMagnitudes();
    clock_t t3 = clock();
    elapsed = double(t3-t2) / CLOCKS_PER_SEC;
    cout<<"Magnitudes: "<<elapsed<<endl;

    computeDistances();
    clock_t t4 = clock();
    elapsed = double(t4-t3) / CLOCKS_PER_SEC;
    cout<<"Distances: "<<elapsed<<endl;

    runDBSCAN();
    clock_t t5 = clock();
    elapsed = double(t5-t4) / CLOCKS_PER_SEC;
    cout<<"DBSCAN: "<<elapsed<<endl;

    computeClusters();
	return 0;
}
