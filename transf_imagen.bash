#!/bin/bash
cd ./$1

for i in `ls Estado_*.dat`;
do 	
	echo "set terminal png size 800,800">>plot.gp
	echo "set view 75,45">>plot.gp
	echo "set output \"$(basename "$i" .dat ).png\"">>plot.gp
	echo "set xrange[-1.1:1.1]">>plot.gp
	echo "set yrange[-1.1:1.1]">>plot.gp
	echo "set zrange[-0.1:1.1]">>plot.gp
	echo "splot'$i' with pm3d,\"`ls datosjuntos.dat`\" ps 1 pt 7 palette notitle">>plot.gp 
	gnuplot < plot.gp
	rm plot.gp
	echo $i
	rm $i
done
convert -delay 10 -loop 0 *.png Evolucion.gif
rm Estado*
exit 0
