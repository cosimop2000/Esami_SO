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
	int c1; // indice
	int c2; // occorrenze 

} struttura;


int main(int argc,char **argv){

int pid;
int pidFiglio;
int N;
int fd;
int occ;
char c;
char Cx;
int H;
int status;
int ritorno;
int nr;
char linea[250];
struttura cur;
struttura tmp;
pipe_t *piped; // schema di comunicazione a pipeline 
//si legge dalla pipe i-1 e si scrive sulla pipe i
// da figlio 1 a figlio n-1 poi a padre
pipe_t *p; // per la comunicazione padre figlio


//controllo parametri


if (argc < 4){
printf("numero parametri scorretto \n");
exit(1);
}

N = argc - 3;

for ( int i = 0 ; i < N ; ++i){

if ( (fd=open ( argv[i + 1] , O_RDONLY ) ) < 0 ){
printf("errore nella apertura del file \n");
exit(2);
}

}

if ( strlen(argv[argc - 2]) != 1){
printf("errore non singolo carattere \n");
exit(3);

}

Cx = argv[argc - 2][0];

if ( (H = atoi(argv[argc -1]) ) <= 0 ){
printf("errore numero non strettamente positivo \n");
exit(4);
}


//pipe

piped = malloc( N * sizeof(pipe_t));
if (piped == NULL){
printf("errore allocazione pipe \n");
exit(5);
}

p = malloc( N * sizeof(pipe_t));
if (p == NULL){
printf("errore allocazione pipe \n");
exit(6);
}


for ( int i = 0; i < N ; ++i){
if ( pipe(piped[i]) < 0 ){
printf("errore creazione pipe \n");
exit(7);
}

if ( pipe(p[i]) < 0 ){
printf("errore creazione pipe \n");
exit(8);
}

}


// creazione figli

for (int i = 0 ; i < N ; ++i){

if (( pid = fork()  ) < 0){
printf("errore creazione figli \n");
exit(9);
}


//codice figlio

if ( pid == 0){

for ( int j = 0 ; j < N ; ++j){

//chiudo in scrittura tutte le pipe eccetto i e in lettura tutte eccetto
//i-1

if ( j != i  ){
close(piped[j][1]);
close(p[j][0]);
}

if ( (i == 0) || (j != i - 1)){
close(piped[j][0]);
}

close(p[j][1]);

}


// nel caso di errore nel figlio ritorno -1

//codice figlio


if ((fd = open(argv[i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}

nr = 0;
occ = 0;
int count = 0;
int k = 0;
int j = 0;

cur.c1 = i;
cur.c2 = occ;

while ( read ( fd,&linea[k],1) ){

if ( linea[k] == Cx ){
cur.c2 = cur.c2 + 1 ;
++k;
}
else if ( linea[k] == '\n' ){

++j;
++k;
linea[k] = '\0';

if ( i != 0 ){

nr = read ( piped[i - 1][0],&tmp,sizeof(struttura));

if ( nr != sizeof(struttura) ){
printf("errore in lettura \n");
exit(-1);
}

if ( cur.c2 < tmp.c2 ){

cur.c1 = tmp.c1;
cur.c2 = tmp.c2;

}

}

write(piped[i][1],&cur,sizeof(struttura));

read( p[i][0], &c , 1);
if ( c == 't'){

printf( "il figlio di indice %d e pid %d stampa la linea %d --> %s \n" ,i,getpid(),j,linea);

count++;
}

cur.c1 = i;
cur.c2 = 0;
k = 0;

}
else ++k;

}



exit(count);
}
}



// codice padre

for ( int j = 0; j < N ; ++j){

close(piped[j][1]);
close(p[j][0]);

if ( j != N - 1  ){
close(piped[j][0]);
} 

}


for ( int j = 0 ; j < H ; ++j){

nr = read(piped[N-1][0],&cur,sizeof(struttura));

if ( nr != sizeof(struttura)){
printf("errore in lettura \n");
exit(10);
}

printf("figlio di indice %d ha trovato %d occorrenze del carattere %c \n",cur.c1,cur.c2,Cx);

for ( int i = 0 ; i < N ; ++i ){

if ( i == cur.c1 ){
c = 't';

write(p[i][1],&c ,1);

}
else{
c = 'n';

write(p[i][1],&c ,1);
}

}

}

for( int i = 0 ; i < N; ++i){

pidFiglio = wait(&status);
if ( pidFiglio < 0){
printf("errore in wait \n");
exit(11);
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
