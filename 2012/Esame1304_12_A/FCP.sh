#!/bin/sh

#controllo numero parametri

case $# in 
1) ;;
*) echo numero parametri scorretto
exit 1
;;
esac


#controllo sul tipo dei parametri

case $1 in
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



#set variabile PATH
PATH=`pwd`:$PATH
export PATH

#variabile che identifica la fase
fase=A

#variabile che identifica i livelli
level=0


FCR.sh $1 $fase $level 0

tot=$?

echo livelli gerarchia ===  $tot

fase=B

#variabile che contiene il numero chiesto all'utente
num=

echo si chiede un numero compreso tra 1 e $tot

read num

case $num in
*[!0-9]*) echo non numero o non positivo
		exit 4
		;;
*) ;;
esac


if test $num -ge 1 -a $num -le $tot
then
	echo OK
else
	echo NO
	exit 5
fi


FCR.sh $1 $fase $level $num 


