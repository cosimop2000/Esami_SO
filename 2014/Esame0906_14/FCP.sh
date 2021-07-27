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
X=0

for i in $*
do

echo "numero richiesto all' utente (maggiore di 255)"
	read X
	
	case $X in 
	*[!0-9]*) echo non numero
	  exit 4
	  ;;
	 *);;	
	esac	

if test $X -le 255
then
	echo numero minore di 255
	exit 5
fi

> /tmp/esame-$n
FCR.sh $i $X /tmp/esame-$n

n=`expr $n + 1`

done


n=1

#variabile che contiene il nome assoluto dei primi file di ogni G
files=


for i in $*
do

echo nome gerarchia $i

D=`wc -l < /tmp/esame-$n`

echo numero totale file = $D
echo nomi file `cat /tmp/esame-$n`

files="$files `head -1 /tmp/esame-$n`"

rm /tmp/esame-$n
n=`expr $n + 1`

done

#invoco la parte C
#main $files


