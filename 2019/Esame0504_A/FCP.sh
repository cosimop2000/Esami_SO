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

F=$1

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

FCR.sh $i $F /tmp/esame

done

count=`wc -l < /tmp/esame`

echo numero file trovati $count
cat /tmp/esame

X=0
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
	K=$i

	echo chiedo un numero X compreso tra 1 e la lunghezza in linee $K
	read X

	case $X in 
	*[!0-9]*) echo non numero
		  rm /tmp/esame 	
		  exit 5
	;;
	*) if test $X -eq 0 -o $X -gt $K
	   then 
	   	echo non diverso da 0 o maggiore della lunghezza
		rm /tmp/esame 
	        exit 6
	   fi
	;;
	esac


	echo visualizzo le prime $X linee del file $file
	head -$X $file 

fi


done



rm /tmp/esame
