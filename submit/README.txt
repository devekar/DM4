Vaibhav Devekar (devekar.1@osu.edu)
Akshay Nikam (nikam.5@osu.edu)


==========================
Data Mining - Assignment 4
==========================
 

Executing the program
---------------------
1) Run command "make copy" to copy the vectors and reuters from ~/home/8/devekar/WWW/DM2
2) Run command "make run_kmeans" to run the K-means clustering program for 4 clusters with cosine metric.
   OR
   Run command "make run_maxlink" to run the Max-Link clustering program with cosine metric.
4) Run command "make clean" to remove output files.




With parameters
----------------

K-MEANS:
Run K-means as follows with parameters:
$python KmeansMain.py <#clusters> <0=cosine dist, 1=euclidean dist>

First argument is the number of clusters, while the second argument takes values 0 or 1 for cosine distance and Euclidean
distance respectively.


MAX-LINK/DBSCAN:
1) Stage1_ParseDM.py is used to convert the CSV data matrix generated in DM Assignment into space-separated matrix.
   The matrix can be produced in the form of actual values or 1 and 0.
   Commands.:
	python Stage1_ParseDM.py 
	will produce data matrix in file 'CPPmatrixActual.txt' with actual values
	
	python Stage1_ParseDM.py -t
	will produce 1/0 data matrix in file 'CPPmatrixBoolean.txt'
	
	Estimated time: 2 min

	
2) Stage2_DistanceMatrix.cpp is used to generate distance matrix which is written to file(cosine.bin/euclidean.bin) in binary format.
   Commands:
		g++ -o Stage2 Stage2_DistanceMatrix.cpp
		./Stage2 <DataMatrix produced from Stage 1> <metric=cosine/euclidean>
	For eg.
		./Stage2 CPPmatrixActual.txt cosine
		
	Estimated time: 30 min	
	
	
3) Stage3_DM4_MaxLink.cpp generates clusters using Max/Min Link. Clusters are written to files with '.cluster' extension
	Commands:
		g++ -o Stage3 Stage3_MaxLink.cpp	
	    ./Stage3 <Distance-matrix from Stage 2>
	For eg.
	    ./Stage3 cosine.bin"
		
	Estimated time: 50 min


   Stage3_DBSCAN.cpp generates clusters using DBSCAN algorithm.
	Commands:
		g++ -o Stage3 Stage3_DBSCAN.cpp	
	    ./Stage3 <Distance-matrix from Stage 2>
	For eg.
	    ./Stage3 cosine.bin"
	Estimated time: 5 min

	
4) Stage4_ComputeEntropy.py reads the output files from Stage 3 which can contain cluster information for different cluster numbers.
   Copy the output files from Stage 3 to an empty directory.
   Commands:
		mkdir cluster_dir
		mv *.cluster cluster_dir
		python Stage4_ComputeEntropy.py cluster_dir
	
	Estimated time: 1 min
	
	


FILES:
------
1) Makefile
2) Report4.pdf
3) KmeansMain.py
4) Kmeans.py
5) Stage1_ParseDM.py
6) Stage2_DistanceMatrix.cpp
7) Stage3_MaxLink.cpp
8) Stage3_DBSCAN.cpp
9) Stage4_ComputeEntropy.py

 
