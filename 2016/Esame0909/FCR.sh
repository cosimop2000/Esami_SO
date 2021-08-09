#!/bin/sh

cd $1

#variabile booleana che indica se vi sono
#file con caratteri non minuscoli
ok=1

#variabile che contiene i nomi assoluti dei file
files=

#variabile booleana
trovato=0


for i in *
do
	if test -f $i 
	then
	
		
		if grep -v [a-z] $i > /dev/null
		then
			
		ok=0		
				
		else
		
		trovato=1
		files="$files `pwd`/$i"
		
		fi
	fi
	
	
done


if test $trovato -eq 1 -a $ok -eq 1
then
	echo `pwd` 
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


