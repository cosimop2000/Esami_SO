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

if test $2 -lt 2
then
	echo minore di 2
	exit 6
fi

#set variabile PATH
PATH=`pwd`:$PATH
export PATH

> /tmp/esame

FCR.sh $1 $2 /tmp/esame

echo `wc -l < /tmp/esame`

#variabili C,lunghezza in caratteri totale e H ,caratteri per file
C=
H=0

for i in `cat /tmp/esame`
do
	cd $i
	
	C=0
	
	for j in *
	do
	H=`wc -c < $j`
	C=`expr $C + $H`
	done

len=`expr $C / $2`

echo lunghezza media $len

main * $len
	
done

rm /tmp/esame

