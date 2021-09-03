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
#include <time.h>

#define PERM 0644

typedef int pipe_t[2];


int main(int argc,char **argv){

int pid;
int pidFiglio;
int pidN;
int pidNipote;
int N; 
char Fout[10]; // merge + i che è al massimo 4 cifre + terminatore = 10
char c;
char ch;
int fd;
int fdN;
int fdout;
int status;
int ritorno;
pipe_t piped; // tra padre e figlio
pipe_t p; // tra figlio e nipote


//controllo parametri


if (argc < 3 ){
printf("numero parametri scorretto pochi \n");
exit(1);
}

N = argc - 1  ;

if ( (N % 2) == 1 ){
printf("numero parametri dispari \n");
exit(2);
}

for ( int i = 0 ; i < N ; ++i ){

if ((fd = open(argv[i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(3);
}

}


// creazione figli

for (int i = 0 ; i < N/2 ; ++i){

if (( pid = fork()  ) < 0){
printf("errore creazione figli \n");
exit(4);
}


//codice figlio

if ( pid == 0){


// nel caso di errore nel figlio o nel nipote ritorno -1

if ( pipe(piped) < 0 ){
printf("errore creazione pipe \n");
exit(-1);
}

if ( pipe(p) < 0 ){
printf("errore creazione pipe \n");
exit(-1);
}

// creazione file 

sprintf(Fout,"merge%d",i);

if ((fdout = creat(Fout,PERM)) < 0){
printf("errore creazione file \n");
exit(-1);
}

if ( (pidN = fork()) < 0){
printf("errore creazione nipote \n");
exit(-1);
}

// codice nipote
if ( pidN == 0){

//chiudo i lati delle pipe non utilizzate

close(piped[1]);
close(p[0]);


if ((fdN = open(argv[(N/2) + i + 1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}

/*
if ((fdout = open(Fout,O_RDWR)) <0 ){
printf("errore apertura file \n");
exit(-1);
}
*/

ch = 'x';

while ( read(fdN,&c,1) ){

read(piped[0],&ch,1);

write(fdout,&c,1);

write(p[1],&ch,1);


}


exit(c);
}

//codice figlio

close(p[1]);
close(piped[0]);

if ((fd = open(argv[i + 1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}

ch = 'x';
int primo = 1;


while ( read(fd,&c,1) ){

if (primo == 1){

primo = 0;

} 
else{
read(p[0],&ch,1);
}

write(fdout,&c,1);

write(piped[1],&ch,1);

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
printf ( "nipote %d ha ritornato %d --> %c \n",pidNipote,ritorno,ritorno);

}

//il figlio torna l'ultimo carattere scritto dal nipote sul file creato
exit(ritorno);
}

}

// codice padre

printf("il padre riceve l'ultimo carattere scritto dai nipoti su Fout \n");

for( int i = 0 ; i < N/2; ++i){

pidFiglio = wait(&status);
if ( pidFiglio < 0){
printf("errore in wait \n");
exit(5);
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
printf ( "figlio %d ha ritornato %d --> %c \n",pidFiglio,ritorno,ritorno);

}

}

exit(0);
}
