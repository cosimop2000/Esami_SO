#!/bin/sh

cd $1

#variabile contatore per le linee dei file
count=0

case $1 in 
*/$2)

for i in *
do
	if test -f $i -a -r $i
	then
	
		count=`wc -l < $i`
		
		if test $count -gt 4
		then
			echo `pwd`/$i $count >> $3	
			
		fi	
	
	fi
	
done
;;

*);;	

esac

#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 $3 
	fi
done


