copy:
	cp /home/8/devekar/WWW/DM2/data_matrix.csv ./

run_kmeans:
	python KmeansMain.py 4 0

run_maxlink:
	make run_maxlink1
	make run_maxlink2
	make run_maxlink3
	make run_maxlink4
	
run_maxlink1:
	python Stage1_ParseDM.py

run_maxlink2:	
	g++ -o Stage2 Stage2_DistanceMatrix.cpp
	./Stage2 CPPmatrixActual.txt cosine
	
run_maxlink3:
	g++ -o Stage3 Stage3_MaxLink.cpp
	./Stage3 cosine.bin
	
run_maxlink4:
	mkdir cluster_dir
	mv *.cluster cluster_dir
	python Stage4_ComputeEntropy.py cluster_dir
	
	
clean:
	rm -rf *.pyc Stage2 Stage3 *.cluster cluster_dir *.txt *.csv

