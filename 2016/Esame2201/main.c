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

typedef struct {
	char chr;
	long int occ;
	char proc;
	int pid;

} struttura;

int main(int argc,char **argv){

int pid;
int pidFiglio;
int pidN;
int pidNipote;
int N; 
long int x; // num occ nel nipote
char c;
char AM;
int nr;
int fd;
int fdN;
int status;
int ritorno;
struttura cur;
struttura tmp;
pipe_t *piped; // tra padre e figlio
pipe_t p1,p2; // tra figlio e nipote


//controllo parametri


if (argc < 5 ){
printf("numero parametri scorretto pochi \n");
exit(1);
}

if (((argc - 1) % 2) == 1 ) {
printf("numero parametri dispari \n");
exit(2);
}

N = (argc - 1) / 2 ;

for ( int i = 0 ; i < argc - 1 ; ++i ){

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

if ( pipe(p1) < 0 ){
printf("errore creazione pipe \n");
exit(-1);
}

if ( pipe(p2) < 0 ){
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
close(p1[1]);
close(p2[0]);

if ((fdN = open(argv[i + N + 1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}

read(p1[0],&AM,1);

x=0;

while( (nr = read(fdN,&c,1)) != 0){

if ( c == AM ){
++x;
}

}

write(p2[1],&x,sizeof(long int));

exit(nr);
}

//codice figlio

close(p1[0]);
close(p2[1]);

for ( int j = 0; j < N ; ++j){

close(piped[j][0]);
if (i != j){
close(piped[j][1]);

}

}

if ((fd = open(argv[i + 1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}

long int count = 0;
long int k = 0;
// se non trovo un carattere maiuscolo cerco un altro carattere
AM='?';

while ( (ritorno = read(fd,&c,1)) != 0 ){

if ( isupper(c) > 0 ){

if (count == 0){
AM=c;
}

++count;
}

}

write(p1[1],&AM,1);

read(p2[0],&k,sizeof(long int));

if (count < k ) {

cur.chr = AM;
cur.occ = k;
cur.proc = 'N';
cur.pid = pid;

}
else{

cur.chr = AM;
cur.occ = count;
cur.proc = 'F';
cur.pid = getpid();

}

write(piped[i][1],&cur,sizeof(struttura));

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

for ( int j = 0; j < N ; ++j){

close(piped[j][1]);

}


for ( int i = 0 ; i < N ; ++i ){

nr = read(piped[i][0],&tmp,sizeof(struttura));
if ( nr != sizeof(struttura) ){
printf("errore in lettura \n");
exit(7);
}

if ( tmp.proc == 'F' ){

printf("%c di indice %d e pid %d ha trovato il carattere %c %ld volte nel file %s\n",tmp.proc,i,tmp.pid,tmp.chr,tmp.occ,argv[i+1]);
}
else{

printf("%c associato al figlio di indice %d e pid %d ha trovato il carattere %c %ld volte nel file %s\n",tmp.proc,i,tmp.pid,tmp.chr,tmp.occ,argv[i+N+1]);
}

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
