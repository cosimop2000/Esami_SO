#!/bin/sh

cd $1

for i in *
do

	
	if test -f $i -a -r $i
	then
		
		if grep $2 $i > /dev/null
		then
			echo `pwd`/$i
			
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


