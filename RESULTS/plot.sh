set title "Entropy plot for number of clusters"
set xlabel "Number of clusters"
set ylabel "Entropy"

set terminal png size 800,600 enhanced font "Helvetica,20"
set output "Kmeans-Entropy.png"

plot "cluster_entropy.txt" using 1:2 title 'Cosine Distance' with lines,\
     "cluster_entropy.txt" using 1:3 title 'Euclidean Distance' with lines
pause -1
