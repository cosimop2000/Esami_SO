#!/bin/sh

cd $1

#variabile che contiene il nome assoluto dei file
files=

#variabile per il numero di linee
count=

for i in *
do
	if test -f $i -a -r $i 
	then
		count=`wc -l < $i`
		
		if test $count -gt $2
		then

		files="$files `pwd`/$i"
		echo $files >> $3
		
		fi
		
	fi
	
done


#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 $3
	fi
done


