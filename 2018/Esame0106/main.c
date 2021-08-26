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


int main(int argc,char **argv){

int *pid;
int pidFiglio;
int N; 
int K;
int X;
int fd;
int status;
int ritorno;
char linea[256];
pipe_t *piped; // tra padre e figlio

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


K = atoi(argv[argc-1]);
if (K <= 0 ){
printf("numero non positivo \n");
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

for ( int i = 0; i < N ; ++i){
if ( pipe(piped[i]) < 0 ){
printf("errore creazione pipe \n");
exit(6);
}

}

// richiesta numero all'utente
printf("chiedo un numero minore uguale a K = %d \n",K);

scanf("%d",&X);

if ( X <= 0 || X > K ){
printf("errore numero chiesto all'utente \n");
exit(7);
}

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

if (i != j){
close(piped[j][1]);
}

}


if ((fd = open(argv[i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}



int j = 0;
int len = 0;
int nlinea = 0;
int esistenza = 0;

while ( read(fd,&(linea[j]),1) ){

if ( linea[j] == '\n'){

linea[j+1] = '\0';

++nlinea;

if ( nlinea == X ){

esistenza = 1;

len = j+1;

printf("il figlio %d ha ricevuto la linea %s",getpid(),linea);

j = 0;


write(piped[i][1], &len,sizeof(len));
write(piped[i][1], linea,len);

}

j = 0;


}
else
{
++j;
}
}


if ( esistenza == 0 ){

write(piped[i][1], &len,sizeof(len));
write(piped[i][1], linea,len);


}
 
 
exit(len);
}

}

// codice padre

for ( int j = 0; j < N ; ++j){

close(piped[j][1]);

}


int valore;


for ( int i = 0  ; i < N ; ++i ){

read( piped[i][0],&valore,sizeof(int));

if ( valore ==  0 ){

read(piped[i][0],linea,0);

printf("il figlio di indice %d e pid %d non ha trovato nel file %s la linea richiesta \n",i,pid[i],argv[i+1]);


}
else{

/*
for (int k = valore ; k < 255 ; ++k ) {

linea[k] = ' ';

}
*/

read(piped[i][0],linea,valore);
linea[valore -1] = '\0';

printf("il figlio di indice %d e pid %d ha trovato nel file %s la linea %s --> lunghezza %d \n",i,pid[i],argv[i+1],linea,valore);

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
