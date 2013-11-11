copy:
	cp /home/8/devekar/WWW/DM2/data_matrix.csv ./
	cp -r /home/8/devekar/WWW/DM2/reuters ./

run_preprocess:
	python DM1.py

run:
	python KmeansMain.py 4 0

clean:
	rm *.pyc
