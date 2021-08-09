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
[a-z]) 
   	echo carattere minuscolo
  
;;
*) echo non singolo carattere o non minuscolo
	  exit 4
;;
esac

G=$1
Cx=$2


#set variabile PATH
PATH=`pwd`:$PATH
export PATH

> /tmp/esame

FCR.sh $1 $2 /tmp/esame

count=`wc -l < /tmp/esame`

if test $count -lt 2
then
	echo non sono stati trovati almeno 2 file
	rm /tmp/esame
	exit 5
fi

echo $count numero file trovati
cat /tmp/esame

echo invocazione parte C
main `cat /tmp/esame` $Cx

rm /tmp/esame
