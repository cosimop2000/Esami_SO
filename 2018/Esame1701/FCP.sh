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
*[!0-9]*) echo non numero
	  exit 4
;;
*) if test $2 -eq 0
   then echo non diverso da 0
    exit 5
   fi
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

case $4 in
?);;
*)echo non singolo carattere
  exit 8
  ;;
esac



G=$1
N=$2
H=$3
Cx=$4


#set variabile PATH
PATH=`pwd`:$PATH
export PATH


FCR.sh $G $N $H $Cx 


