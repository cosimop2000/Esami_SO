#!/bin/sh

#controllo numero parametri

case $# in 
0|1|2)echo numero parametri scorretto 
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

if test $1 -ge 100
then
	echo numero non minore di 100
	exit 4
fi

L=$1

shift


for i in $*
do

case $i in
/*) if test ! -d $i -o ! -x $i
    then 
    	echo directory non traversabile
	exit 5
     fi
;;
*) echo directory nome non assoluto
 exit 6
;;
esac
	
done		
		

#set variabile PATH
PATH=`pwd`:$PATH
export PATH

#programma organizzzato in N fasi

> /tmp/esame

#variabile che contiene il nome assoluto e la lunghezza in linee

for i in $*
do
	FCR.sh $i $L /tmp/esame	
	
done

count=`wc -l < /tmp/esame`
count=`expr $count / 2`

#conto nomi dei file 

echo $count numero file trovati

cat /tmp/esame

echo invocazione parte C

main `cat /tmp/esame`

rm /tmp/esame
