#!/bin/sh

cd $1

#variabile booleana
trovato=0

#variabile per contare le linee
count=0


for i in *
do
	if test -f $i 
	then
		count=`wc -l < $i`
		
		if test $count -ge $2 -a $count -lt 255	
		then
		
		trovato=1
		
		echo `pwd`/$i >> $3
		echo $count >> $3
		
		fi
	fi
	
	
done


if test $trovato -eq 1 
then
	echo `pwd`
	
fi


#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 $3
	fi
done


