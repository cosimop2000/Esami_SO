#!/bin/sh

cd $1


#file F
F=

#variabile booleana
trovato=0

#variabile per contare i file 
cont=0

G=`basename $1`
#comando per salvare solo la parte relativa del nome

shift


case $G in

*[0-9]*)

for F in *
do
	if test -f $F 
	then
		
		for i in $*
		do
			trovato=0
			
			if test $F = $i -a $trovato -eq 0
			then
				cont=`expr $cont + 1`
				trovato=1
		
			fi		
		done
	fi	
	

done

;;
*)
;;
esac


if test $cont -eq $#
then
	echo trovata directory `pwd`
			
	echo invocazione parte C
			
	main $*
			
		
fi

#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $* 
	fi
done


