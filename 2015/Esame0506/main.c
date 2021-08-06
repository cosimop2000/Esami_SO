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
int pidN;
int pidNipote;
int M; 
int len;
int fd;
int fdN;
int status;
int ritorno;
char linea[256];
pipe_t *piped; // tra padre e figlio
pipe_t p; // tra figlio e nipote


//controllo parametri


if (argc < 3 ){
printf("numero parametri scorretto pochi \n");
exit(1);
}


M = argc -1 ;

for ( int i = 0 ; i < M ; ++i ){

if ((fd = open(argv[i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(2);
}

}


//pipe

piped = malloc( M * sizeof(pipe_t));
if (piped == NULL){
printf("errore allocazione pipe \n");
exit(3);
}

for ( int i = 0; i < M ; ++i){
if ( pipe(piped[i]) < 0 ){
printf("errore creazione pipe \n");
exit(4);
}
}


// creazione figli

for (int i = 0 ; i < M ; ++i){

if (( pid = fork()  ) < 0){
printf("errore creazione figli \n");
exit(5);
}


//codice figlio

if ( pid == 0){


// nel caso di errore nel figlio o nel nipote ritorno -1

if ( pipe(p) < 0 ){
printf("errore creazione pipe \n");
exit(-1);
}

if ( (pidN = fork()) < 0){
printf("errore creazione nipote \n");
exit(-1);
}

// codice nipote
if ( pidN == 0){

//chiudo i lati delle pipe non utilizzate

close(piped[i][1]);
close(piped[i][0]);
close(p[0]);

close(0);


if ((fdN = open(argv[i + 1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}


close(1);
dup(p[1]);

close(p[1]);

close(2);
open("/dev/null",O_WRONLY);


execlp("tail","tail","-1",(char *)0);


// esco con -1 solo in caso di errore
exit(-1);
}

//codice figlio

close(p[1]);

for ( int j = 0; j < M ; ++j){

close(piped[j][0]);
if (i != j){
close(piped[j][1]);

}

}

char c;
int j = 0;
len = 0;

while ( read(p[0],&c,1) ){

if ( c == '\n'){

linea[j+1] = '\0';

printf("il figlio %d ha ricevuto la linea %s \n",getpid(),linea);
len = j ;


}

linea[j] = c;

++j;
}



write(piped[i][1],&len,len);

//aspetto il nipote

pidNipote = wait(&status);
if ( pidNipote < 0){
printf("errore in wait \n");
exit(-1);
}
if ( (status & 0xFF) != 0){
printf ("nipote %d terminato in modo anomalo \n",pidNipote);
}
else{
status = status >> 8;
ritorno = status & 0xFF;
printf ( "nipote %d ha ritornato %d \n",pidNipote,ritorno);

}

//il figlio torna il valore tornato dal nipote
exit(ritorno);
}

}

// codice padre

for ( int j = 0; j < M ; ++j){

close(piped[j][1]);

}


for ( int i = M -1  ; i >= 0 ; --i ){

read( piped[i][0],&len,sizeof(int));

printf("il figlio di indice %d ha trovato lunghezza %d per l'ultima linea del file %s \n",i,len,argv[i + 1]);

}



for( int i = 0 ; i < M; ++i){

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
printf ( "figlio o nipote hanno ritornato -1 problemi \n");
}
else
printf ( "figlio %d ha ritornato %d \n",pidFiglio,ritorno);

}

}

exit(0);
}
