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
	long int c1; // valore massimo occorrenze
	int c2; // indice
	long int c3; // somma occorrenze 

} struttura;


int main(int argc,char **argv){

int *pid;
int pidFiglio;
int N;
int fd;
long int occ;
char c;
char Cx;
int status;
int ritorno;
int nr;
struttura cur;
struttura tmp;
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

if ( strlen(argv[argc - 1]) != 1){
printf("errore non singolo carattere \n");
exit(3);

}

Cx = argv[argc - 1][0];

// array

pid = malloc( N * sizeof(int));
if (pid == NULL){
printf("errore allocazione vettore di pid \n");
exit(4);
}
//pipe

piped = malloc( N * sizeof(pipe_t));
if (piped == NULL){
printf("errore allocazione pipe \n");
exit(5);
}


for ( int i = 0; i < N ; ++i){
if ( pipe(piped[i]) < 0 ){
printf("errore creazione pipe \n");
exit(6);
}

}


// creazione figli

for (int i = 0 ; i < N ; ++i){

if (( pid[i] = fork()  ) < 0){
printf("errore creazione figli \n");
exit(7);
}


//codice figlio

if ( pid[i] == 0){

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
occ = 0;

while ( read ( fd,&c,1) ){

if ( c == Cx ){
occ = occ + 1 ;
}

}

if ( i == 0 ){

cur.c1 = occ,
cur.c2 = i;
cur.c3 = occ;

}

if ( i != 0 ){

nr = read(piped[i-1][0],&tmp, sizeof(struttura));
if ( nr !=  sizeof(struttura) ){
printf("errore lettura \n");
exit(-1);
}

cur.c3 = tmp.c3 + occ ;

if ( occ > tmp.c1 ){

cur.c1 = occ;
cur.c2 = i;

}
else{

cur.c1 = tmp.c1;
cur.c2 = tmp.c2;

}


}


printf("il figlio di pid %d ha trovato %ld occorrenze del carattere %c \n",getpid(),occ,Cx);


write(piped[i][1],&cur, sizeof(struttura));


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


nr = read(piped[N - 1][0],&cur, sizeof(struttura) );

if ( nr != sizeof(struttura) ){
printf("errore in lettura \n");
exit(8);
}


printf("il figlio di indice %d di pid %d ha trovato %ld occorrenze massime nel file %s con %ld occorrenze totali \n",cur.c2,pid[cur.c2],cur.c1,argv[cur.c2 + 1],cur.c3);

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
