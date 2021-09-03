#!/bin/sh

cd $1

#variabile contatore per la dimensione in caratteri
L=0

#file F
F=

#variabile che contiene i nomi assoluti dei file
files=

#variabili booleane
trovato=0
trovata=0



for i in *
do
	if test -f $i 
	then
		
		L=`wc -c < $i`
		
		if test `expr $L % $2` -eq 0 -a $L -ne 0
		then
		
			trovato=1
			files="$files `pwd`/$i"	
	
		fi
	
	fi	
	
	if test -d $i
	then
		trovata=1
	fi

done


if test $trovato -eq 1 -a $trovata -eq 1
then
	echo trovata directory `pwd`

	for F in $files
	do
	
		> $F.Chiara
		
		L=`wc -c < $F`
		
		echo invocazione parte C
		
		main $F $L $2
	
	done

fi


#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 
	fi
done


