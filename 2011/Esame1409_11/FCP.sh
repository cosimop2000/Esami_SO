#!/bin/sh

#controllo numero parametri

case $# in 
3) ;;
*) echo numero parametri scorretto
exit 1
;;
esac

#variabili per i parametri

G=$1
H=$2
K=$3

#controllo sul tipo dei parametri

case $1 in
/*) if test ! -d $G -o ! -x $G
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
*) if test $H -eq 0
   then echo non diverso da 0
    exit 5
   fi
;;
esac


case $3 in 
*[!0-9]*) echo non numero
	  exit 6
;;
*) if test $K -eq 0
   then echo non diverso da 0
    exit 7
   fi
;;
esac

if test $2 -ge $3
then 
	echo H maggiore di K
	exit 8
fi


#set variabile PATH
PATH=`pwd`:$PATH
export PATH


FCR.sh $G $H $K 


