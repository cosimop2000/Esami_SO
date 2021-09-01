#!/bin/sh

cd $1

#variabile contatore per le linee dei file
count=0


for i in *
do
	if test -f $i -a -r $i
	then
	
	if test $i = $2
	then
	
		count=`wc -l < $i`
		
		if test $count -ge 4
		then
			echo `pwd`/$i $count >> $3	
			
		fi	
	

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


