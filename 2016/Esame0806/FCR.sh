#!/bin/sh

cd $1

#variabile per il numero di linee
count=0

#variabile che contiene i nomi assoluti dei file
files=

#variabile contatore
cnt=0


for i in *
do
	if test -f $i 
	then
		
		count=`wc -l < $i`
		
		if test $count -eq $3
		then
			
		cnt=`expr $cnt + 1`		
		files="$files `pwd`/$i"
				
		fi
		
	fi
	
	
done


if test $cnt -ge 2
then
	echo `pwd` 
	echo `pwd` >> $4
	echo $files >> $5
fi


#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 $3 $4 $5
	fi
done


