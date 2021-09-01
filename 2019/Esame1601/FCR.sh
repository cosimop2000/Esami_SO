#!/bin/sh

cd $1

#variabile per il numero di linee
lines1=
lines2=

#variabile per il numero di caratteri
car1=
car2=

#variabile per il numero di file
nfile=0

#variabile che contiene i nomi assoluti dei file trovati 
files=

#variabili booleana
trovato1=0
trovato2=0


for i in *
do
	if test -f $i  
	then
	
	case $i in
	$2.1) if test $trovato1 -eq 0
		then
			lines1=`wc -l < $i`
			car1=`wc -c < $i`
			
			trovato1=1
			nfile=`expr $nfile + 1`
			files="$files `pwd`/$i"
		fi
	;;
	$2.2)  if test $trovato2 -eq 0
		then
			lines2=`wc -l < $i`
			car2=`wc -c < $i`
			
					
			trovato2=1
			nfile=`expr $nfile + 1`
			files="$files `pwd`/$i"
			
		fi
	;;
	*)
	;;
	
	esac

	fi	
	
done

if test $nfile -eq 2
then
	if test $lines1 -eq $lines2 -a $car1 -eq $car2
	then

	echo trovata directory `pwd`
	echo $files
	
	echo invocazione C
	
	main $files 
		
	fi
fi



#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 
	fi
done


