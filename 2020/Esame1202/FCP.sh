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
*[!0-9]*) echo non numero
	  exit 2
;;
*) if test $1 -eq 0
   then echo non diverso da 0
    exit 3
   fi
;;
esac

case $2 in 
*[!0-9]*) echo non numero
	  exit 4
;;
*) if test $2 -eq 0
   then echo non diverso da 0
    exit 5
   fi
;;
esac

K1=$1
K2=$2

if test `expr $K1 % 2` -eq 0 -o `expr $K2 % 2` -eq 1
then
	echo K1 pari o K2 dispari
	exit 6
fi

shift
shift

for i in $*
do

case $i in
/*) if test ! -d $i -o ! -x $i
    then 
    	echo directory non traversabile
	exit 7
     fi
;;
*) echo directory nome non assoluto
 exit 8
;;
esac
	
done		
		

#set variabile PATH
PATH=`pwd`:$PATH
export PATH

#programma organizzzato in W fasi

> /tmp/esame

for i in $*
do
	FCR.sh $i $K1 $K2 /tmp/esame 
done

count=`wc -l < /tmp/esame`
echo $count file trovati

cat /tmp/esame

echo invocazione parte C

main `cat /tmp/esame`

rm /tmp/esame
