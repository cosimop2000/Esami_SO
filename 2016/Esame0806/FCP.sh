#!/bin/sh

#controllo numero parametri


case $# in 
4)
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
/*) if test ! -d $2 -o ! -x $2
    then 
    	echo directory non traversabile
	exit 4
     fi
;;
*) echo directory nome non assoluto
 exit 5
;;
esac

case $3 in
*/*)echo nome non relativo semplice
    exit 6
    ;;
*)	
;;
esac
		
case $4 in 
*[!0-9]*) echo non numero
	  exit 7
	;;
*) if test $4 -eq 0
   	then echo non diverso da 0
  	  exit 8
  	 fi
;;
esac

if test $4 -ge 255
then
	echo numero maggiore uguale a 255
	exit 9
fi


G1=$1
G2=$2
D=$3
H=$4


#set variabile PATH
PATH=`pwd`:$PATH
export PATH

#programma organizzato in 2 fasi

> /tmp/esame1
> /tmp/esame1file

echo fase 1

FCR.sh $1 $3 $4 /tmp/esame1 /tmp/esame1file

#variabile che contiene il numero dei nomi assoluti 
count1=`wc -l < /tmp/esame1`
echo $count1 directory trovati

cat /tmp/esame1file

> /tmp/esame2
> /tmp/esame2file

echo fase 2

FCR.sh $2 $3 $4 /tmp/esame2 /tmp/esame2file 

#variabile che contiene il numero dei nomi assoluti 
count2=`wc -l < /tmp/esame2`
echo $count2 directory trovati

cat /tmp/esame2file

if test $count1 -eq 0 -o $count2 -eq 0
then
	echo non sono stati trovati due D
	rm /tmp/esame1
	rm /tmp/esame2
	rm /tmp/esame1file
	rm /tmp/esame2file
	exit 10
fi

#invocazione parte C

echo invocazione parte C
main `cat /tmp/esame1file` `cat /tmp/esame2file` $4


rm /tmp/esame1
rm /tmp/esame2
rm /tmp/esame1file
rm /tmp/esame2file
