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
	int indice;
	int occ;

} struttura;

int main(int argc,char **argv){

int *pid;
int pidFiglio;
int N;
int fd;
int H;
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

N = argc - 2 ;


for ( int i = 0  ; i < N ; ++i ){

if ( (fd=open ( argv[i+1] , O_RDONLY ) ) < 0 ){
printf("errore nella apertura del file \n");
exit(2);
}

}

H = atoi(argv[argc - 1]);
if ( H <= 0 ){
printf("numero non positivo \n");
exit(3);
}

printf("chiedo all'utente un carattere c \n");
scanf("%c",&Cx);


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


// nel caso di errore nel figlio ritorno un valore maggiore di 0 --> 90

//codice figlio


if ((fd = open(argv[i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(90);
}

nr = 0;
int occ = 0;
cur.indice = i;
cur.occ = occ;

while ( read ( fd,&c,1) ){

if ( c == Cx ){
cur.occ = cur.occ + 1 ;
}

if ( c == '\n'){

if ( i != 0 ){

nr = read(piped[i-1][0],&tmp,sizeof(struttura));
if ( nr != sizeof(struttura) ){
printf("errore lettura \n");
exit(90);
}

if ( cur.occ > tmp.occ ){
cur.indice = tmp.indice;
cur.occ = tmp.occ;

}
}

write(piped[i][1],&cur,sizeof(struttura));

// dopo invio ripristino i dati

cur.indice = i;
cur.occ = 0;
}

}


exit(0);
}
}





// codice padre

for ( int j = 0; j < N ; ++j){

close(piped[j][1]);

if ( j != N - 1  ){
close(piped[j][0]);
} 

}

for ( int j = 0 ; j < H ; ++j){

nr = read(piped[N - 1][0],&tmp,sizeof(struttura) );

if ( nr != sizeof(struttura) ){
printf("errore in lettura \n");
exit(8);
}

printf("il figlio di indice %d di pid %d ha letto la linea %d e ha trovato %d occorrenze ( il numero minore ) del carattere %c \n",tmp.indice,pid[tmp.indice],j+1, tmp.occ , Cx);

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
printf ( "figlio %d ha ritornato %d \n",pidFiglio,ritorno);

}

}

exit(0);
}
