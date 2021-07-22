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

G1=$1

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

G2=$2

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

K=$3

#set variabile PATH
PATH=`pwd`:$PATH
export PATH

#fase A

> /tmp/esame1

echo fase A

FCR.sh $G1 $K /tmp/esame1

#fase B

> /tmp/esame2

echo fase B

FCR.sh $G2 $K /tmp/esame2

file1=`wc -l < /tmp/esame1`
file2=`wc -l < /tmp/esame2`

echo $file1 ----- $file2

if test $file1 -eq $file2 -a $file1 -eq $K
then
	echo invoco la parte C
	#main `cat /tmp/esame1` `cat /tmp/esame2`
fi


rm /tmp/esame1
rm /tmp/esame2

