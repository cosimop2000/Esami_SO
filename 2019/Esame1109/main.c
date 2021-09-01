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

#define PERM 0644

typedef int pipe_t[2];


int main(int argc,char **argv){

int pid;
int pidFiglio;
int N;
char c;
char ch; 
int occ;
char *nome;
int fd;
int fdw;
int status;
int ritorno;
pipe_t *piped; // tra figlio pari e figlio dispari

//controllo parametri


if (argc < 3 ){
printf("numero parametri scorretto pochi \n");
exit(1);
}

N = argc - 1 ;

for ( int i = 0 ; i < N ; ++i ){

if ((fd = open(argv[i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(2);
}

}

if ((N % 2) == 1){
printf("numero parametri dispari\n");
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

// creazione figli

for (int i = 0 ; i < N ; ++i){

if (( pid = fork()  ) < 0){
printf("errore creazione figli \n");
exit(8);
}


//codice figlio

if ( pid == 0){

// nel caso di errore nel figlio ritorno -1

//codice figlio

if ((fd = open(argv[i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}


if ( (i % 2) == 0 ){

for ( int j = 0; j < N ; ++j){

close(piped[j][0]);

if (i + 1 != j ){
close(piped[j][1]);
}

}


occ = 0;

while ( read(fd,&c,1) ){

++occ;

write(piped[i+1][1],&c,1);

}




 
exit(occ);

}
else{

for ( int j = 0; j < N ; ++j){

close(piped[j][1]);

if (j != i ){
close(piped[j][0]);
}

}

nome = malloc ((strlen(argv[i+1]) + 10 )*sizeof(char));
if (nome == NULL){
printf("errore allocazione nome file \n");
exit(-1);
}

strcpy(nome,argv[i+1]);
strcat(nome,".MAGGIORE");

if ((fdw = creat(nome,PERM)) <0 ){
printf("errore apertura file \n");
exit(-1);
}


occ = 0;

while ( read(fd,&c,1) ){

++occ;

read(piped[i][0],&ch,1);

if (c <= ch){

c = ch;

}

write(fdw,&c,1);

}

exit(occ);
}
}
}
 
// codice padre

for ( int j = 0; j < N ; ++j){

close(piped[j][1]);
close(piped[j][0]);

}

printf("Il padre riceve il numero di caratteri letti dai singoli processi delle coppie di figli \n");

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
