#!/bin/sh

cd $1

#variabile che contiene i file leggibili
files=0

#variabile che contiene i nomi dei file per invocazione parte C
filenames=


for i in *
do
	if test -f $i -a -r $i
	then
		files=`expr $files + 1`
		filenames="$filename $i"
	fi
done

if test $files -ge $2 -a $files -le $3
then
	echo trovata dir `pwd`
	#main $filename

fi



#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 $3 
	fi
done

