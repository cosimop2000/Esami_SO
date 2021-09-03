#!/bin/sh

cd $1

#variabile per contare la lunghezza in caratteri
count=0

#variabile per contare i file
nfile=0

#variabili per file pari e dispari
pari=0
dispari=0

#variabili per nomi file pari e dispari
filep=
filed=

for i in *
do
	if test -f $i 
	then
		count=`wc -c < $i`
		
		if test $count -eq $3 	
		then
			pari=`expr $pari + 1`
			
			filep="`pwd`/$i"
			
			
		fi

		if test $count -eq $2
		then
			dispari=`expr $dispari + 1`
			
			filed="`pwd`/$i"
		
		fi
		

	fi
	
	nfile=`expr $nfile + 1`
	
done

if test $pari -eq 1 -a $dispari -eq 1
then
	if test $filep != $filed -a $nfile -eq 2
	then
		echo trovata directory `pwd`
		
		echo $filep >> $4
		echo $filed >> $4
	
	fi


fi


#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 $3 $4
	fi
done


