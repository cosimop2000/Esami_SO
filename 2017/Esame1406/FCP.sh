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
?);;
*)echo non singolo carattere
  exit 6
  ;;
esac



G=$1
K=$2
Cx=$3


#set variabile PATH
PATH=`pwd`:$PATH
export PATH

> /tmp/esame

FCR.sh $G $K $Cx /tmp/esame

count=`wc -w < /tmp/esame`
echo $count
cat /tmp/esame

echo invocazione parte C
main `cat /tmp/esame` $Cx

rm /tmp/esame
