#!/bin/sh

cd $1

#variabile booleana 
trovato=0

#variabile che contiene il nome assoluto dei file cercati in un direttorio
file=

#variabile che contiene il nome relativo semplice dei file
files=

#variabile che indica il numero dei file all'interno della dir
count=0

#variabile che contiene il numero di caratteri
car=

for i in *
do
	trovato=1
	
	if test -f $i -a -r $i 
	then
		car=`wc -c < $i`
		
		if test $car -eq $3
		then
			if grep -v [0-9] $i > /dev/null
			then
				trovato=0		
			fi
			
			if test $trovato -eq 1
			then
				count=`expr $count + 1`
				file="$file `pwd`/$i"
				files="$files $i"
			fi
		fi
	fi
	
done


if test $count -ge $2 
then
	echo `pwd` 
	#main $file
	#main $files
fi



#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 $3 
	fi
done


