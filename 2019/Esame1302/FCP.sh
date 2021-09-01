#!/bin/sh

#controllo numero parametri


case $# in 
0|1|2) echo numero parametri scorretto 
    exit 1
;;
*)
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

shift

count=0

for i in $* 
do

case $2 in
*/*)echo nome non relativo semplice
	exit 4
;;
*)
;;
esac

count=`expr $count + 1`

done

#set variabile PATH
PATH=`pwd`:$PATH
export PATH

M=`expr $count / 2`
# numero di file uguale a N=$#

FCR.sh $G $M $* 

