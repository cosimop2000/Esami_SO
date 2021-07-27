#!/bin/sh

#controllo numero parametri


case $# in 
3)
;;
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
/*) if test ! -d $2 -o ! -x $2
    then 
	echo directory non traversabile
	exit 4
   fi
;;
*) echo directory nome non assoluto
   exit 5
;;
esac


case $3 in 
*[!0-9]*) echo non numero
	  exit 6
;;
*) if test $3 -eq 0
   then echo non diverso da 0
    exit 7
   fi
;;
esac

G1=$1
G2=$2
K=$3

#set variabile PATH
PATH=`pwd`:$PATH
export PATH

> /tmp/esame

#fase A

FCR.sh $G1 $K /tmp/esame


#fase B

FCR.sh $G2 $K /tmp/esame

cat /tmp/esame

#il file temporaneo contiene i nomi assoluti dei file trovati

main `cat /tmp/esame` $K

rm /tmp/esame 

