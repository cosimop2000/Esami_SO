#!/bin/sh

#controllo numero parametri

case $# in 
0|1|2|3)echo numero parametri scorretto 
      exit 1
;;
*)
;;
esac


#controllo sul tipo dei parametri

case $1 in 
*/*) echo nome non relativo semplice
	exit 2
	;;
*)
;;
esac


case $2 in 
*[!0-9]*) echo non numero
	  exit 3
;;
*) if test $2 -eq 0
   then echo non diverso da 0
    exit 4
   fi
;;
esac

D=$1
Y=$2

shift
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

#programma organizzzato in W fasi


#file temporaneo che contiene i nomi dei file creati

for i in $*
do

	> /tmp/esame
	
	FCR.sh $i $D $Y /tmp/esame

	count=`wc -l < /tmp/esame`
	
	echo file trovati $count
	
	cat /tmp/esame
	
	echo invocazione parte C
	main `cat /tmp/esame` $Y
	

done

rm /tmp/esame
