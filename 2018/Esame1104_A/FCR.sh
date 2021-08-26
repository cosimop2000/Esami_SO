#!/bin/sh

cd $1

#variabile booleana
trovato=0

#variabile per contare le linee
count=0

#variabile 
nl=

for i in *
do
	if test -f $i -a -r $i
	then
		count=`wc -l < $i`
		
		if test $count -ge $2 	
		then
		
		if test $count -ge 5
		then
		
		> $i.quinta

		nl=`head -5 $i | tail -1 `
		
		echo $nl >> $i.quinta 	
		
		echo `pwd`/$i.quinta >> $3
		
		else
		
		> $i.NOquinta
		
		echo `pwd`/$i.NOquinta >> $3
		
		fi
		fi
	fi
	
	
done


#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 $3
	fi
done


