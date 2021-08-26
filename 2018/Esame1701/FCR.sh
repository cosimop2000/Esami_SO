#!/bin/sh

cd $1

#variabile per il numero di linee
count=

#variabile per il numero di file
nfile=0

#variabile che contiene i nomi assoluti dei file trovati 
files=


for i in *
do
	if test -f $i  
	then
	
		count=`wc -l < $i`
		
		if test $count -eq $3
		then
			
			if grep $4 $i > /dev/null 
			then
			
				nfile=`expr $nfile + 1`
				files="$files `pwd`/$i"
			
			fi
				
		fi
	fi	
	
done

if test $nfile -eq $2
then
	echo trovata directory `pwd`
	echo invocazione C
	
	main $files $4 $3
		
fi



#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 $3 $4
	fi
done


