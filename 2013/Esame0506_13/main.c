#define _POSIX_SOURCE

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ctype.h>

typedef int pipe_t[2];


int main(int argc,char **argv){

int pid;
int pidFiglio;
int N;
int fd;
char linea[256];
int status;
int ritorno;
pipe_t *piped; // tra padre e figli


//controllo parametri

if (argc < 3){
printf("numero parametri scorretto \n");
exit(1);
}

for ( int i = 0  ; i < argc - 1 ; ++i ){

if ( (fd=open ( argv[i+1] , O_RDONLY ) ) < 0 ){
printf("errore nella apertura del file \n");
exit(2);
}

}

N = argc - 1;

//pipe

piped = malloc( N * sizeof(pipe_t));
if (piped == NULL){
printf("errore allocazione pipe \n");
exit(3);
}

for ( int i = 0; i < N ; ++i){
if ( pipe(piped[i]) < 0 ){
printf("errore creazione pipe \n");
exit(4);
}

}

// creazione figli

for (int i = 0 ; i < N ; ++i){

if (( pid = fork()  ) < 0){
printf("errore creazione figli \n");
exit(5);
}


//codice figlio

if ( pid == 0){

for ( int j = 0; j < N ; ++j){

close(piped[j][0]);
if (i != j){
close(piped[j][1]);
}

}

// nel caso di errore nel figlio ritorno -1

//codice figlio

close(0);

if ((fd = open(argv[i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}

//avrei potuto evitare la close(0) se avessi aggiunto un 
//parametro alla exec con il file
//es -> execlp("head","head","-1",argv[i+1],(char *)0);

close(1);
dup(piped[i][1]);

close(piped[i][1]);


/*
ridirezione stderr per evitare errori a video
close(2);
open("/dev/null",O_WRONLY);
*/

execlp("head","head","-1",(char *)0);

//si esce con -1 solo in caso di errore
exit(-1);
}
}



// codice padre

for ( int j = 0; j < N ; ++j){

close(piped[j][1]);

}


for ( int i = 0 ; i < N ; ++i){

read( piped[i][0] ,linea ,sizeof(linea));

printf("il figlio di indice %d nel file %s ha trovato la linea -> \n %s \n",i,argv[i+1],linea);


}



for( int i = 0 ; i < N; ++i){

pidFiglio = wait(&status);
if ( pidFiglio < 0){
printf("errore in wait \n");
exit(6);
}
if ( (status & 0xFF) != 0){
printf ("figlio %d terminato in modo anomalo \n",pidFiglio);
}
else{
status = status >>8;
ritorno = status & 0xFF;
if (ritorno == 255){
printf ( "figlio ha ritornato -1 ==> problemi \n");
}
else
printf ( "figlio %d ha ritornato %d \n",pidFiglio,ritorno);

}

}

exit(0);
}
