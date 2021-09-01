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

typedef int pipe_t[2];


int main(int argc,char **argv){

int pid;
int pidFiglio;
int N;
char c;
char Cz; 
long int pos;
int occ;
int fd;
int status;
int ritorno;
pipe_t *piped; // tra figlio 1 e figlio 2
pipe_t *p; // tra figlio 2 e figlio 1


//controllo parametri


if (argc < 4 ){
printf("numero parametri scorretto pochi \n");
exit(1);
}

N = argc - 2 ;

for ( int i = 0 ; i < N ; ++i ){

if ((fd = open(argv[i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(2);
}

}

if (strlen(argv[argc-1]) != 1){
printf("errore non singolo carattere \n");
exit(3);
}

Cz = argv[argc - 1][0];


//pipe

piped = malloc( N * sizeof(pipe_t));
if (piped == NULL){
printf("errore allocazione pipe \n");
exit(4);
}

p = malloc( N * sizeof(pipe_t));
if (p == NULL){
printf("errore allocazione pipe \n");
exit(5);
}

for ( int i = 0; i < N ; ++i){
if ( pipe(piped[i]) < 0 ){
printf("errore creazione pipe \n");
exit(6);
}

if ( pipe(p[i]) < 0 ){
printf("errore creazione pipe \n");
exit(7);
}
}

// creazione figli

for (int i = 0 ; i < 2*N ; ++i){

if (( pid = fork()  ) < 0){
printf("errore creazione figli \n");
exit(8);
}


//codice figlio

if ( pid == 0){

// nel caso di errore nel figlio ritorno -1

//codice figlio

if ( i < N ){

for ( int j = 0; j < N ; ++j){

close(piped[j][0]);
close(p[j][1]);

if (i != j){
close(piped[j][1]);
close(p[j][0]);
}

}

if ((fd = open(argv[i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}


pos = 0;
occ = 0;
int nr = 0;
long int x;

while ( read(fd,&c,1) ){

if ( c == Cz){

++occ;

pos = lseek(fd,0L,SEEK_CUR) - 1;

write(piped[i][1],&pos,sizeof(long int));

nr = read(p[i][0],&x,sizeof(long int));

if (nr != sizeof(long int) ) {

break;

}
else {

lseek(fd,x+1,SEEK_SET);

}

}


}

 
exit(occ);

}
else{

for ( int j = 0; j < N ; ++j){

close(piped[j][1]);
close(p[j][0]);

if ((2*N -i -1) != j){
close(piped[j][0]);
close(p[j][1]);
}

}

if ((fd = open(argv[2*N - i ],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}


pos = 0;
occ = 0;
int nr = 0;
long int x;

nr = read(piped[2*N - i -1][0],&x,sizeof(long int));

if ( nr != sizeof(long int) ){

exit(0);
}

lseek(fd,x+1,SEEK_SET);


while ( read(fd,&c,1) ){

if ( c == Cz){

++occ;

pos = lseek(fd,0L,SEEK_CUR) - 1;

write(p[2*N - i-1][1],&pos,sizeof(long int));

nr = read(piped[2*N - i-1][0],&x,sizeof(long int));

if (nr != sizeof(long int) ) {

break;

}
else {

lseek(fd,x+1,SEEK_SET);

}


}
}
 
exit(occ);

}
}

}

// codice padre

for ( int j = 0; j < N ; ++j){

close(piped[j][1]);
close(piped[j][0]);
close(p[j][0]);
close(p[j][1]);

}

printf("Il padre riceve il numero di occorrenze trovate dai singoli processi delle coppie di figli \n");

for( int i = 0 ; i < 2*N; ++i){

pidFiglio = wait(&status);
if ( pidFiglio < 0){
printf("errore in wait \n");
exit(9);
}
if ( (status & 0xFF) != 0){
printf ("figlio %d terminato in modo anomalo \n",pidFiglio);
}
else{
status = status >>8;
ritorno = status & 0xFF;
if (ritorno == 255){
printf ( "figlio ha ritornato -1 problemi \n");
}
else
printf ( "figlio %d ha ritornato %d \n",pidFiglio,ritorno);

}

}

exit(0);
}
