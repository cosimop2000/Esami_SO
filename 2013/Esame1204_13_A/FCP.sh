#!/bin/sh

#controllo numero parametri

case $# in 
3) ;;
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

case $2 in
*/*) echo non in forma semplice 
	exit 4
	;;
*) ;;
esac


case $3 in 
*[!0-9]*) echo non numero
	  exit 5
;;
*) if test $3 -eq 0
   then echo non diverso da 0
    exit 6
   fi
;;
esac


#set variabile PATH
PATH=`pwd`:$PATH
export PATH

fase=A

> /tmp/esame

FCR.sh $1 $2 $3 $fase /tmp/esame 0

D=`wc -l < /tmp/esame`

echo numero totale direttori $D
echo nome dir `cat /tmp/esame`


if test $D -gt $3
then
	fase=B
	X=
	echo numero compreso tra 1 e $3
	read X
	
	
	case $X in 
	*[!0-9]*) echo non numero
	  rm /tmp/esame
	  exit 7
	;;
	
	esac	
		
	> /tmp/esame
	
	#per la fase B invoco nuovamente il file FCR.sh
	
	FCR.sh $1 $2 $3 $fase /tmp/esame $X
			
	
fi

rm /tmp/esame

