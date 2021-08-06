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
	int pid;
	long int occmax;
	long int occtotale;

} struttura;

int main(int argc,char **argv){

int pid;
int pidFiglio;
int N;
int fd;
char c;
char C1;
char C2;
int status;
int ritorno;
int nr;
long int occ;
struttura cur1;
struttura cur2;
struttura tmp;
pipe_t *piped; // schema di comunicazione a pipeline 
//si legge dalla pipe i-1 e si scrive sulla pipe i
// da figlio 1 a figlio n-1 poi a padre

pipe_t *pipes; // per gli altri N figli

//controllo parametri


if (argc < 5){
printf("numero parametri scorretto \n");
exit(1);
}

N = argc - 3 ;
if ( (N % 2) == 1 ){
printf("numero file dispari \n");
exit(2);
}

if ( N < 2 ){
printf("pochi file \n");
exit(3);
}

for ( int i = 0  ; i < N ; ++i ){

if ( (fd=open ( argv[i+1] , O_RDONLY ) ) < 0 ){
printf("errore nella apertura del file \n");
exit(4);
}

}

C1 =  argv[argc-2][0];
C2 =  argv[argc-1][0];

// creazione array


//pipe

piped = malloc( (N/2) * sizeof(pipe_t));
if (piped == NULL){
printf("errore allocazione pipe \n");
exit(5);
}

pipes = malloc( (N/2) * sizeof(pipe_t));
if (pipes == NULL){
printf("errore allocazione pipe \n");
exit(6);
}

for ( int i = 0; i < N/2 ; ++i){
if ( pipe(piped[i]) < 0 ){
printf("errore creazione pipe \n");
exit(7);
}

}

for ( int i = N/2; i < N ; ++i){
if ( pipe(pipes[i]) < 0 ){
printf("errore creazione pipe \n");
exit(8);
}

}

// creazione figli

for (int i = 0 ; i < N/2 ; ++i){

if (( pid = fork()  ) < 0){
printf("errore creazione figli \n");
exit(9);
}


//codice figlio

if ( pid == 0){

for ( int j = 0 ; j < N/2 ; ++j){

//chiudo in scrittura tutte le pipe eccetto i e in lettura tutte eccetto
//i+1

if ( j != i && j != i +1 ){
close(piped[j][0]);
close(piped[j][1]);

}

}

close(piped[i][0]);
close(piped[i+1][1]);

// nel caso di errore nel figlio ritorno -1

//codice figlio


if ((fd = open(argv[i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}

occ = 0;
nr = 0;

while ( read ( fd,&c,1) ){

if ( c == C1 ){
occ++;
}

}

if ( i == (N/2) -1 ){
cur1.pid = getpid();
cur1.occmax = occ;
cur1.occtotale = occ;
}
else{

nr = read( piped[i+1][0] ,&cur1,sizeof(cur1));

if ( nr != sizeof(cur1) ){
printf("errore lettura \n");
exit(-1);
}

if ( cur1.occmax < occ ){
cur1.occmax = occ;
}

cur1.occtotale += occ ;
}

write ( piped[i][1] , &cur1 ,sizeof(cur1) );

if ( nr != sizeof(cur1) ){
printf("errore scrittura \n");
exit(-1);
}

exit(0);
}
}



for (int i = N/2 ; i < N ; ++i){

if (( pid = fork()  ) < 0){
printf("errore creazione figli \n");
exit(10);
}

if ( pid == 0){

for ( int j = N/2 ; j < N ; ++j){

if ( j != i && j != i +1 ){
close(pipes[j][0]);
close(pipes[j][1]);

}
}

close(pipes[i][0]);
close(pipes[i+1][1]);

// nel caso di errore nel figlio ritorno -1

//codice figlio


if ((fd = open(argv[i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}

occ = 0;
nr = 0;

while ( read ( fd,&c,1) ){

if ( c == C2 ){
occ++;
}

}

if ( i == N -1 ){
cur2.pid = getpid();
cur2.occmax = occ;
cur2.occtotale = occ;
}
else{

nr = read( pipes[i+1][0] ,&cur2,sizeof(cur2));

if ( nr != sizeof(cur2) ){
printf("errore lettura \n");
exit(-1);
}

if ( cur2.occmax < occ ){
cur2.occmax = occ;
}

cur2.occtotale += occ ;
}

write ( pipes[i][1] , &cur2 ,sizeof(cur2) );

if ( nr != sizeof(cur2) ){
printf("errore scrittura \n");
exit(-1);
}

exit(0);
}


}

// codice padre

for ( int j = 0; j < N ; ++j){

close(piped[j][1]);
close(pipes[j][1]);

if ( j != 0  ){
close(piped[j][0]);
close(pipes[j][0]);
} 

}



nr = read(piped[0][0],&tmp,sizeof(struttura) );

if ( nr != sizeof(struttura) ){
printf("errore in lettura \n");
exit(11);
}

printf("il figlio della prima meta con pid %d ha trovato %ld occorrenze e un numero massimo di %ld del carattere %c \n",tmp.pid,tmp.occtotale,tmp.occmax, C1);


nr = read(pipes[0][0],&tmp,sizeof(struttura) );

if ( nr != sizeof(struttura) ){
printf("errore in lettura \n");
exit(12);
}

printf("il figlio della seconda meta con pid %d ha trovato %ld occorrenze e un numero massimo di %ld del carattere %c \n",tmp.pid,tmp.occtotale,tmp.occmax, C2);




for( int i = 0 ; i < N; ++i){

pidFiglio = wait(&status);
if ( pidFiglio < 0){
printf("errore in wait \n");
exit(13);
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
