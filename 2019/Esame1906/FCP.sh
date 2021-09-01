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

if test $H -le 1 -o $H -ge 255
then
	echo numero errato
	exit 4
fi

shift

for i in $*
do

case $i in
/*) if test ! -d $i -o ! -x $i
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

#programma organizzzato in W fasi


for i in $*
do
	FCR.sh $i $H 
done

