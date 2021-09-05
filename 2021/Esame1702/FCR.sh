#!/bin/sh

cd $1

#variabile per contare i file
cont=0

#variabile per contare le linee dei file
lines=0

#variabile che contiene i nomi assoluti dei file trovati
files=

for F in *
do
	if test -f $F -a -r $F 
	then
		lines=`wc -l < $F`
		
		if test $lines -eq $3 	
		then
			cont=`expr $cont + 1`
			files="$files `pwd`/$F"
			
		fi



	fi
		
done

if test $cont -ge $2
then
	echo trovata directory `pwd`	
	
	echo invocazione parte C
	
	echo $files
	
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


