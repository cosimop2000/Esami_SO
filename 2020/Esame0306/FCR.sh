#!/bin/sh

cd $1


#variabile booleana
trovato=0


#for i in *
#do
#	if test -d $i 
#	then
#		
#		case $i in
#		
#		$2?$2)
#		trovato=1
#		D=$i
#		;;
#		*)
#		;;
#		esac
#	
#
#	fi	
#
#done

case $1 in 
*/$2?$2) trovato=1
;;
*);;
esac

if test $trovato -eq 1
then
	#echo `pwd` >> $3
	pwd >> $3
fi


#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 $3 
	fi
done


