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

#define MAX_DIST 2

using namespace std;
list<vector<int> > clusterList;


int MINPTS;
float EPSILON;
char FILENAME[] = "CPPmatrix1.txt";
int rows, cols, word_size, topic_size;
int *dataset= NULL;
float *magnitudes = NULL;
float *matrix = NULL;
float *matrix1 = NULL;
int *status=NULL;

void parse()
{
	string line;
	ifstream infile(FILENAME);
	getline(infile, line);
	rows = atoi(line.c_str());
	
	getline(infile, line);
	word_size = cols = atoi(line.c_str());
	
	getline(infile, line);
	topic_size = atoi(line.c_str());
	
	dataset = new int[rows*cols];
    status = new int[rows];
	magnitudes = new float[rows];
	matrix = new float[(rows*(rows-1))/2];
	if(!dataset || !status || !magnitudes || !matrix) cout<<"Error"<<endl;
		
	for(int i=0;i<rows;i++) status[i]=0;
	 
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



inline float getMagnitude(int row)
{
	float sum=0;
	for(int j=0; j<cols; j++)
		sum += dataset[row*cols + j] * dataset[row*cols + j];
	return sqrt(sum);
}

void computeMagnitudes()
{
	for(int i=0;i < rows; i++) {
        magnitudes[i] = getMagnitude(i);
	}
}

inline float getCosineDist(int P1, int P2)
{
  float dotprod = 0;
  for(int j=0; j<cols; j++)
    dotprod += dataset[P1*cols + j]*dataset[P2*cols + j];
	
  return (1 - dotprod/(magnitudes[P1]*magnitudes[P2]));
}


void write_binary(int idx)
{
FILE *f =fopen("binmat.txt","wb");
int ok = fwrite(matrix, sizeof(float),idx, f);
fclose(f);
if(ok==idx) cout<<"Success binary write\n";

matrix1 = new float[idx];
f = fopen("binmat.txt","rb");
ok = fread(matrix1, sizeof(float),idx, f);
fclose(f);
if(ok==idx) cout<<"Success binary read\n";

int i;
for(i=0;i<idx;i++)
if(matrix[i]!=matrix[i]) break;

if(i==idx) cout<<"Same copy\n";
}


void parseBinMatrix(int idx)
{
FILE* f = fopen("binmat.txt","rb");
int ok = fread(matrix, sizeof(float),idx, f);
fclose(f);
if(ok==idx) cout<<"Success binary read\n";
}

void computeDistances()
{
  int j, idx=0; 
  for(int i=0; i< 1000; i++){
    for(j=0; j<i; j++) {
	   matrix[idx++]=getCosineDist(i, j);
	}
  }
  cout<<"Computed distances: "<<idx<<endl;
}

float getDistance(int P1, int P2)
{
  if(P1==P2) return 0;
  else if(P1>P2) return matrix[(P1*(P1-1))/2 + P2];
  else return matrix[(P2*(P2-1))/2 + P1];
}




/*
HIERARCHICAL
*/
// P1 is larger than P2
void find_min_distance(int *points)
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


void setDistance(int P1, int P2, float dist)
{
  if(P1==P2) return;
  else if(P1>P2) matrix[(P1*(P1-1))/2 + P2] = dist;
  else matrix[(P2*(P2-1))/2 + P1] = dist;
}

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


void constructListOfvectors()
{
 for(int i=0; i <rows; i++) {
	vector<int> t;
	t.push_back(i);
    clusterList.push_back(t);
 }
}

void computeHierarchicalCluster()
{
	int c=0;
	for(list<vector<int> >::iterator listit=clusterList.begin(); listit != clusterList.end(); listit++) {
		cout<<c<<" "<<(*listit).size()<<endl;
		c++;
	}
}


void hierarchical(int C)
{
constructListOfvectors();

 if(C<2) {
	cout<<"Wrong cluster size\n";
	return;
 }
 int currentC = rows;
 int *points=new int[2];
 while(1) {
	find_min_distance(points);
	updateMatrixAndStatus(points);
	currentC--;
	if(currentC==C) break;
	if(currentC%50==0) cout<<currentC<<" ";
 }
 cout<<endl;
 computeHierarchicalCluster();
}


int main(int argc, char* argv[])
{
	/*
    if(argc<3) {
        cout<<"Incorrect Args\n";
        exit(1);
    }
    EPSILON = atof(argv[1]);
    MINPTS = atoi(argv[2]);
    cout<<EPSILON<<" "<<MINPTS<<endl;
    */
	int rows1 = atoi(argv[1]);
    clock_t t1 = clock();

	parse();
    clock_t t2 = clock();
    double elapsed = double(t2-t1) / CLOCKS_PER_SEC;
    cout<<"Parse: "<<elapsed<<endl;

    computeMagnitudes();
    clock_t t3 = clock();
    elapsed = double(t3-t2) / CLOCKS_PER_SEC;
    cout<<"Magnitudes: "<<elapsed<<endl;

	int idx = (rows*(rows-1))/2;
    computeDistances();
	cout<<*min_element(matrix, matrix + idx)<<endl;
	cout<<*max_element(matrix, matrix + idx)<<endl;
	write_binary(idx);
    clock_t t4 = clock();
    elapsed = double(t4-t3) / CLOCKS_PER_SEC;
    cout<<"Distances: "<<elapsed<<endl;

	//parseBinMatrix(idx);
	//rows = rows1;
	//hierarchical(64);
    clock_t t5 = clock();
    elapsed = double(t5-t4) / CLOCKS_PER_SEC;
    cout<<"Algo: "<<elapsed<<endl;

	return 0;
}
