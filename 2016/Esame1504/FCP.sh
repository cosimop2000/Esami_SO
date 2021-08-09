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

#variabile che contiene le gerarchie
params=

#varaibile per capire se siamo all'ultimo parametro
num=1

for i in $*
do
	if test $num -ne $#
	then
		case $i in
		/*) if test ! -d $i -o ! -x $i
    			then 
			echo directory non traversabile
			exit 2
		   fi
		;;
		*) echo directory nome non assoluto
 		  exit 3
		;;
		esac
	
	params="$params $i"

	fi
	
	
	if test $num -eq $#
	then
	
	case $i in 
	*[!0-9]*) echo non numero
	  exit 4
	;;
	*) if test $i -eq 0
   	then echo non diverso da 0
  	  exit 5
  	 fi
	;;
	esac

	X=$i

	fi

num=`expr $num + 1`

done


#set variabile PATH
PATH=`pwd`:$PATH
export PATH

#programma organizzato in N fasi

> /tmp/esame

#file tmp che contiene nomi assoluti delle directory nelle n gerarchie


for i in $params
do

FCR.sh $i $X /tmp/esame

done

#variabile che contiene il numero dei nomi assoluti 
count=`wc -l < /tmp/esame`
echo $count directory trovati

cat /tmp/esame


K=0

for i in `cat /tmp/esame`
do

echo la direcctory $i contiene i file

cd $i

for j in *
do
	echo `pwd`/$j
	echo `tail -$X $j | head -1`
	
done


done


#invocazione parte C

rm /tmp/esame 

