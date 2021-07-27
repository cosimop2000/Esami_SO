#!/bin/sh

cd $1


#variabile booleana 
trovato=0

#variabile che contiene i nomi assoluti dei file
files=

#variabile che indica la lunghezza in byte
count=

for i in *
do
	if test -f $i
	then
		count=`wc -c < $i`
		if test $count -gt $2
		then
			trovato=1
			files="$files `pwd`/$i"
		fi
	fi
	
done


if test $trovato -eq 1
then
	echo `pwd` 
	echo $files >> $3
fi



#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 $3
	fi
done


