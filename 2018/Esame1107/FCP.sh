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
?) 
;;
*) echo non singolo carattere
	exit 2

;;
esac

Cz=$1

shift

for i in $*
do

case $i in
/*) if test ! -d $i -o ! -x $i
    then 
    	echo directory non traversabile
	exit 3
     fi
;;
*) echo directory nome non assoluto
 exit 4
;;
esac
	
done		
		

#set variabile PATH
PATH=`pwd`:$PATH
export PATH

#programma organizzzato in W fasi

> /tmp/esame

#file temporaneo che contiene i nomi dei file creati

for i in $*
do
	FCR.sh $i $Cz /tmp/esame
done

count=`wc -l < /tmp/esame`

echo numero file trovati $count

echo invocazione parte C

main $Cz `cat /tmp/esame`

rm /tmp/esame

