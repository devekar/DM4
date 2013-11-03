#!/bin/bash

for eps in 0.1 0.2
do
	for minpts in 3 5 7 10
	do
		outfile=DBSCAN_${eps}_${minpts}.output
		command="sbatch -N 1 --output=$outfile ./ri_run.sbatch ${eps} ${minpts}"
		echo $command
		`$command`
	done
done
