#!/bin/sh

cd $1

#variabile booleana
trovato=0


for F in *
do

	if test -f $F
	then
		
		case $F in
		*.$2) trovato=1
		;;
		*)
		;;
		esac	
	fi


done

if test $trovato -eq 1
then
	pwd >> $3

fi

#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 $3 
	fi
done


