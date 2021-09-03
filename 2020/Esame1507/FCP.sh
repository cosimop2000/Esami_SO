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
*[!0-9]*)echo non numero
	 exit 2
;;
*) if test $1 -eq 0
   then
   	echo numero non strettamente positivo
   	exit 3
   fi
;;
esac

L=$1

shift

for i in $* 
do

case $i in
/*) if test ! -d $i -o ! -x $i
    then 
	echo no directory o directory non traversabile
	exit 4
   fi
;;
*) echo directory nome non assoluto
   exit 5
;;
esac

done

#set variabile PATH
PATH=`pwd`:$PATH
export PATH

#programma organizzato in Q fasi


for i in $*
do

> /tmp/nomiAssoluti$$

FCR.sh $i $L /tmp/nomiAssoluti$$

count=`wc -l < /tmp/nomiAssoluti$$`

echo $count file trovati

for F in `cat /tmp/nomiAssoluti$$`
do
	echo $F
	echo invocazione parte C
	
	main $F $L

done

done


rm /tmp/nomiAssoluti$$

