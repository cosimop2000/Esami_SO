#!/bin/sh

#controllo numero parametri

case $# in 
2) ;;
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

#set variabile PATH
PATH=`pwd`:$PATH
export PATH

> /tmp/esame

FCR.sh $1 $2 /tmp/esame

#variabili N,lunghezza in linee e H ,caratteri diviso linee
N=
H=

for i in `cat /tmp/esame`
do
	N=`wc -l < $i`
	H=`wc -c <$i`
	H=`expr $H / $N`
	echo linee $N len_media $H per il file $i
	#main $i $N $H
done



rm /tmp/esame


