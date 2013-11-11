Vaibhav Devekar	(devekar.1@osu.edu)
Akshay Nikam 	(nikam.5@osu.edu)


==========================
Data Mining - Assignment 4
==========================

NOTE: The program depends on feature vectors by Assignment 1 program - DM1.py
      DM1.py makes use of NLTK for stemming, hence cannot be run on STDLINUX. 
      Use "make copy" to get the vectors.  


Executing the program
---------------------
1) Run command "make run_preprocess" to generate the feature vectors.
   OR
   Run command "make copy" to copy the vectors and reuters from ~/home/8/devekar/WWW/DM2 (This will not run DM1.py)
2) Run command "make run" to run the K-means clustering program for 4 clusters
4) Run command "make clean" to remove output files.


With parameters
----------------
Run K-means as follows with parameters:
$python KmeansMain.py <#clusters> <0=cosine dist, 1=euclidean dist>

First argument is the number of clusters, while the second argument takes values 0 or 1 for cosine distance and Euclidean
distance respectively.


Output:
-------
  

                                  


