#!/bin/sh

#controllo numero parametri

case $# in 
0|1|2)echo numero parametri scorretto 
      exit 1
;;
*)
;;
esac

F=$1

case $1 in
*/*) echo nome non relativo semplice
	exit 2
	;;
*);;
esac

shift

#controllo sul tipo dei parametri

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

#programma organizzzato in N fasi

> /tmp/esame

for i in $*
do
	FCR.sh $i $F /tmp/esame	
	
done

count=`wc -l < /tmp/esame`

#conto nomi dei file 

echo $count numero file trovati

for j in `cat /tmp/esame`
do
	echo $j

	echo prima linea 
	head -1 $j
	
	echo ultima linea
	tail -1 $j

done

rm /tmp/esame
