#!/bin/sh

cd $1

#variabile booleana
trovato=0

#variabile per contare le linee
count=0

#variabile che contiene i nomi dei file creati
files=

for i in *
do
	if test -f $i 
	then
		count=`wc -l < $i`
		
		if test $count -gt $2 	
		then
		
		trovato=1
		
		files="$files $i.testa"
		
		head -$2 $i > $i.testa
		
		fi
	fi
	
	
done


if test $trovato -eq 1 
then
	echo `pwd`
	
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


