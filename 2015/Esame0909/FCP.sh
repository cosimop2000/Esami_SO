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
/*)	if test ! -f $2 -o ! -r $2
	then
		echo no file o non leggibile
		exit 4
    	fi
;;
*) echo nome non assoluto
   exit 5
  ;;
esac

G=$1
AF=$2

#set variabile PATH
PATH=`pwd`:$PATH
export PATH

#variabile che indica i byte del file AF
bytes=`wc -c < $AF`

echo $bytes
echo invoco la parte ricorsiva


FCR.sh $1 $2 $bytes 


