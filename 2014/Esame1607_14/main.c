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
int  X; // numero di linee all interno dei file
int fd;
char c;
char ch; // contiene il primo carattere di una linea
int status;
int ritorno;
char *primiCar;
int count; //contatore di linea
pipe_t *piped; // schema di comunicazione a pipeline 
//si legge dalla pipe i-1 e si scrive sulla pipe i
// da figlio 1 a figlio n-1 poi a padre


//controllo parametri

if (argc < 3){
printf("numero parametri scorretto \n");
exit(1);
}

for ( int i = 0  ; i < argc - 2 ; ++i ){

if ( (fd=open ( argv[i+1] , O_RDONLY ) ) < 0 ){
printf("errore nella apertura del file \n");
exit(2);
}

}

N = argc - 2;

X = atoi( argv[argc-1]);


// creazione array

primiCar = malloc( N * sizeof(char));
if ( primiCar == NULL ){
printf("errore nella allocazione della memoria \n");
exit(3);
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

for ( int j = 0 ; j < N ; ++j){

//chiudo in scrittura tutte le pipe eccetto i e in lettura tutte eccetto
//i-1

if ( j != i ){
close(piped[j][1]);
}

if ( (i == 0) || (i != j -1)){
close(piped[j][0]);
}
}

// nel caso di errore nel figlio ritorno -1

//codice figlio


if ((fd = open(argv[i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}

count = 1;
int j = 0 ;
int nr;

while ( read ( fd,&c,1) > 0){

if ( j == 0){
ch = c;
}

if ( c == '\n' ){


if ( i != 0){


nr = read(piped[i-1][0] , primiCar , N);

if ( nr != N){
printf("errore in lettura \n");
exit(-1);
}

}

primiCar[i] = ch ;
count++;

nr = write(piped[i][1] , primiCar, N);

if ( nr != N){
printf("errore in scrittura \n");
exit(-1);
}

j = 0;
}
else{

++j;

}

}



exit(ch);
}
}



// codice padre

for ( int j = 0; j < N ; ++j){

close(piped[j][1]);

if ( j != N - 1  ){
close(piped[j][0]);
} 

}

// int nr ;

for ( int j = 0 ; j < X ; ++j ){

read (piped[N-1][0] , primiCar ,N );

/*if ( nr != N ){
printf("errore in lettura \n");
exit(7);
}
*/

printf( "Nella linea %d sono stati trovati i caratteri ----> \n",j+1 );

for ( int i = 0 ; i < N ; ++i){

printf("%c per il processo figlio di indice %d e per il file %s \n",primiCar[i],i,argv[i+1]);

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
printf ( "figlio ha ritornato -1 ==> problemi \n");
}
else
printf ( "figlio %d ha ritornato %d \n",pidFiglio,ritorno);

}

}

exit(0);
}
