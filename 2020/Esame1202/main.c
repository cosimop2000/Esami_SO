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

typedef struct {
	long int c1; // primo conteggio
	long int c2; // secondo conteggio
	
} struttura;


int main(int argc,char **argv){

int pid;
int pidFiglio;
int N; 
long int k1;
long int k2;
char c;
int fd;
int nr;
int status;
int ritorno;
struttura cur;
pipe_t *piped; // tra padre e figlio


//controllo parametri


if (argc < 3 ){
printf("numero parametri scorretto \n");
exit(1);
}

N = argc - 1;

if ((N % 2) == 1 ){
printf("errore parametri dispari \n");
exit(2);
}



for ( int i = 0 ; i < N ; ++i ){

if ((fd = open(argv[i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(3);
}

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


// nel caso di errore nel figlio ritorno -1


//codice figlio

for ( int j = 0; j < N ; ++j){

close(piped[j][0]);

if (i != j){

close(piped[j][1]);

}

}


if ((fd = open(argv[i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}

k1 = 0;
k2 = 0;

int j = 0;

cur.c1 = 0;
cur.c2 = 0;

if ( (i % 2) == 0){

while(read(fd,&c,1)){

if ( (j % 2) == 0){

if ( (c % 2) == 0){

++k1;

}
else{
++k2;
}

}

++j;
}



}
else{

while(read(fd,&c,1)){

if ( (j % 2 ) == 1) { 

if ( (c % 2) == 1){

++k1;

}
else{
++k2;
}

}

++j;
}



}


cur.c1 = k1;
cur.c2 = k2;


write(piped[i][1],&cur,sizeof(struttura));


if ( k1 > k2 ){
exit(0);
}
else{
exit(1);
}

}
}

// codice padre

for ( int j = 0; j < N ; ++j){

close(piped[j][1]);

}


for ( int i = 0 ; i < N ; i = i+2){

nr = read(piped[i][0],&cur,sizeof(struttura));

if (nr != sizeof(struttura)){
printf("errore in lettura \n");
exit(7);
}

printf("il figlio %d ha ricevuto la struttura c1 : %ld e c2 : %ld \n",i,cur.c1,cur.c2);

}

for ( int i = 1 ; i < N ; i = i+2){

nr = read(piped[i][0],&cur,sizeof(struttura));

if (nr != sizeof(struttura)){
printf("errore in lettura \n");
exit(7);
}

printf("il figlio %d ha ricevuto la struttura c1 : %ld e c2 : %ld \n",i,cur.c1,cur.c2);

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
printf ( "figlio ha ritornato -1 problemi \n");
}
else
printf ( "figlio %d ha ritornato %d \n",pidFiglio,ritorno);

}

}

exit(0);
}
