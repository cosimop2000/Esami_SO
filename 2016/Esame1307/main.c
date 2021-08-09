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
int N;
int H;
char ch; 
char c;
int nr;
int fd;
int status;
int ritorno;
int *x; // array di divisori 
pipe_t *piped; // tra padre e figlio
pipe_t *p; // tra figlio e padre


//controllo parametri


if (argc < 3 ){
printf("numero parametri scorretto pochi \n");
exit(1);
}

if (((argc-1)%2) == 1) {
printf("numero parametri dispari \n");
exit(2);
}

N = (argc - 1) / 2;

for ( int i = 0 ; i < N ; ++i ){

if ((fd = open(argv[2*i+1],O_RDONLY)) <0 ){
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

x = malloc( N * sizeof(int));
if (x == NULL){
printf("errore allocazione array \n");
exit(7);
}


// creazione figli

for (int i = 0 ; i < N ; ++i){

if (( pid = fork()  ) < 0){
printf("errore creazione figli \n");
exit(8);
}


//codice figlio

if ( pid == 0){


// nel caso di errore nel figlio ritorno -1

//codice figlio

for ( int j = 0; j < N ; ++j){

close(piped[j][0]);
close(p[j][1]);

if (i != j){
close(piped[j][1]);
close(p[j][0]);

}

}

if ((fd = open(argv[2*i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}


int count= 0;
int j = 1;
int k = 1;

read(p[i][0],&H,sizeof(int));

while ( read(fd,&c,1) ){

if ( j == H || j == H*k){

++count;
write(piped[i][1],&c,1);

++k;
}

 
++j;
}



//il figlio torna il numero di caratteri 
exit(count);
}

}

// codice padre

for ( int j = 0; j < N ; ++j){

close(piped[j][1]);
close(p[j][0]);

}


for ( int i = 0 ; i < N ; ++i){

scanf("%d",&(x[i]));

if ( (x[i] <= 0) || ((atoi(argv[2*i +2]) % x[i]) != 0) ){
printf("non divisore \n");
exit(9);
}

write(p[i][1],&(x[i]),sizeof(int));

}

int finito = 0;
int j = 1;

while (finito == 0){

finito = 1;

for ( int i = 0 ; i < N ; ++i){

nr = read(piped[i][0],&ch,1);

if ( nr != 0 ){

finito = 0;

printf("il figlio di indice %d ha trovato nel file %s in posizione %d il carattere %c \n",i,argv[2*i +1],x[i]*j,ch);

}
}

++j;
}

for( int i = 0 ; i < N; ++i){

pidFiglio = wait(&status);
if ( pidFiglio < 0){
printf("errore in wait \n");
exit(10);
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
