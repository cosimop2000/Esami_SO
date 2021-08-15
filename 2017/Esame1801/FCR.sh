#!/bin/sh

cd $1

#variabile che contiene i nomi assoluti dei file
files=

#variabile booleana
trovato=0

#si usa il case per controllare anche la radice della gerarchia

case $1 in 
*/$2)
	for i in *
	do
	if test -f $i -a -r $i 
	then
	
		
		if grep [0-9] $i > /dev/null
		then
			
		trovato=1
		files="$files `pwd`/$i"
		
		fi
	fi
	
	
	done
 
;;
esac



if test $trovato -eq 1 
then
	echo `pwd` 
	echo $files >> $3

fi


#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 $3
	fi
done


