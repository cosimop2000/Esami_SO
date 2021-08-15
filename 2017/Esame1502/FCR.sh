#!/bin/sh

cd $1

#variabile per il numero di linee
count=

#variabile booleana
trovato=0

#variabile per il numero di linee con un numero 
nl=

#variabile che contiene i nomi assoluti dei file trovati 
files=


#variabile per la conferma da parte dell'utente
conferma=


for i in *
do
	if test -f $i -a -r $i 
	then
		count=`wc -l < $i`
		
		if test $count -eq $2
		then
			
			nl=`grep [0-9] $i | wc -l`
			
			if test $nl -eq $count 
			then
			
				trovato=1
				files="$files `pwd`/$i"
			
			fi
				
		fi
		
	fi
	
done

if test $trovato -eq 1
then
	echo trovata directory `pwd`
	echo $files
	
	echo chiedo allo utente conferma , si o SI
	read conferma

	case $conferma in 
	si|SI) echo invocazione parte C
		main $files $2
	
	;;
	*) echo nessuna conferma
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


