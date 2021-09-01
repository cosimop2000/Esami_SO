#!/bin/sh

cd $1

#variabile contatore per le linee dei file
count=0


for i in *
do
	if test -f $i -a -r $i -a -w $i
	then
	
	case $i in 
	$2.txt)	
	
		if test $i.txt = $2 
		then
			count=`wc -c < $i.txt`
			echo `pwd`/$i $count >> $3	
			
		fi
	
	;;
	
	*)
	;;
	
	esac
	
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


