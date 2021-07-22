#!/bin/sh

#controllo numero parametri

case $# in 
0|1|2) echo numero parametri scorretto
exit 1
 ;;
*) ;;
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

for i in $*
do
	case $i in
	*/*) echo nome non relativo semplice
		exit 4
		;;
	*) ;;
	esac

done


#set variabile PATH
PATH=`pwd`:$PATH
export PATH

#variabile che contiene la meta dei file
M=`expr $# / 2`

FCR.sh $G $M $*

