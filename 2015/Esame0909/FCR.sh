#!/bin/sh

cd $1

#variabile per il numero di bytes
count=0

#variabile booleana per le directory
trovato=0

#variabile per i nomi assoluti dei file leggibili
files=

#variabile per individuare almeno 2 file
k=0

for i in *
do
	
		if test -f $i -a -r $i
		then
			count=`wc -c < $i`
			
			if test $count -eq $3
			then
			
				trovato=1
				k=`expr $k + 1`
				files="$files `pwd`/$i"
			
			fi
		
		fi

done

if test $trovato -eq 1 -a $k -ge 2
then
	echo `pwd`
	echo nomi assoluti dei file
	echo $files
	echo invocazione parte C
	main $files $2
fi


#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 $3
	fi
done


