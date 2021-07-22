#!/bin/sh

cd $1

#variabile che contiene le linee dei file
lines=0

#variabile che contiene il numero dei file
files=0

#variabile che contiene i nomi dei file
filenames=

for i in *
do
	if test -f $i -a -r $i
	then 
		files=`expr $files + 1`
		lines=`wc -l < $i`
		
	
		if test $files -eq $2 -a $lines -eq $3
		then
			
			if grep $4 $i > /dev/null
			then
				filenames="$filenames $i"
				echo trovata directory `pwd`
				main `pwd`/$i $4 $3
			fi
		fi
	fi

done




#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 $3 $4 
	fi
done


