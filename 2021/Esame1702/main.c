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
int Q;
int k;
int q;
char c;
int fd;
int nr;
int status;
int ritorno;
char linea[250];
pipe_t *piped; // comunicazione a ring

//controllo parametri


if (argc < 3 ){
printf("numero parametri scorretto \n");
exit(1);
}

Q = argc - 1;

for ( q = 0 ; q < Q ; ++q){

if ((fd = open(argv[q+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(2);
}

}

//pipe

piped = malloc( Q * sizeof(pipe_t));
if (piped == NULL){
printf("errore allocazione pipe \n");
exit(3);
}


for ( q = 0; q < Q ; ++q){
if ( pipe(piped[q]) < 0 ){
printf("errore creazione pipe \n");
exit(4);
}

}

// creazione figli

for (q = 0 ; q < Q ; ++q){

if (( pid = fork()  ) < 0){
printf("errore creazione figli \n");
exit(5);
}


//codice figlio

if ( pid == 0){


// nel caso di errore nel figlio ritorno -1


//codice figlio

for ( int j = 0; j < Q ; ++j){

//chiudo in scrittura tutte le pipe eccetto ((q+1) % Q) e in lettura tutte eccetto
//q

if ( j != q ){
close(piped[j][0]);
}


if ( j != ((q+1) % Q)  ){
close(piped[j][1]);
}


}


if ((fd = open(argv[q+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}


int j = 0;
k = 0;

while (read(fd,&(linea[j]),1)){

if ( isdigit(linea[j]) ){

++k;

}

if ( linea[j] == '\n' ){

nr = read(piped[q][0],&c,sizeof(char));

if ( nr != sizeof(char) ){
printf("errore lettura \n");
exit(-1);
}

linea[j] = '\0';

printf("il figlio con indice %d e pid %d ha letto %d caratteri nella linea %s \n",q,getpid(),k,linea);

ritorno = k;
k = 0;

write(piped[(1+q) % Q][1],&c,sizeof(char));

// ultimo figlio manda ok al primo figlio che non viene ricevuto ,il padre
// deve tenere aperto il lato di lettura

j = 0;

}

else{
++j;
}

}

exit(ritorno);
}

}


// codice padre

for ( int j = 1; j < Q ; ++j){

close(piped[j][1]);

close(piped[j][0]);


}


write(piped[0][1],&c,sizeof(char));

close(piped[0][1]);


for( q = 0 ; q < Q; ++q){

pidFiglio = wait(&status);
if ( pidFiglio < 0){
printf("errore in wait \n");
exit(6);
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
