#!/bin/sh

cd $1

#variabile per contare il numero di file
count=0

#variabile che contiene i nomi assoluti dei file
files=

for i in *
do
	if test -f $i -a -r $i
	then
		count=`expr $count + 1`
		files="$files `pwd`/$i"
	fi	
	
done

if test $count -ge $2 -a $count -le $3
then
	echo trovata directory `pwd`

	echo trovati $count files
	
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


