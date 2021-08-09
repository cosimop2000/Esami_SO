#!/bin/sh

cd $1

#variabile per il numero di linee
count=0

#variabile che indica se esistono sottodirectory
dir=0

#variabile contatore
cnt=0

#varaibile che conta tutte le cose allo interno di $1
k=0

for i in *
do
	if test -f $i 
	then
		
		count=`wc -l < $i`
		
		if test $count -ge $2
		then
			
		cnt=`expr $cnt + 1`		
				
		fi
		
	fi
	
	if test -d $i
	then
		dir=1
	fi

k=`expr $k + 1`
	
done


if test $dir -eq 0 -a $cnt -eq $k
then
	echo `pwd` >> $3
fi


#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 $3
	fi
done


