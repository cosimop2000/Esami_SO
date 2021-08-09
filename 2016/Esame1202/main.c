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
	int c1; // indice processi
	long int c2; // occorrenze carattere Cx

} struttura;

int main(int argc,char **argv){

int *pid;
int pidFiglio;
int N;
int fd;
//long int occ;
char c;
char Cx;
int status;
int ritorno;
int nr;
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

N = argc - 2 ;


for ( int i = 0  ; i < N ; ++i ){

if ( (fd=open ( argv[i+1] , O_RDONLY ) ) < 0 ){
printf("errore nella apertura del file \n");
exit(2);
}

}

Cx = argv[argc - 1][0];
if ( !islower(Cx) || ((nr = strlen(argv[argc -1]) != 1))){
printf("non carattere minuscolo \n");
exit(3);
}

printf("carattere da cercare %c \n",Cx);

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



cur = malloc ((i+1)*sizeof(struttura));
if ( cur == NULL ){
printf("errore allocazione struct \n");
exit(-1);
}

cur[i].c1 = i;
cur[i].c2 = 0;


nr = 0;
//occ = 0;

while ( read(fd,&c,1) ){

if ( c == Cx ){
//occ = occ + 1;

(cur[i].c2)++;
}

}

//cur[i].c2 = occ;

// leggo i struct dalla pipe e ne devo scrivere i+1

if ( i != 0 ){

nr = read(piped[i-1][0],cur,i*sizeof(cur));
if (nr != i*sizeof(cur)){
printf("errore in lettura \n");
exit(-1);
}

}

write(piped[i][1],cur,(i+1)*sizeof(cur));

// dopo invio ripristino i dati


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


tmp = malloc (N*sizeof(struttura));
if ( tmp == NULL ){
printf("errore allocazione struct \n");
exit(8);
}

nr = read(piped[N - 1][0],tmp,N*sizeof(tmp) );

if ( nr != N*sizeof(tmp) ){
printf("errore in lettura \n");
exit(9);
}


for ( int j = 0 ; j < N ; ++j){


printf("il figlio di indice %d di pid %d ha trovato %ld occorrenze del carattere %c nel file %s \n",tmp[j].c1,pid[j],tmp[j].c2,Cx,argv[j+1]);

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
printf ( "figlio ha ritornato -1 ==> problemi \n");
}
else
printf ( "figlio %d ha ritornato %d \n",pidFiglio,ritorno);

}

}

exit(0);
}
