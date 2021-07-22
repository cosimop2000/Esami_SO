#!/bin/sh

cd $1

#variabile che contiene la lunghezza in linee del file
files=0

for i in *
do
	if test -f $i -a -r $i
	then
	files=`wc -l < $i`
		if test $files -ge $2
		then
#stampo il nome del file sul file tmp per invocazione parte C
			echo `pwd`/$i >> $3
		fi
	
	fi
done



#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 $3
	fi
done


