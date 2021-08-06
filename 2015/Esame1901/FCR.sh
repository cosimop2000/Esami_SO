#!/bin/sh

cd $1


#variabile booleana
trovato=0


for i in *
do
	if test -f $i -a -r $i 
	then
		
		if grep $2 $i > /dev/null
		then
			trovato=1
			echo `pwd`/$i >> $3
				
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


