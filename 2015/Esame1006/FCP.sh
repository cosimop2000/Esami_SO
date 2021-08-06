#!/bin/sh

#controllo numero parametri


case $# in 
3)
;;
*)echo numero parametri scorretto 
    exit 1 
;;
esac



#controllo sul tipo dei parametri

case $1 in
/*) if test ! -d $1 -o ! -x $1
    then 
	echo directory non traversabile
	exit 2
   fi
;;
*) echo directory nome non assoluto
   exit 3
;;
esac

case $2 in
*/*) echo nome non relativo semplice
	exit 4
;;
*) ;;
esac


case $3 in 
*[!0-9]*) echo non numero
	  exit 5
;;
*) if test $3 -eq 0
   then echo non diverso da 0
    exit 6
   fi
;;
esac

H=$3


#set variabile PATH
PATH=`pwd`:$PATH
export PATH

#programma organizzato in N fasi

> /tmp/esame

#file tmp che contiene nomi dei file nelle n gerarchie


FCR.sh $1 $2 $H /tmp/esame


cat /tmp/esame

echo invocazione parte C
main `cat /tmp/esame` $H


rm /tmp/esame 

