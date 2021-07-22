#!/bin/sh

cd $1

#numero minimo di file da trovare
M=$2

shift
shift

#variabile booleana 
trovato=0

#variabile che contiene il nome relativo semplice dei file
files=

#variabile che indica il numero dei file all'interno della dir
count=0

for i in *
do
	if test -f $i
	then
		for j in $*
		do
			if test $i = $j
			then
				count=`expr $count + 1`	
				files="$files $i"			
			fi
		
		done
	fi
	
done


if test $count -ge $M
then
	echo $count
	echo `pwd` 
	#main $files
fi



#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $M $*
	fi
done


