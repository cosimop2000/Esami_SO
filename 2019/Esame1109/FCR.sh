#!/bin/sh

cd $1

#variabile per contare i file
nfile=0

#variabile per contare la lunghezza in caratteri
count=0

#variabili per file pari e dispari
pari=
dispari=

for i in *
do
	if test -f $i 
	then
		count=`wc -c < $i`
		
		if test $count -eq $3 	
		then
			nfile=`expr $nfile + 1`
			
			if test `expr $nfile % 2` -eq 1
			then
			dispari="$dispari `pwd`/$i"		
			
			
			else
			pari="$pari `pwd`/$i"		
			
			fi

		fi
		

	fi
	
	
done

if test $nfile -eq $2
then
	echo trovata directory `pwd`
	
	echo invocazione 1 parte C
	echo $dispari
	main $dispari
	
	echo invocazione 2 parte C
	echo $pari
	main $pari
	

fi


#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 $3
	fi
done


