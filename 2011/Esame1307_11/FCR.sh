#!/bin/sh

cd $1

#variabile che contiene i bytes del file
bytes=0

#variabile booleana
trovato=false

for i in *
do
	if test $trovato != true
	then
	if test -f $i -a -r $i
	then 
		
		bytes=`wc -c < $i`
		
	
		if test $bytes -le $2
		then
			
			if grep $3 $i > /dev/null
			then
				trovato=true
				echo trovata directory `pwd`
				main `pwd`/$i $3
			fi
		fi
	fi
	fi
done




#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 $3 $4 
	fi
done


