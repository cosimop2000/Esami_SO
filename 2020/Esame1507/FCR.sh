#!/bin/sh

cd $1

#variabile per la dimensione in linee
count=0

#variabile per i caratteri
car=


#variabile booleana
trovato=0


for i in *
do
	if test -f $i -a -r $i
	then
		
		count=`wc -l < $i`
				
		car=`wc -c < $i`
		
		if test $count -eq $2
		then
		
		if test `expr $car / $count` -gt 10
		then
		
			trovato=1

			echo `pwd`/$i >> $3
		fi
		fi
	
	fi	
	

done


if test $trovato -eq 1 
then
	echo trovata directory `pwd`

fi


#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 $3
	fi
done


