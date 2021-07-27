#!/bin/sh

cd $1

#variabile che contiene il nome assoluto dei file
#files=

#variabile per il numero di linee
count=

#variabile booleana
trovato=0


for i in *
do
	if test -f $i 
	then
		count=`wc -l < $i`
		
		if test $count -ge $2
		then
			trovato=1
			echo `pwd`/$i >> $3
			#files="$files `pwd`/$i"	
		fi
		
	fi
	
done

if test $trovato -eq 1
then
	echo trovata directory `pwd`
	#echo $files >> $3
		
fi



#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 $3
	fi
done


