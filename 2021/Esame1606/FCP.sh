#!/bin/sh

#controllo numero parametri

case $# in 
0|1|2|3)echo numero parametri scorretto 
      exit 1
;;
*)
;;
esac


#controllo sul tipo dei parametri

case $1 in 
*[!0-9]*) echo non numero
	  exit 2
;;
*) if test $1 -eq 0
   then echo non diverso da 0
    exit 3
   fi
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


H=$1
M=$2

shift
shift

for G in $*
do

case $G in
/*) if test ! -d $G -o ! -x $G
    then 
    	echo directory non traversabile
	exit 6
     fi
;;
*) echo directory nome non assoluto
 exit 7
;;
esac
	
done		
		

#set variabile PATH
PATH=`pwd`:$PATH
export PATH

#programma organizzzato in Q fasi


for G in $*
do
	FCR.sh $G $H $M

done


