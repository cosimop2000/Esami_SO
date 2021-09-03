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
?)
;;
*)echo non singolo carattere
	exit 2
;;
esac

C=$1

shift

for i in $* 
do

case $i in
/*) if test ! -d $i -o ! -x $i
    then 
	echo no directory o directory non traversabile
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

#programma organizzato in N fasi

> /tmp/nomiAssoluti


for i in $*
do

FCR.sh $i $C /tmp/nomiAssoluti

done

count=`wc -l < /tmp/nomiAssoluti`

echo numero directory trovate $count

#variabile answer per ricevere risposta di Elena
answer=

for i in `cat /tmp/nomiAssoluti`
do

	echo $i
	
	echo Elena , vuole visualizzare il contenuto della directory compresi gli elementi nascosti "(si/yes)"
	
	read answer
	
	case $answer in
	si|yes) ls -a $i
	;;
	*) echo nessuna visualizzazione richiesta
	;;
	esac
done


rm /tmp/nomiAssoluti
