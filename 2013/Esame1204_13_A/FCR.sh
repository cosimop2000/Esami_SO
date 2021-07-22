#!/bin/sh

cd $1

#variabile booleana
trovato=0

#variabile che contiene il nome assoluto dei file cercati in un direttorio
file=



for i in *
do
	if test -f $i -a -r $i 
	then
		
		case $i in
		*.$2) trovato=1 
		      file="$file `pwd`/$i"
			;;
		esac
		
		
		
	fi
	
	
	
done


if test $trovato -eq 1 
then
	echo `pwd` >> $5
	count=`wc -l < $5`
	
	if test $4 = B 
	then
	
	if test $count -eq $6
	then
		for j in $file
		do
			echo nome assoluto $j
			head -1 < $j 
			
		done
	fi
	fi
fi



#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 $3 $4 $5 $6
	fi
done


