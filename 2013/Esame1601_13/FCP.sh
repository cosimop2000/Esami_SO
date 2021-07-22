#!/bin/sh

#controllo numero parametri

case $# in 
2) ;;
*) echo numero parametri scorretto
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
[a-z]) 
;;
[A-Z]) 
;;
*) echo non singolo carattere alfabetico
	exit 4
	;;
esac


#set variabile PATH
PATH=`pwd`:$PATH
export PATH

> /tmp/esame

FCR.sh $1 $2 /tmp/esame

echo contenuto == `cat < /tmp/esame` 

#main `cat /tmp/esame` $2

rm /tmp/esame

