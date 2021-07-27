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

done


#set variabile PATH
PATH=`pwd`:$PATH
export PATH

#programma organizzato in N fasi

n=1

for i in $*
do

> /tmp/esame-$n
FCR.sh $i /tmp/esame-$n

n=`expr $n + 1`

done


n=1

for i in $*
do

echo nome gerarchia $i

D=`wc -l < /tmp/esame-$n`

echo numero totale file = $D

echo nomi file `cat /tmp/esame-$n`

X=0

for j in `cat /tmp/esame-$n`
do
echo numero di linee richieste
	read X
	
	case $X in 
	*[!0-9]*) echo non numero
	  rm /tmp/esame-$n
	  exit 4
	  ;;
	 *);;	
	esac	

head -$X < $j

done		


rm /tmp/esame-$n
n=`expr $n + 1`

done


