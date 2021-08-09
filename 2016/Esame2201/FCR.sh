#!/bin/sh

cd $1

#variabile per il numero di file
count=0

#variabile booleana per le directory
trovato=0

#variabile per vedere se ci sono sottodirectory
x=1

#variabile per i nomi assoluti dei file leggibili
files=

for i in *
do

		if test -f $i -a -r $i
		then
			trovato=1
			files="$files `pwd`/$i"
			count=`expr $count + 1`
		fi

		if test -d $i
		then
			x=0
		fi	
		
done

if test $trovato -eq 1 -a $x -eq 1 
then

	if test $count -eq $2
	then
	
	echo `pwd`
	echo nomi assoluti dei file
	echo $files
	echo invocazione parte C
	main $files
	
	fi
fi


#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 
	fi
done


