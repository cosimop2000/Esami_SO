#!/bin/sh

#controllo numero parametri

case $# in 
3) ;;
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

G=$1

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

K=$2

if test $K -lt 2
then
	echo minore di 2
	exit 6
fi

case $3 in 
*[!0-9]*) echo non numero
	  exit 7
;;
*) if test $3 -eq 0
   then echo non diverso da 0
    exit 8
   fi
;;
esac

Y=$3

#set variabile PATH
PATH=`pwd`:$PATH
export PATH


FCR.sh $1 $2 $3 


