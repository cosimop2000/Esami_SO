#!/bin/sh

#controllo numero parametri

case $# in 
0|1)echo numero parametri scorretto 
    exit 1
;;
*)
;;
esac

#controllo sul tipo dei parametri

for i in $*
do

case $i in
/*) if test ! -d $i -o ! -x $i
    then 
    	echo directory non traversabile
	exit 2
     fi
;;
*) echo directory nome non assoluto
 exit 3
;;
esac
	
done		
		

#set variabile PATH
PATH=`pwd`:$PATH
export PATH

#programma organizzzato in N fasi

> /tmp/esame

for i in $*
do
	FCR.sh $i /tmp/esame	
	
done

count=`wc -w < /tmp/esame`

#conto le words ( nomi dei file ) 

echo $count numero file trovati

for j in `cat /tmp/esame`
do
	echo $j
	echo invocazione parte C
	main $j

done

rm /tmp/esame
