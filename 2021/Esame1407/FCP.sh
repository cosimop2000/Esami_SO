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

params=
num=1

for G in $*
do
	
	if test $num -lt `expr $# - 1`
	then
	
	case $G in
	/*) if test ! -d $G -o ! -x $G
   	 then 
   	 	echo directory non traversabile
		exit 2
  	   fi
	;;
	*) echo directory nome non assoluto
 	exit 3
	;;
	esac

	params="$params $G" 
	
	else 
	
	if test $num -eq `expr $# - 1` 		
	then

	case $G in 
	*[!0-9]*) echo non numero
		  exit 4
	;;
	*) if test $G -eq 0
   	then echo non diverso da 0
    	exit 5
   	fi
	;;
	esac	
	
	H=$G
	
	fi 
	
	
	
	if test $num -eq $# 
	then
	
	case $G in 
	*[!0-9]*) echo non numero
		  exit 6
	;;
	*) if test $G -eq 0
	   then echo non diverso da 0
	    exit 7
	   fi
	;;
	esac
	
	M=$G
	
	fi 
	
	
	fi
	
	num=`expr $num + 1`

done


#set variabile PATH
PATH=`pwd`:$PATH
export PATH

#programma organizzzato in Q fasi


for G in $params
do
	FCR.sh $G $H $M

done


