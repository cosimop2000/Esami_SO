#!/bin/sh

cd $1

#variabile booleana
trovato=0

#variabile per contare le linee del file
count=

#variabile 
nl=

case $1 in
*/$2)

for i in *
do

	
	if test -f $i -a -r $i
	then
		count=`wc -l < $i`
				
		if test $count -ge 5
		then
		
		> $i.quintultima

		nl=`tail -5 $i | head -1 `
		
		echo $nl >> $i.quintultima 	
		
		echo `pwd`/$i.quintultima >> $3
		
		else
		
		> $i.NOquintultima
		
		echo `pwd`/$i.NOquintultima >> $3
		
		fi

	fi	
done
;;
*);;
esac


#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 $3
	fi
done


