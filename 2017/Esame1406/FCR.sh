#!/bin/sh

cd $1

#variabile per il numero di byte
count=

#variabile booleana
trovato=0

#variabile che contiene i nomi assoluti dei file trovati 
files=


for i in *
do
	if test -f $i -a -r $i 
	then
	if test -w $i
	then
	
		count=`wc -c < $i`
		
		if test $count -eq $2
		then
			
		
			if grep $3 $i > /dev/null 
			then
			
				trovato=1
				files="$files `pwd`/$i"
			
			fi
				
		fi
	fi	
	fi
	
done

if test $trovato -eq 1
then
	echo trovata directory `pwd`
	echo $files >> $4
		
fi



#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 $3 $4
	fi
done


