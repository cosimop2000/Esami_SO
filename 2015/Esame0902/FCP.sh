#!/bin/sh

#controllo numero parametri


case $# in 
2)
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
*[!0-9]*) echo non numero
	  exit 4
;;
*) if test $2 -eq 0
   then echo non diverso da 0
    exit 5
   fi
;;
esac

G1=$1
K=$2

#set variabile PATH
PATH=`pwd`:$PATH
export PATH

> /tmp/esame

#file tmp che contiene nomi assoluti dei file

FCR.sh $G1 $K /tmp/esame

cat /tmp/esame

#il file temporaneo contiene i nomi assoluti dei file trovati

#variabile che contiene i nomi assoluti intervallati da un numero x

params=

X=0

for i in `cat /tmp/esame`
do

echo chiedo un numero X positivo,minore uguale a K
read X

case $X in 
*[!0-9]*) echo non numero
	rm /tmp/esame 	
	  exit 6
;;
*) if test $X -eq 0 -o $X -gt $K
   then echo non diverso da 0 e maggiore di K
	rm /tmp/esame 
       exit 7
   fi
;;
esac

params="$params $i $X"

done

echo $params

main $params


rm /tmp/esame 

