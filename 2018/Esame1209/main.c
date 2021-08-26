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
int N;
char c;
char ch; 
long int tr;
int fd;
int fdN;
int status;
int ritorno;
pipe_t *piped; // tra padre e figlio
pipe_t *p; // tra padre e nipote


//controllo parametri


if (argc < 3 ){
printf("numero parametri scorretto pochi \n");
exit(1);
}


N = argc -1 ;

for ( int i = 0 ; i < N ; ++i ){

if ((fd = open(argv[i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(2);
}

}


//pipe

piped = malloc( N * sizeof(pipe_t));
if (piped == NULL){
printf("errore allocazione pipe \n");
exit(3);
}

p = malloc( N * sizeof(pipe_t));
if (p == NULL){
printf("errore allocazione pipe \n");
exit(4);
}

for ( int i = 0; i < N ; ++i){

if ( pipe(piped[i]) < 0 ){
printf("errore creazione pipe \n");
exit(5);
}
if ( pipe(p[i]) < 0 ){
printf("errore creazione pipe \n");
exit(6);
}
}


// creazione figli

for (int i = 0 ; i < N ; ++i){

if (( pid = fork()  ) < 0){
printf("errore creazione figli \n");
exit(7);
}


//codice figlio

if ( pid == 0){


// nel caso di errore nel figlio o nel nipote ritorno -1


if ( (pidN = fork()) < 0){
printf("errore creazione nipote \n");
exit(-1);
}

// codice nipote
if ( pidN == 0){

//chiudo i lati delle pipe non utilizzate

close(piped[i][1]);

for ( int j = 0; j < N ; ++j){

close(p[j][0]);

if (i != j){
close(p[j][1]);
}

}


if ((fdN = open(argv[i + 1],O_RDWR)) <0 ){
printf("errore apertura file \n");
exit(-1);
}

tr = 0;

while( read(fdN,&c,1) ){

if ( islower(c) ){

tr++;

ch = toupper(c);

lseek(fdN,-1L,SEEK_CUR);

write(fdN,&ch,1);

}

}

write(p[i][1],&tr,sizeof(long int));


ritorno = tr % 256;
exit(ritorno);
}

//codice figlio


for ( int j = 0; j < N ; ++j){

close(piped[j][0]);
close(p[j][0]);
close(p[j][1]);

if (i != j){
close(piped[j][1]);
}

}


if ((fd = open(argv[i + 1],O_RDWR)) <0 ){
printf("errore apertura file \n");
exit(-1);
}

ch = ' ';
tr = 0;

while ( read(fd,&c,1) ){

if ( isdigit(c) ){

tr++;

lseek(fd,-1L,SEEK_CUR);

write(fd,&ch,1);

}

}



write(piped[i][1],&tr,sizeof(long int));

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

ritorno = tr % 256;
exit(ritorno);
}

}

// codice padre

for ( int j = 0; j < N ; ++j){

close(piped[j][1]);
close(p[j][1]);

}


for ( int i = 0  ; i < N ; ++i ){

read( piped[i][0],&tr,sizeof(long int));

printf("il figlio di indice %d ha effetuato %ld trasformazioni nel file %s --> SPAZI \n",i,tr,argv[i + 1]);

read( p[i][0],&tr,sizeof(long int));

printf("il nipote di indice %d ha effetuato %ld trasformazioni nel file %s --> MAIUSCOLE \n",i,tr,argv[i + 1]);

}



for( int i = 0 ; i < N; ++i){

pidFiglio = wait(&status);
if ( pidFiglio < 0){
printf("errore in wait \n");
exit(8);
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
