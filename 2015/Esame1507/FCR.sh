#!/bin/sh

cd $1

#variabile per il numero di linee
count=`expr $3 + 1`

#variabile booleana per le directory
trovato=0

#variabile per i nomi assoluti dei file leggibili
files=

for i in *
do
	if test $count -eq $2
	then
		trovato=1
		if test -f $i -a -r $i
		then
			files="$files `pwd`/$i"
		fi
	fi



done

if test $trovato -eq 1
then
	echo `pwd`
	echo nomi assoluti dei file
	echo $files
	echo invocazione parte C
	main $files
fi


#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 $count
	fi
done


