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
    	echo non directory o non traversabile
	exit 2
     fi
;;
*) echo directory nome non assoluto
 exit 3
;;
esac
	
case $2 in 
*/*) echo nome non relativo semplice
	exit 4
;;
*)
;;
esac
	

#set variabile PATH
PATH=`pwd`:$PATH
export PATH

#programma organizzzato in N fasi

> /tmp/esame


FCR.sh $1 $2 /tmp/esame	
	

count=`wc -w < /tmp/esame`

#conto nomi dei file 

echo $count numero file trovati
cat /tmp/esame
echo invocazione parte C

main `cat /tmp/esame`

rm /tmp/esame
