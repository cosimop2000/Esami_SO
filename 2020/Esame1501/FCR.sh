#!/bin/sh

cd $1

#variabile per contare la lunghezza in caratteri
count=0

#variabile booleana
trovato=0

for i in *
do
	if test -f $i 
	then
		count=`wc -c < $i`
		
		if test $count -eq $2 	
		then
			trovato=1
			echo `pwd`/$i 
			echo `pwd`/$i >> $3
		fi
		

	fi
	
	
done

if test $trovato -eq 1
then
	echo trovata directory `pwd`

fi


#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 $3
	fi
done


