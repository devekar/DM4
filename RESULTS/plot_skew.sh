set title "Skew for K-means"
set xlabel "Number of clusters"

set terminal png size 800,600 enhanced font "Helvetica,20"
set output "Kmeans-skew.png"

plot "skew_mean_sd.txt" using 1:2 title 'Means' with lines,\
     "skew_mean_sd.txt" using 1:3 title 'Std-Dev(Cosine)' with lines,\
     "skew_mean_sd.txt" using 1:5 title 'Std-Dev(Euclidean)' with lines


set title "Skew for Max-Link"
set xlabel "Number of clusters"

set terminal png size 800,600 enhanced font "Helvetica,20"
set output "Maxlink-skew.png"

plot "maxlink_mean_sd.txt" using 1:2 title 'Means' with lines,\
     "maxlink_mean_sd.txt" using 1:3 title 'Std-Dev(Cosine)' with lines,\
     "maxlink_mean_sd.txt" using 1:5 title 'Std-Dev(Euclidean)' with lines
