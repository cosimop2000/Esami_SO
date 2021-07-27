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

int pid;
int pidFiglio;
int len;
int sum;
char c;
int N;
int X;
int fd;
int *fdtmp;
int status;
int ritorno;
char linea[256]; 
char nome[10]; // per il nome del file all'interno di /tmp/X
// esempio /tmp/255
pipe_t *piped; // tra padre e figlio

//controllo parametri

if (argc < 3){
printf("numero parametri scorretto pochi \n");
exit(1);
}

N = argc - 2  ;

for ( int i = 0 ; i < N ; ++i ){

if ((fd = open(argv[i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(2);
}

}

X = atoi (argv[argc - 1]);

fdtmp = malloc ( X * sizeof(int));
if (fdtmp == NULL){
printf("errore allocazione array di fd \n");
exit(3);
}

for ( int j = 1 ; j <= X ; ++j ){

sprintf(nome,"/tmp/%d",j);

if ( (fdtmp[j-1] = creat(nome,PERM)) < 0 ){
printf("errore creazione file \n");
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

sum = 0;
int j = 0;

while ( read (fd, &c ,1) ){

if ( c == '\n'){


len = j + 1;
linea[j] = '\0';
j = 0;

printf("figlio di indice %d associato al file %s linea di valore %d e la linea %s \n",i,argv[i+1],len,linea);

write(piped[i][1],&len,sizeof(int));

write(piped[i][1],linea,len);
sum = sum + len;
}

linea[j] = c;

++j;
}


ritorno = sum / X ; 

//il figlio torna la media delle linee
exit(ritorno);
}

}

// codice padre

for ( int j = 0; j < N ; ++j){

close(piped[j][1]);

}


for ( int j = 1 ; j <= X ; ++ j){

for ( int i = 0 ; i < N ; ++i ){

read( piped[i][0], &len , sizeof(int));
read( piped[i][0], &linea , len);

/*
printf("padre --> figlio di indice %d associato al file %s linea di valore %d e la linea %s \n",i,argv[i+1],len,linea);
*/

write(fdtmp[j -1],linea,len);

}

}

for( int i = 0 ; i < N; ++i){

pidFiglio = wait(&status);
if ( pidFiglio < 0){
printf("errore in wait \n");
exit(7);
}
if ( (status & 0xFF) != 0){
printf ("figlio %d terminato in modo anomalo \n",pidFiglio);
}
else{
status = status >>8;
ritorno = status & 0xFF;
if (ritorno == 255){
printf ( "figlio o nipote hanno ritornato -1 problemi \n");
}
else
printf ( "figlio %d ha ritornato %d \n",pidFiglio,ritorno);

}

}

exit(0);
}
