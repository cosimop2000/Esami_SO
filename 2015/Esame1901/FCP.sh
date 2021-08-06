#!/bin/sh

#controllo numero parametri


case $# in 
4)
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
?) ;;
*) echo non singolo carattere
	exit 6
	;;
esac

case $3 in
?) ;;
*) echo non singolo carattere
	exit 7
	;;
esac


G1=$1
G2=$2
C1=$3
C2=$4


#set variabile PATH
PATH=`pwd`:$PATH
export PATH

> /tmp/esameA

#fase A

FCR.sh $G1 $C1 /tmp/esameA

#variabili che contengono il numero di file all interno
A=
B=

#fase B

FCR.sh $G2 $C2 /tmp/esameB

A=`wc -l < /tmp/esameA`
B=`wc -l < /tmp/esameB`

echo fase A file $A
cat /tmp/esameA

echo fase B file $B
cat /tmp/esameB

if test $A -eq $B
then
	echo invocazione parte C
	#main `cat /tmp/esameA` `cat /tmp/esameB` $C1 $C2
fi

#i file temporanei contengono i nomi assoluti dei file trovati


rm /tmp/esameA
rm /tmp/esameB 

