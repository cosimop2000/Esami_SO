#!/bin/sh

cd $1

#variabile booleana
trovato=0

#variabile che contiene il nome assoluto dei file
files=

#variabile per il numero di linee
count=

#variabile per il numero di linee con il grep
lines=


for i in *
do
	if test -f $i -a -r $i 
	then
		count=`wc -l < $i`
		
		if test $count -ne 0
		then
		
		lines=`grep '^a' $i | wc -l`
		
		if test $count -eq $lines
		then
			trovato=1
			files="$files `pwd`/$i"
		fi		
		
		
		fi
		
	fi
	
	
	
done


if test $trovato -eq 1 
then
	echo $files >> $2
fi



#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 
	fi
done


