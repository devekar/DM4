#include<fstream>
#include<iostream>
#include<string>
#include<sstream>
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<list>
#include<algorithm>
#include<math.h>
#include<string.h>
#include<ctime>

using namespace std;

char *DATAFILE;
char *DISTANCEFILE;
int rows, cols, word_size, topic_size;
int *dataset= NULL;
float *magnitudes = NULL;
float *matrix = NULL;

// Read the data matrix(only the word features, not the labels)
void parse()
{
	string line;
	ifstream infile(DATAFILE);
	getline(infile, line);
	rows = atoi(line.c_str());
	
	getline(infile, line);
	word_size = cols = atoi(line.c_str());
	
	getline(infile, line);
	topic_size = atoi(line.c_str());
	
	dataset = new int[rows*cols];
	magnitudes = new float[rows];
	matrix = new float[(rows*(rows-1))/2];
	if(!dataset || !magnitudes || !matrix) cout<<"Error"<<endl;
	 
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


// Magnitude used in cosine calculation
inline float getMagnitude(int row)
{
	float sum=0;
	for(int j=0; j<cols; j++)
		sum += dataset[row*cols + j] * dataset[row*cols + j];
	return sqrt(sum);
}

// Compute magnitudes for all records
void computeMagnitudes()
{
	for(int i=0;i < rows; i++) 
        magnitudes[i] = getMagnitude(i);
}

// Return cosine distance between 2 record
inline float getCosineDist(int P1, int P2)
{
  float dotprod = 0;
  for(int j=0; j<cols; j++)
    dotprod += dataset[P1*cols + j]*dataset[P2*cols + j];
	
  return (1 - dotprod/(magnitudes[P1]*magnitudes[P2]));
}

// Return euclidean distance between 2 record
inline float getEuclidean(int P1, int P2)
{
 float sum = 0;
  for(int j=0; j<cols; j++)
    sum += (dataset[P1*cols + j] - dataset[P2*cols + j])*(dataset[P1*cols + j] - dataset[P2*cols + j]);
	
  return sqrt(sum);
}

// Write the distance matrix to file in binary
void write_binary(int idx)
{
FILE *f =fopen(DISTANCEFILE,"wb");
int ok = fwrite(matrix, sizeof(float),idx, f);
fclose(f);
if(ok==idx) cout<<"Success binary write\n";

float *matrix1 = new float[idx];
f = fopen(DISTANCEFILE,"rb");
ok = fread(matrix1, sizeof(float),idx, f);
fclose(f);
if(ok==idx) cout<<"Success binary read\n";

int i;
for(i=0;i<idx;i++)
if(matrix1[i]!=matrix[i]) break;

if(i==idx) cout<<"Same copy\n";
}


// Compute lower triangular distance matrix, also excludes diagonal
void computeDistancesCosine()
{
  int j, idx=0; 
  for(int i=0; i< rows; i++)
    for(j=0; j<i; j++) 
	   matrix[idx++]=getCosineDist(i, j);
	
  cout<<"Computed distances: "<<idx<<endl;
}


void computeDistancesEuclidean()
{
  int j, idx=0; 
  for(int i=0; i< rows; i++)
    for(j=0; j<i; j++) 
	   matrix[idx++]=getEuclidean(i, j);
	
  cout<<"Computed distances: "<<idx<<endl;
}


int main(int argc, char* argv[])
{
    if(argc<3) {
        cout<<"Incorrect Args\n";
		cout<<"Usage:  <DataMatrix produced from Stage 1> <metric=cosine/euclidean>";
        exit(1);
    }
	DATAFILE = argv[1];
	char *metric = argv[2];
	if (!strcmp(metric, "cosine")) DISTANCEFILE = "Cosine.bin";
	else DISTANCEFILE = "Euclidean.bin";
	
	
    clock_t t1 = clock();

	parse();
    clock_t t2 = clock();
    double elapsed = double(t2-t1) / CLOCKS_PER_SEC;
    cout<<"Parse: "<<elapsed<<endl;

	if (!strcmp(metric, "cosine")) computeMagnitudes();
    clock_t t3 = clock();
    elapsed = double(t3-t2) / CLOCKS_PER_SEC;
    cout<<"Magnitudes: "<<elapsed<<endl;
	
	int idx = (rows*(rows-1))/2;
	if (!strcmp(metric, "cosine")) computeDistancesCosine();
	else computeDistancesEuclidean();
    clock_t t4 = clock();
    elapsed = double(t4-t3) / CLOCKS_PER_SEC;
    cout<<"Distances: "<<elapsed<<endl;
	
	cout<<*min_element(matrix, matrix + idx)<<endl; //Min of the distances
	cout<<*max_element(matrix, matrix + idx)<<endl; //Max of the distances
	write_binary(idx);

	return 0;
}
