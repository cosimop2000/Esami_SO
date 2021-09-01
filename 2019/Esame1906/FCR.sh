#!/bin/sh

cd $1

#variabile per contare i file
nfile=0

#variabile per contare le linee
count=0

#variabile che contiene i nomi dei file
files=

for i in *
do
	if test -f $i 
	then
		count=`wc -l < $i`
		
		if test $count -lt $2 	
		then
			nfile=`expr $nfile + 1`
			files="$files `pwd`/$i"		

		fi
	fi
	
	
done

if test $nfile -ge 2
then
	echo trovata directory `pwd`
	
	echo invocazione parte C
	
	main $files

fi


#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 
	fi
done


