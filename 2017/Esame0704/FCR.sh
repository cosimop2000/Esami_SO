#!/bin/sh

cd $1

#variabile booleana
trovato=0


for i in *
do
	if test -f $i -a -r $i
	then
		
		if test $i = $2	
		then
		
		trovato=1
		
		sort -f $i > sorted
		
		echo `pwd`/sorted >> $3
		
		fi
	fi
	
	
done


if test $trovato -eq 1 
then
	echo `pwd`
	
	
	#sort -f per ignorare maiuscole e minuscole
	#sort -u per ignorare le linee doppie
	

fi


#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 $3
	fi
done


