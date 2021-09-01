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
*/*)echo nome non relativo semplice
	exit 4
;;
*)
;;
esac



G=$1
F=$2

#set variabile PATH
PATH=`pwd`:$PATH
export PATH


FCR.sh $G $F 

