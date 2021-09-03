#!/bin/sh

cd $1


#variabile contatore per la dimensione in linee
L=0


#file F
F=

#variabile booleana
trovato=0

shift

for F in *
do
	if test -f $F 
	then
		
		trovato=1
		
		for c in $*
		do
						
			if ! grep $c $F > /dev/null
			then
				trovato=0
			
			fi
			
			
		done		
		
		if test $trovato -eq 1
		then
			L=`wc -l < $F`
			
			echo `pwd`/$F
			echo invocazione parte C
			
			main `pwd`/$F $L $*
			
		
		fi
		
	fi	
	

done


#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $* 
	fi
done


