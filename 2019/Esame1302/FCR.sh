#!/bin/sh

cd $1

#variabile che contiene i nomi relativi semplici dei file
files=

#variabile contatore
count=0


M=$2

shift
shift


for i in $*
do
	if test -f $i
	then
	
		count=`expr $count + 1`
		files="$files $i"
	
	fi	

done


if test $count -ge $M 
then

	echo trovata directory `pwd`
	echo $files
	
	echo invocazione C
	
	main $files 
		
fi



#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $M $* 
	fi
done


