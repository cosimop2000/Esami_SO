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

H=$1

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

#programma organizzzato in W fasi


for i in $*
do
	
	> /tmp/esame

	FCR.sh $i $H /tmp/esame
	
	count=`wc -l < /tmp/esame`
	
	echo $count numero file trovati
		
	if test `expr $count % 2` -eq 0
	then
		echo invocazione parte C
		
		main `cat /tmp/esame`
	fi 
	 
done


rm /tmp/esame
