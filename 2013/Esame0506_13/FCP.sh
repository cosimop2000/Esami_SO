#!/bin/sh

#controllo numero parametri

case $# in 
2) ;;
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


#set variabile PATH
PATH=`pwd`:$PATH
export PATH


> /tmp/esame

FCR.sh $1 $2 /tmp/esame 

D=`wc -l < /tmp/esame`

echo numero totale direttori $D
cat /tmp/esame

X=
echo numero compreso tra 1 e $D
read X
		
case $X in 
*[!0-9]*) echo non numero
	  rm /tmp/esame
	  exit 5
	  ;;
*);;
esac

if test $X  -lt 1 -o $X  -gt $D
then
	echo numero non compreso tra 1 e $D
	rm /tmp/esame
	  exit 6
fi	

#variabile che indica la dir
count=1

#variabile che contiene i file
file=

#variabile che contiene il nome assoluto della directory
d=`head -$X < /tmp/esame | tail -1`

cd $d

for i in *
do
	if test -f $i
	then
		case $i in
		*.$2)file="$file $i"
		;;
		esac
	fi
done

echo $file
#main $file
		
rm /tmp/esame

