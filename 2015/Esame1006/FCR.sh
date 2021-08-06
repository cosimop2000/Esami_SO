#!/bin/sh

cd $1

#variabile per il numero di linee
count=0

#variabile booleana per le directory
trovato=0


case $1 in 
*/$2) 
	for j in *
	do
		if test -f $j  
		then
		
		count=`wc -l < $j`
		
		if test $count -eq $3
		then
			
			echo `pwd`/$j >> $4
			trovato=1
				
		fi
		
		fi
	done

	;;
esac

# se non avessi dovuto considerare anche la radice

#for i in *
#do
#	if test -d $i -a $i = $2
#	then
#	
#	cd `pwd`/$2
#	
#	for j in *
#	do
#		if test -f $j  
#		then
#		
#		count=`wc -l < $j`
#		
#		if test $count -eq $3
#		then
#			
#			echo $j >> $4
#			trovato=1
#				
#		fi
#		
#		fi
#	done
#	fi
#done

if test $trovato -eq 1
then
	echo `pwd`
fi


#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 $3 $4
	fi
done


