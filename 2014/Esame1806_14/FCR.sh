#!/bin/sh

cd $1

#variabile che contiene il nome assoluto dei file
files=

#variabile per il numero di linee
count=

#variabile booleana
trovato=0

for i in *
do
	if test -f $i 
	then
		count=`wc -l < $i`
		
		if test $count -eq $3
		then

		case $i in 
		$2*) ;;
		*$2) ;;
		*$2*) # echo stringa non allo inizio non alla fine
				files="$files $i"
				trovato=1
				;;
		*) ;;		
		esac
		
		fi
		
	fi
	
done

if test $trovato -eq 1
then
	echo `pwd`
	echo $files
	
	echo "conferma richiesta all' utente Si|SI|si "
	read X
	
	case $X in 
	si|Si|SI) 
	echo conferma OK
	#invoco la parte C passando i nomi dei file e la lunghezza
	#main $files $3
	  ;;
	*)	  
	 ;;	
	esac	
	
fi



#parte ricorsiva

for i in *
do 
	if test -d $i -a -x $i
	then 
	FCR.sh `pwd`/$i $2 $3 
	fi
done


