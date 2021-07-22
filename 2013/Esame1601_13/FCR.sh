#!/bin/sh

cd $1

#variabile che contiene la versione maiuscola o minuscola del carattere
ch=

trovato=false

for i in *
do
	if test  -f $i -a -r $i 
	then
	#valori tabella ASCII 90 -> Z  97 -> a
	
	#se il carattere e' maiuscolo
			if grep $2 $i > /dev/null
			then
				ch=`echo $2 | tr [:lower:] [:upper:]`
				if grep $ch $i > /dev/null
				then
					trovato=true
					echo `pwd`/$i >> $3
				fi
			fi
			
		
	#se il carattere e' minuscolo	
	
		if grep ch $i > /dev/null
			then
				ch=`echo $2 | tr [:upper:] [:lower:]`
				if grep ch $i > /dev/null
				then
					trovato=true
					echo `pwd`/$i >> $3
				fi
			fi
		
	fi
	
done

if test $trovato = true
then
	echo `pwd`
fi



#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 $3
	fi
done


