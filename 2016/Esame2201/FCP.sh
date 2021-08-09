#!/bin/sh

#controllo numero parametri

case $# in 
2)
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
*[!0-9]*) echo non numero
	  exit 4
;;
*) if test $2 -eq 0
   then echo non diverso da 0
    exit 5
   fi
;;
esac

N=$2

if test $N -lt 2
then
	echo numero minore di 2
	exit 6
fi

#set variabile PATH
PATH=`pwd`:$PATH
export PATH

N=`expr $N + $N`
#N=`expr $N \* 2`

echo $N

FCR.sh $1 $N 


