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

num=1
params=
#variabile che contiene le gerarchie


for i in $* 
do


if test $num -ne $#
then

case $i in
/*) if test ! -d $i -o ! -x $i
    then 
	echo no directory o directory non traversabile
	exit 2
   fi
;;
*) echo directory nome non assoluto
   exit 3
;;
esac

params="$params $i"

else

case $i in
*/*)echo nome non relativo semplice
	exit 4
;;
*)
;;
esac

S=$i

fi

num=`expr $num + 1`

done

#set variabile PATH
PATH=`pwd`:$PATH
export PATH

#programma organizzato in N fasi

> /tmp/esame


for i in $params
do

FCR.sh $i $S /tmp/esame

done

count=`wc -l < /tmp/esame`

echo numero file trovati $count
cat /tmp/esame

X=0
c=0

for i in `cat /tmp/esame`
do

c=`expr $c + 1`

if test `expr $c % 2` -eq 1
then
	echo nome file $i
	file=$i
	
	
else

	echo lunghezza file $i
	K=$i

	echo chiedo se il file deve essere ordinato yes/no
	read X

	case $X in
	yes) echo file ordinato secondo ordine alfabetico senza differenza tra maiuscole e minuscole
	
		
	
	 	sort -f $file
	 	
	 	cat $file
	 ;;

	*) echo non ordinato 
	;;
	
	esac
fi


done



rm /tmp/esame
