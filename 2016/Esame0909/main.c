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
	char v1; // carattere
	long int v2; // occ

} struttura;

void bubbleSort ( struttura *cur , int dim){

int i;
bool ordinato = false;
struttura tmp;

while ( dim > 1 && ordinato == false){

ordinato = true;

for(i = 0 ; i < dim -1 ; ++i){

if (cur[i].v2 > cur[i+1].v2){

tmp = cur[i+1];
cur[i+1]= cur[i];
cur[i] = tmp;
 
ordinato = false;
}

}

dim--;
}

}

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
char lettere[30] = "abcdefghijklmnopqrstuvwxyz";
struttura *cur;
pipe_t *piped; // schema di comunicazione a pipeline 
//si legge dalla pipe i-1 e si scrive sulla pipe i
// da figlio 1 a figlio n-1 poi a padre


//controllo parametri


if (argc != 2){
printf("numero parametri scorretto \n");
exit(1);
}

N = 26;

if ( (fd=open ( argv[argc - 1] , O_RDONLY ) ) < 0 ){
printf("errore nella apertura del file \n");
exit(2);
}


// array

pid = malloc( N * sizeof(int));
if (pid == NULL){
printf("errore allocazione vettore di pid \n");
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


cur = malloc( N * sizeof(struttura));
if (cur == NULL){
printf("errore allocazione struct \n");
exit(6);
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


if ((fd = open(argv[1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}

nr = 0;
occ = 0;
Cx = lettere[i];
cur[i].v1 = Cx;
cur[i].v2 = occ;

while ( read ( fd,&c,1) ){

if ( c == Cx ){
occ = occ + 1 ;
}


}


if ( i != 0 ){

nr = read(piped[i-1][0],cur,i * sizeof(struttura));
if ( nr != i * sizeof(struttura) ){
printf("errore lettura \n");
exit(-1);
}


}

cur[i].v1 = Cx;
cur[i].v2 = occ;

printf("il figlio di pid %d ha trovato %ld occorrenze del carattere %c \n",getpid(),cur[i].v2,cur[i].v1);

write(piped[i][1],cur,(i+1) * sizeof(struttura));



exit(c);
}
}



// codice padre

for ( int j = 0; j < N ; ++j){

close(piped[j][1]);

if ( j != N - 1  ){
close(piped[j][0]);
} 

}


nr = read(piped[N - 1][0],cur, N * sizeof(struttura) );

if ( nr != N * sizeof(struttura) ){
printf("errore in lettura \n");
exit(8);
}

bubbleSort( cur,N);

for ( int j = 0 ; j < N ; ++j){

printf("il figlio di indice %d di pid %d ha trovato il carattere %c con %ld occorrenze \n",lettere[j] - 'a',pid[lettere[j] - 'a'],cur[j].v1 ,cur[j].v2 );

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
printf ( "figlio ha ritornato 90 ==> problemi \n");
}
else
printf ( "figlio %d ha ritornato %d %c \n",pidFiglio,ritorno,ritorno);

}

}

exit(0);
}
