#!/bin/sh

#controllo numero parametri


case $# in 
0|1|2) echo numero parametri scorretto 
    exit 1
;;
*)
;;
esac


#controllo sul tipo dei parametri

case $1 in
*/*)echo nome non relativo semplice
	exit 2
;;
*)
;;
esac

D=$1

shift

for i in $* 
do

case $i in
/*) if test ! -d $i -o ! -x $i
    then 
	echo no directory o directory non traversabile
	exit 3
   fi
;;
*) echo directory nome non assoluto
   exit 4
;;
esac

done

#set variabile PATH
PATH=`pwd`:$PATH
export PATH

#programma organizzato in N fasi

> /tmp/esame


for i in $*
do

FCR.sh $i $D /tmp/esame

done

count=`wc -l < /tmp/esame`

echo numero file trovati $count

K=0
c=0

for i in `cat /tmp/esame`
do

c=`expr $c + 1`

if test `expr $c % 2` -eq 1
then
	echo nome file $i
	file=$i
	
	
else

	echo lunghezza file $i
	X=$i

	echo chiedo un numero K compreso tra 1 e 4
	read K

	case $K in 
	*[!0-9]*) echo non numero
		  rm /tmp/esame 	
		  exit 5
	;;
	*) if test $K -eq 0 -o $K -gt 4
	   then 
	   	echo non diverso da 0 o maggiore di 4
		rm /tmp/esame 
	        exit 6
	   fi
	;;
	esac


	echo visualizzo la K-esima linea del file $file
	head -$K $file | tail -1 

fi


done



rm /tmp/esame
