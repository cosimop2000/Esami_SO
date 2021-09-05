#!/bin/sh

cd $1

#variabile per contare i file
cont=0

#variabile per contare le linee
count=0

#variabile per i nomi assoluti dei file
files=

for F in *
do

	if test -f $F
	then
		count=`wc -l < $F`
		
		if test $count -eq $2
		then
			case $F in
			??)
			cont=`expr $cont + 1`
			files="$files `pwd`/$F"
			;;
			*)
			;;
			esac
		
		fi

	fi


done

if test $cont -ge 2 -a $cont -lt $3
then

	echo trovata directory `pwd`
	echo $files
	echo invocazione parte C
	
	main $files

fi

#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 $3 
	fi
done


