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

typedef char L[250];

int main(int argc,char **argv){

int pid;
int pidFiglio;
int N;
int n;
int dim;
int nroLinee;
int fd;
int fdw;
int nr;
int status;
int ritorno;
L linea;
L *tutteLinee;
pipe_t *piped; // comunicazione a pipeline

//controllo parametri


if (argc < 4 ){
printf("numero parametri scorretto \n");
exit(1);
}

N = argc - 2;

for ( n = 0 ; n < N ; ++n){

if ((fd = open(argv[n+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(2);
}

}

if ((fdw = creat("PAVONE",PERM)) <0 ){
printf("errore creazione file \n");
exit(3);
}

nroLinee = atoi(argv[argc - 1]);
if ( nroLinee <= 0 ){
printf("errore numero non strettamente positivo \n");
exit(4);
}

//pipe

piped = malloc( N * sizeof(pipe_t));
if (piped == NULL){
printf("errore allocazione pipe \n");
exit(5);
}


for ( n = 0; n < N ; ++n){
if ( pipe(piped[n]) < 0 ){
printf("errore creazione pipe \n");
exit(6);
}

}

// array

tutteLinee = malloc( N * sizeof(L));
if (tutteLinee == NULL){
printf("errore allocazione vettore di linee \n");
exit(7);
}


// creazione figli

for (n = 0 ; n < N ; ++n){

if (( pid = fork()  ) < 0){
printf("errore creazione figli \n");
exit(8);
}


//codice figlio

if ( pid == 0){


// nel caso di errore nel figlio ritorno -1


//codice figlio

for ( int j = 0; j < N ; ++j){

//chiudo in scrittura tutte le pipe eccetto n e in lettura tutte eccetto
//n-1

if ( j != n  ){
close(piped[j][1]);
}

if ( (n == 0) || (j != n - 1)){
close(piped[j][0]);
}



}


if ((fd = open(argv[n+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}


int j = 0;
dim = 0;

while (read(fd,&(linea[j]),1)){


if ( linea[j] == '\n' ){

if ( n != 0 ){
nr = read(piped[n-1][0],tutteLinee,N * sizeof(L));

if ( nr != N * sizeof(L) ){
printf("errore lettura \n");
exit(-1);
}

}

linea[j+1] = '\0';

printf("il figlio con indice %d e pid %d ha letto la linea %s \n",n,getpid(),linea);

dim = j+1;

strcpy(tutteLinee[n],linea);

tutteLinee[n][dim] = '\0';

write(piped[n][1],tutteLinee,N * sizeof(L));


j = 0;

}

else{
++j;
}

}

exit(dim);
}

}


// codice padre

for ( int j = 0; j < N ; ++j){

close(piped[j][1]);

if ( j != N - 1  ){
close(piped[j][0]);
} 

}

for ( int i = 1 ; i <= nroLinee ; ++i ){


nr = read(piped[N-1][0],tutteLinee,N * sizeof(L));

if ( nr != N * sizeof(L) ){
printf("errore lettura \n");
exit(9);
}


for ( int k = 0 ; k < N ; ++k){
printf("linea numero %d %s \n",i,tutteLinee[k]);

write(fdw,tutteLinee[k], sizeof(L));

}


}

for( n = 0 ; n < N; ++n){

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
