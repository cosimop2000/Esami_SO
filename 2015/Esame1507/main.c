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
char Cx = 'x'; // per la sincronizzazione dei figli
int N;
int fd;
int fdw;
int nr;
int status;
int ritorno;
pipe_t *piped; // tra padre e figlio
pipe_t *p; // per sincronizzazione


//controllo parametri

if (argc < 3){
printf("numero parametri scorretto \n");
exit(1);
}

N=argc-1;

if ( (fd = creat("Merge",PERM)) <0 ){
printf("errore creazione file \n");
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


// creazione figli

for (int i = 0 ; i < N ; ++i){

if (( pid[i] = fork()  ) < 0){
printf("errore creazione figli \n");
exit(8);
}


//codice figlio

if ( pid == 0){

// nel caso di errore nel figlio o nel nipote ritorno -1


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

do{

read(p[i][0],&Cx,1);

nr = read(fd,&c,1);

if ( nr == 1 ){

write(piped[i][1],&c,1);

}

}while(nr);

exit(c);
}

}

// codice padre

for ( int j = 0; j < N ; ++j){

close(piped[j][1]);
close(p[j][0]);

}

if ( (fdw = open ("Merge",O_WRONLY) ) < 0 ){
printf("errore apertura file \n");
exit(9);
}

int primoindice;
nr=1;

while (nr == 1){

for ( int i = 0; i < N; ++i){

write(p[i][1] ,&Cx, 1);

nr = read(piped[i][0],&c,1);

if ( nr == 1 ){

write(fdw,&c,1);

}
else{
primoindice = i;
break;
}


}

}


for ( int i = 0 ; i < N ; ++i){

if ( i != primoindice ){
if ( kill(pid[i] , SIGKILL ) < 0){
printf("il figlio è gia terminato \n");
}

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
else
printf ( "figlio %d ha ritornato %d \n",pidFiglio,ritorno);

}

}

exit(0);
}
