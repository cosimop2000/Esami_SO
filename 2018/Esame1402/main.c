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

typedef int pipe_t[2];


int mia_random(int n){

int casuale;

casuale = rand() % n;

return casuale;

}

int main(int argc,char **argv){

int *pid;
int pidFiglio;
int N; 
int H;
int cas;
int fd;
int status;
int ritorno;
char linea[256];
pipe_t *piped; // tra padre e figlio
pipe_t *p; // tra figlio e padre


//controllo parametri


if (argc < 4 ){
printf("numero parametri scorretto pochi \n");
exit(1);
}

N = argc - 2 ;

for ( int i = 0 ; i < N ; ++i ){

if ((fd = open(argv[i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(2);
}

}


H = atoi(argv[argc-1]);
if (H <= 0 || H >= 255 ){
printf("numero non positivo o maggiore di 255 \n");
exit(3);
}

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

p = malloc( N * sizeof(pipe_t));
if (p == NULL){
printf("errore allocazione pipe \n");
exit(6);
}

for ( int i = 0; i < N ; ++i){
if ( pipe(piped[i]) < 0 ){
printf("errore creazione pipe \n");
exit(6);
}

if ( pipe(p[i]) < 0 ){
printf("errore creazione pipe \n");
exit(7);
}
}

srand(time(NULL));

// creazione figli

for (int i = 0 ; i < N ; ++i){

if (( pid[i] = fork()  ) < 0){
printf("errore creazione figli \n");
exit(8);
}


//codice figlio

if ( pid[i] == 0){


// nel caso di errore nel figlio ritorno -1


//codice figlio

for ( int j = 0; j < N ; ++j){

close(piped[j][0]);
close(p[j][1]);

if (i != j){
close(piped[j][1]);
close(p[j][0]);
}

}


if ((fd = open(argv[i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}



int j = 0;
int count = 0;
int len = 0;
int nlinea = 0;

while ( read(fd,&(linea[j]),1) ){

if ( linea[j] == '\n'){

linea[j+1] = '\0';

++nlinea;
len = j+1;

printf("il figlio %d ha ricevuto la linea %s",getpid(),linea);

j = 0;


write(piped[i][1], &len,sizeof(len));

read(p[j][0],&cas,sizeof(cas));

++count;

printf("il figlio %d di pid %d ha ricevuto indice %d per la linea %d nel file %s ----> carattere %c \n",i,getpid(),cas,nlinea,argv[i+1],linea[cas]);

}
else
{
++j;
}
}

 
exit(count);
}

}

// codice padre

for ( int j = 0; j < N ; ++j){

close(piped[j][1]);
close(p[j][0]);

}


int valore;
int giusto;

for ( int j = 0 ; j < H ; ++j ){

giusto = 1000;

for ( int i = 0  ; i < N ; ++i ){

read( piped[i][0],&valore,sizeof(int));

if ( valore < giusto ){

giusto = valore;

}

}

cas = mia_random(giusto);

for ( int k = 0 ; k < N ; ++k ){

write(p[k][1],&cas, sizeof(cas));

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
printf ( "figlio ha ritornato -1 problemi \n");
}
else
printf ( "figlio %d ha ritornato %d \n",pidFiglio,ritorno);

}

}

exit(0);
}
