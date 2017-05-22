set terminal png;
set output "pollDist.png";
set datafile sep ";";
unset key
set ylabel "pollen arrival probability";
set xlabel "x in m";
set key right top;
filename(n)=sprintf("pollDist_%d.txt",n);
plot for [i=187:201] filename(i*10) u 1:2 title sprintf("%d",i*10) w l ;