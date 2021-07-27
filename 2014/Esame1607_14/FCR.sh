#!/bin/sh

cd $1

#variabile che contiene il nome assoluto dei file
files=

#variabile per il numero di linee
count=

#variabile booleana
trovato=0

#variabile che conta le linee che iniziano con una minuscola
lines=0

for i in *
do
	if test -f $i -a -r $i
	then
		count=`wc -l < $i`
		
		if test $count -eq $2
		then

		
		lines=`grep ^[a-z] $i | wc -l`
		
		if test $lines -eq $2
		then
			trovato=1
			files="$files $i"
		fi
		
		fi
		
	fi
	
done

if test $trovato -eq 1
then
	echo `pwd`
	echo $files
	
	echo "conferma richiesta all' utente Si|SI|si "
	read conferma
	
	case $conferma in 
	si|Si|SI) 
	echo conferma OK
	#invoco la parte C passando i nomi dei file e la lunghezza
	main $files $2
	  ;;
	*) echo nessuna invocazione parte C  
	 ;;	
	esac	
	
fi



#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 
	fi
done


