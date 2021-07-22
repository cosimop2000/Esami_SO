#!/bin/sh

cd $1

#variabile che contiene il numero dei file
files=0

#variabile booleana
dir=0

for i in *
do
	if test  -f $i 
	then
		files=`expr $files + 1`
	fi
	
	if test -d $i 
	then
		dir=`expr $dir + 1`
			
	fi
	
done


 
if test $files -eq $2 -a $dir -eq 0
then
	pwd >> $3
	echo `pwd` dir che contiene solo gli N file richiesti
	fi


#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 $3
	fi
done


