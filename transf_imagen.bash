#!/bin/bash
cd ./$1

for i in `ls Estado_*.dat`;
do
	echo "set terminal png size 800,800">>plot.gp
	echo "set output \"$(basename "$i" .dat ).png\"">>plot.gp
	echo "set xrange[-1.1:1.1]">>plot.gp
	echo "set yrange[-1.1:1.1]">>plot.gp
	echo "plot \"`ls datos*`\" ps 3 pt 5 palette notitle,'$i' pt 6 palette notitle">>plot.gp 
	gnuplot < plot.gp
	rm plot.gp
	echo $i
	rm $i
done

exit 0
