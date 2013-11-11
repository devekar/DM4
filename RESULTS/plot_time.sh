set title "Running time plot for number of clusters"
set xlabel "Number of clusters"
set ylabel "Running Time (seconds)"
set key right bottom

set terminal png size 800,600 enhanced font "Helvetica,20"
set output "Kmeans-Time.png"

plot "cluster_time.txt" using 1:2 title 'Cosine Distance' with lines,\
     "cluster_time.txt" using 1:3 title 'Euclidean Distance' with lines
pause -1
