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
*/*) echo non deve contenere il carattere \/
	  exit 4
;;
*) 
;;
esac

W=$1
S=$2

shift
shift

for G in $*
do

case $G in
/*) if test ! -d $G -o ! -x $G
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

#programma organizzzato in Q fasi

> /tmp/nomiAssoluti

for G in $*
do
	FCR.sh $G $S /tmp/nomiAssoluti  
done

count=`wc -l < /tmp/nomiAssoluti`

echo $count directory trovate

if test $count -ge $W
then

	echo Cosimo ,inserisci un numero X compreso tra 1 e $W
	read X
	
	case $X in 
	*[!0-9]*) echo non numero
	  rm /tmp/nomiAssoluti
	  exit 7
	;;
	*) if test $X -lt 1 -o $X -gt $W
  	   then 
  	   	echo non compreso nel giusto intervallo
    		rm /tmp/nomiAssoluti
    		exit 8
   	   fi
	;;
	esac
	
	k=0
	
	for j in `cat /tmp/nomiAssoluti`
	do
		k=`expr $k + 1`
		if test $k -eq $X
		then
			echo trovata directory $j
		fi
	
	done
	
	#head -$X < /tmp/nomiAssoluti | tail -1

fi


rm /tmp/nomiAssoluti

