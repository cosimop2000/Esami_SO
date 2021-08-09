#!/bin/sh

cd $1

#variabile per il numero di byte
count=0

#variabile che contiene i nomi assoluti dei file
files=

#variabile booleana
trovato=0


for i in *
do
	if test -f $i 
	then
		
		count=`wc -c < $i`
		
		if test $count -le $2 -a $count -gt 0
		then
			
		trovato=1		
		files="$files `pwd`/$i"
		files="$files $count"	
				
		fi
		
	fi
	
	
done


if test $trovato -eq 1
then
	echo `pwd` 
	echo file trovati $files
	
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


