#!/bin/sh

cd $1

#fase A conto i livelli

conta=`expr $3 + 1`

livello_max=$conta


if test $2 = B
then
	if test $4 -eq $conta
	then
		echo `ls -l`
	fi
	
fi


#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 $conta $4
	
	ret=$?
	if test $ret -gt $livello_max 
	then livello_max=$ret
	fi
	
	
	fi
done



exit $livello_max
