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
int X;
int num;
char ch; 
char c;
int nr;
int fd;
int status;
int ritorno;
char *primiCarNum; 
pipe_t *piped; // tra padre e figlio


//controllo parametri


if (argc < 3 ){
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

X = atoi(argv[argc-1]);

primiCarNum = malloc(X * sizeof(char));
if ( primiCarNum == NULL){
printf("errore creazione array \n");
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

int primo = 1;
int j = 0;

while ( read(fd,&c,1) ){

if ( isdigit(c) ){

if ( primo == 1 ){

ch = c;
primo = 0;
primiCarNum[j] = ch;

}

}


if ( c == '\n'){

++j;
primo = 1;

}
}

for ( int j = 0 ; j < X ; ++j){

printf("il figlio di indice %d ha trovato il carattere numerico %c \n",i,primiCarNum[j]);

}

write(piped[i][1],primiCarNum,X);

//il figlio torna il numero di caratteri 
exit(ch);
}

}

// codice padre

for ( int j = 0; j < N ; ++j){

close(piped[j][1]);

}


num = -1;
int indice = -1;

for ( int i = 0 ; i < N ; ++i){

nr = read(piped[i][0],primiCarNum,X);

if ( nr != X){
printf("errore in lettura \n");
exit(7);
}

for ( int j = 0 ; j < X ; ++j){

printf("il figlio di indice %d nel file %s ha trovato il numero %c nella linea %d \n",i,argv[i+1],primiCarNum[j],j+1);


if ( (primiCarNum[j] -'0') > num ){

num = primiCarNum[j] -'0';
indice = i;
}


}
}

printf("il numero max trovato %d dal figlio di indice %d \n",num,indice);

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
