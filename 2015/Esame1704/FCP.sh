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
*[!0-9]*) echo non numero
	  exit 2
;;
*) if test $1 -eq 0
   then echo non diverso da 0
    exit 3
   fi
;;
esac

X=$1

shift 

for i in $*
do

case $i in
/*) if test ! -d $i -o ! -x $i
    then 
	echo directory non traversabile
	exit 4
   fi
;;
*) echo directory nome non assoluto
   exit 5
;;
esac

done


#set variabile PATH
PATH=`pwd`:$PATH
export PATH

#programma organizzato in N fasi

> /tmp/esame

#file tmp che contiene nomi assoluti dei file nelle n gerarchie


for i in $*
do

FCR.sh $i $X /tmp/esame

done

cat /tmp/esame

#variabile che contiene il numero dei nomi assoluti 
count=`wc -l < /tmp/esame`

echo $count file trovati

K=0

for i in `cat /tmp/esame`
do


echo $i

echo chiedo un numero K positivo,minore a X
read K

case $K in 
*[!0-9]*) echo non numero
	rm /tmp/esame 	
	  exit 6
;;
*) if test $K -eq 0 -o $K -ge $X
   then echo non diverso da 0 e maggiore di X
	rm /tmp/esame 
       exit 7
   fi
;;
esac

echo `head -$K $i | tail -1`

done


#invocazione parte C

rm /tmp/esame 

