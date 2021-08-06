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

#define PERM 0644

typedef int pipe_t[2];


int main(int argc,char **argv){

int *pid;
int pidFiglio;
char c;
char Cx = 'v'; // per la sincronizzazione dei figli v == yes t == no
int N;
int fd;
int fdw;
int status;
int ritorno;
int *confronti;
pipe_t *piped; // tra padre e figlio
pipe_t *p; // per sincronizzazione


//controllo parametri

if (argc < 4){
printf("numero parametri scorretto \n");
exit(1);
}

N=argc-2;

if ( (fdw = open(argv[argc-1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(2);
}

pid = malloc( N * sizeof(int));
if (pid == NULL){
printf("errore allocazione array \n");
exit(3);
}

//pipe

piped = malloc( N * sizeof(pipe_t));
if (piped == NULL){
printf("errore allocazione pipe \n");
exit(4);
}

p = malloc( N * sizeof(pipe_t));
if (p == NULL){
printf("errore allocazione pipe \n");
exit(5);
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


confronti = malloc( N * sizeof(int));
if (confronti == NULL){
printf("errore allocazione array \n");
exit(8);
}

for ( int i = 0 ; i < N ; ++i){

confronti[i] = 1;

}



// creazione figli

for (int i = 0 ; i < N ; ++i){

if (( pid[i] = fork()  ) < 0){
printf("errore creazione figli \n");
exit(9);
}


//codice figlio

if ( pid == 0){

// nel caso di errore nel figlio ritorno -1


//codice figlio

for( int j = 0 ; j < N ; ++j){

close(piped[j][0]);
close(p[j][1]);

if ( j != i ){
close(piped[j][1]);
close(p[j][0]);
}

}


if ((fd = open(argv[i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}

while(read(p[i][0],&Cx,1)){

if ( Cx == 't'){

break;
}
else{

read(fd,&c,1);

write(piped[i][1],&c ,1);

}


}

exit(0);
}

}

// codice padre

for ( int j = 0; j < N ; ++j){

close(piped[j][1]);
close(p[j][0]);

}


char cAF;

while ( read ( fdw,&cAF,1)){

for ( int i = 0 ; i < N ; ++i ){

if ( confronti[i] == 1){

write(p[i][1],&Cx,1);

read(piped[i][0],&c,1);

if ( c != cAF){

confronti[i] = 0;
}

}

}


}


for ( int i = 0; i < N ; ++i){

if ( confronti[i] == 0 ){

if ( kill(pid[i] ,SIGKILL) < 0 ){

printf("figlio con pid %d non esiste , terminato \n",pid[i]);

}

}
else{

Cx= 't';

write(p[i][1],&Cx,1);
}

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
printf ( "figlio ha ritornato -1 problemi \n");
}
else{

for ( int j = 0 ; j < N ; ++j ){
if ( pid[i] == pidFiglio ){
printf ( "figlio %d ha ritornato %d per il file %s uguale a %s \n",pidFiglio,ritorno,argv[j+1],argv[argc-1]);
}
}

}
}

}

exit(0);
}
