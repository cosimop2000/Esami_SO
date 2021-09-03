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

num=1
params=
#variabile che contiene le gerarchie


for i in $* 
do


if test $num -ne $#
then

case $i in
/*) if test ! -d $i -o ! -x $i
    then 
	echo no directory o directory non traversabile
	exit 2
   fi
;;
*) echo directory nome non assoluto
   exit 3
;;
esac

params="$params $i"

else

case $i in
*[!0-9]*)echo non numero
	 exit 4
;;
*) if test $i -eq 0
   then
   	echo numero non strettamente positivo
   	exit 5
   fi
;;
esac

B=$i

fi

num=`expr $num + 1`

done

#set variabile PATH
PATH=`pwd`:$PATH
export PATH

#programma organizzato in Q fasi


for i in $params
do

FCR.sh $i $B 

done

