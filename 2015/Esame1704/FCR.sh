#!/bin/sh

cd $1

#variabile per il numero di linee
count=0


for i in *
do
	if test -f $i -a -r $i 
	then
		
		count=`grep 't$' $i | wc -l`
		
		if test $count -ge $2
		then
			
			echo `pwd`/$i >> $3
				
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


