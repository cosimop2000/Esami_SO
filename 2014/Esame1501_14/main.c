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
char chj;
char cchj;
int N;
int fd;
int fdN;
int status;
int ritorno;

pipe_t *piped; // tra padre e figlio
pipe_t p; // tra figlio e nipote


//controllo parametri

if ( ((argc -1) % 2) != 0 ){
printf("numero parametri scorretto dispari \n");
exit(1);
}

if (argc < 3){
printf("numero parametri scorretto pochi \n");
exit(2);
}

N = (argc -1) / 2 ;

for ( int i = 0 ; i < argc -1 ; ++i ){

if ((fd = open(argv[i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(3);
}

}


//pipe

piped = malloc( N * sizeof(pipe_t));
if (piped == NULL){
printf("errore allocazione pipe \n");
exit(4);
}

for ( int i = 0; i < N ; ++i){
if ( pipe(piped[i]) < 0 ){
printf("errore creazione pipe \n");
exit(5);
}
}


// creazione figli

for (int i = 0 ; i < N ; ++i){

if (( pid = fork()  ) < 0){
printf("errore creazione figli \n");
exit(6);
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

if ((fdN = open(argv[i+N+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}

long int pos = -1*(i+1);

lseek(fdN,pos,SEEK_END);

int nr;
nr = read(fdN,&cchj,1);
if ( nr == 0){
//se la lettura non è andata a buon fine comunico -1
exit(-1);
}

write(p[1],&cchj,1);


//se la lettura è andata a buon fine comunico 0
exit(0);
}

//codice figlio

close(p[1]);

for ( int j = 0; j < N ; ++j){

close(piped[j][0]);
if (i != j){
close(piped[j][1]);

}

}



if ((fd = open(argv[i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}

long int pos = i ;

lseek(fd,pos ,SEEK_SET);

int nr;

nr = read (fd , &chj,1);
if ( nr == 0){
//se la lettura non è andata a buon fine comunico -1
exit(-1);
}

char c;
read ( p[1],&c,1);

if ( c == chj ){

write(piped[i][1],&chj ,1);

}
else{ 
char t = -1;
write(piped[i][1],&t ,1);
}


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

//se la lettura è andata a buon fine comunico 0
exit(0);
}

}

// codice padre

for ( int j = 0; j < N ; ++j){

close(piped[j][1]);
/*
close(p[0]);
close(p[1]);
*/
}

for ( int i = 0 ; i < N ; ++i ){

// se 255 i caratteri trovati da figlio e nipote sono diversi
read(piped[i][0],&chj ,1);

printf("il figlio di indice %d ha trovato il carattere %c come il nipote(se -1 non hanno trovato lo stesso carattere \n",i,chj);

}


for( int i = 0 ; i < N; ++i){

pidFiglio = wait(&status);
if ( pidFiglio < 0){
printf("errore in wait \n");
exit(7);
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
