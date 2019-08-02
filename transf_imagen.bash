#!/bin/bash
cd ./$1

for i in `ls Estado_*.dat`;
do 	
	echo "set terminal png size 800,800">>plot.gp
	echo "set view 75,45">>plot.gp
	echo "set output \"$(basename "$i" .dat ).png\"">>plot.gp
	echo "set xrange["$2":"$3"]">>plot.gp
	echo "set yrange["$4":"$5"]">>plot.gp
	echo "set zrange["$6":"$7"]">>plot.gp
	echo "splot'$i' with pm3d,\"`ls datosjuntos.dat`\" ps 1 pt 7 palette notitle">>plot.gp 
	gnuplot < plot.gp
	rm plot.gp
	echo $i
	rm $i
done

convert -delay 10 -loop 0 *.png Evolucion.gif

echo "set terminal png size 800,600">>plot.gp
echo "set xlabel \"Iteraciones\"">>plot.gp
echo "set ylabel \"Error\"">>plot.gp
echo "set output \"error.png\"">>plot.gp
if [ -e error_try.dat ]
then
    echo "plot \"error_train.dat\" w l lw 2 title \"Train\",\"error_try.dat\" w l lw 2 title \"Try\"">>plot.gp
else
    echo "plot \"error.dat\" w l lw 2 notitle">>plot.gp
fi
gnuplot < plot.gp
rm plot.gp

rm Estado*
exit 0
