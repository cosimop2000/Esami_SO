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
#include <stdbool.h>

typedef int pipe_t[2];

typedef struct {
	long int c1; // max linee
	int c2; // indice 

} struttura;


int main(int argc,char **argv){

int *pid;
int pidFiglio;
int M;
int fd;
long int len;
char c;
int status;
int ritorno;
int nr;
struttura cur;
struttura tmp;
pipe_t *piped; // schema di comunicazione a pipeline 
//si legge dalla pipe i-1 e si scrive sulla pipe i
// da figlio 1 a figlio n-1 poi a padre


//controllo parametri


if (argc < 2){
printf("numero parametri scorretto \n");
exit(1);
}

M = argc - 1;

for ( int i = 0 ; i < M ; ++i){

if ( (fd=open ( argv[i + 1] , O_RDONLY ) ) < 0 ){
printf("errore nella apertura del file \n");
exit(2);
}

}

// array

pid = malloc( M * sizeof(int));
if (pid == NULL){
printf("errore allocazione array di pid \n");
exit(3);
}


//pipe

piped = malloc( M * sizeof(pipe_t));
if (piped == NULL){
printf("errore allocazione pipe \n");
exit(4);
}


for ( int i = 0; i < M ; ++i){

if ( pipe(piped[i]) < 0 ){
printf("errore creazione pipe \n");
exit(5);
}

}

// creazione figli

for (int i = 0 ; i < M ; ++i){

if (( pid[i] = fork()  ) < 0){
printf("errore creazione figli \n");
exit(6);
}


//codice figlio

if ( pid[i] == 0){

for ( int j = 0 ; j < M ; ++j){

//chiudo in scrittura tutte le pipe eccetto i e in lettura tutte eccetto
//i-1

if ( j != i  ){
close(piped[j][1]);
}

if ( (i == 0) || (j != i - 1)){
close(piped[j][0]);
}

}


// nel caso di errore nel figlio ritorno -1

//codice figlio


if ((fd = open(argv[i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}

nr = 0;
len = 0;

while ( read ( fd,&c,1) ){

if ( c == '\n' ){

++len;

}

}

tmp.c1 = len;
tmp.c2 = i;


if ( i != 0 ){

nr = read ( piped[i - 1][0],&tmp,sizeof(struttura));

if ( nr != sizeof(struttura) ){
printf("errore in lettura \n");
exit(-1);
}

if ( len > tmp.c1 ){

tmp.c1 = len;
tmp.c2 = i;

}

}


write(piped[i][1],&tmp,sizeof(struttura));


exit(i);
}
}



// codice padre

for ( int j = 0; j < M ; ++j){

close(piped[j][1]);

if ( j != M - 1  ){
close(piped[j][0]);
} 

}


nr = read(piped[M-1][0],&cur,sizeof(struttura));

if ( nr != sizeof(struttura)){
printf("errore in lettura \n");
exit(7);
}

printf("figlio di pid %d e indice %d ha trovato un numero massimo di linee %ld nel file %s \n",pid[cur.c2],cur.c2,cur.c1,argv[cur.c2 + 1]);


for( int i = 0 ; i < M; ++i){

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
printf ( "figlio ha ritornato -1  ==> problemi \n");
}
else
printf ( "figlio %d ha ritornato %d \n",pidFiglio,ritorno);

}

}

exit(0);
}
