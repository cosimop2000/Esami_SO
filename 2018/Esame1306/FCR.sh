#!/bin/sh

cd $1

#variabile booleana
trovato=0

#variabile per contare le linee
count=0


case $1 in

*/$2)

for i in *
do
	if test -f $i 
	then
		count=`wc -l < $i`
		
		if test $count -eq $3 	
		then
		
			trovato=1
			
			echo `pwd`/$i >> $4
		
		fi
	fi
	
done

if test $trovato -eq 1
then
	echo trovata directory `pwd`
	
fi

;;

*);;
esac


#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 $3 $4
	fi
done


