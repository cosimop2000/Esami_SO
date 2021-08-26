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
	int c1; // pid
	int c2; // lunghezza 

} struttura;


void BubbleSort( struttura *v , int dim ){

int i;
bool ordinato = false;
struttura tmp;


while ( dim > 1 && ordinato == false){

ordinato = true;

for ( i = 0 ; i < dim - 1; ++i){

if ( v[i].c2 > v[i+1].c2){

tmp = v[i];
v[i] = v[i+1];
v[i+1] = tmp;


ordinato = false;
}

}

dim--;
}


}

int main(int argc,char **argv){

int pid;
int pidFiglio;
int N;
int fd;
int len;
int Y;
int status;
int ritorno;
int nr;
char linea[256];
struttura *cur;
struttura *tmp;
pipe_t *piped; // schema di comunicazione a pipeline 
//si legge dalla pipe i-1 e si scrive sulla pipe i
// da figlio 1 a figlio n-1 poi a padre


//controllo parametri


if (argc < 4){
printf("numero parametri scorretto \n");
exit(1);
}

N = argc - 2;



for ( int i = 0 ; i < N ; ++i){

if ( (fd=open ( argv[i + 1] , O_RDONLY ) ) < 0 ){
printf("errore nella apertura del file \n");
exit(2);
}

}

Y = atoi(argv[argc -1]);
if ( Y <= 0 ){
printf("errore numero non strettamente positivo \n");
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

// struct e array

cur = malloc( N * sizeof(struttura));
if (cur == NULL){
printf("errore allocazione struct \n");
exit(6);
}

// creazione figli

for (int i = 0 ; i < N ; ++i){

if (( pid = fork()  ) < 0){
printf("errore creazione figli \n");
exit(7);
}


//codice figlio

if ( pid == 0){

for ( int j = 0 ; j < N ; ++j){

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
int j = 0;

tmp = malloc( (i+1) * sizeof(struttura));
if (tmp == NULL){
printf("errore allocazione struct \n");
exit(-1);
}

tmp[i].c1 = getpid();
tmp[i].c2 = len;


while ( read ( fd,&linea[j],1) ){

if ( linea[j] == '\n' ){

++j;
linea[j] = '\0';

len = j;

tmp[i].c1 = getpid();
tmp[i].c2 = len;

if ( i != 0 ){

nr = read ( piped[i - 1][0],tmp,i*sizeof(struttura));

if ( nr != i*sizeof(struttura) ){
printf("errore in lettura \n");
exit(-1);
}


}

write(piped[i][1],tmp,(i+1)*sizeof(struttura));


printf( "il figlio di indice %d e pid %d stampa la linea --> %s \n" ,i,getpid(),linea);


/*
tmp[i].c1 = i;
tmp[i].c2 = 0;
*/

j = 0;

}
else ++j;

}



exit(i);
}
}



// codice padre

for ( int j = 0; j < N ; ++j){

close(piped[j][1]);

if ( j != N - 1  ){
close(piped[j][0]);
} 

}


for ( int j = 0 ; j < Y ; ++j){

nr = read(piped[N-1][0],cur,N*sizeof(struttura));

if ( nr != N*sizeof(struttura)){
printf("errore in lettura \n");
exit(8);
}

BubbleSort( cur , N );

for ( int i = 0 ; i < N ; ++i ){

printf("figlio di pid %d ha trovato la linea %d di lunghezza %d nel file %s \n",cur[i].c1,j+1,cur[i].c2,argv[i+1]);

}

}

for( int i = 0 ; i < N; ++i){

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
printf ( "figlio ha ritornato -1  ==> problemi \n");
}
else
printf ( "figlio %d ha ritornato %d \n",pidFiglio,ritorno);

}

}

exit(0);
}
