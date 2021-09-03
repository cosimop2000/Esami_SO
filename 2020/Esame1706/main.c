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
int B;
int L;
char c;
char *file; 
int dim;
int q;
int fd;
int fdw;
int status;
int ritorno;
char *buff;
pipe_t *piped; // tra padre e figlio

//controllo parametri


if (argc != 4 ){
printf("numero parametri scorretto \n");
exit(1);
}

if ((fd = open(argv[1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(2);
}

L = atoi(argv[2]);
if ( L <= 0 ){
printf("numero non positivo \n");
exit(3);
}

B = atoi(argv[3]);
if ( B <= 0 ){
printf("numero non positivo \n");
exit(4);
}

file=malloc((strlen(argv[1]) + 8) * sizeof(char));
if (file == NULL){
printf("errore allocazione nome file \n");
exit(5);
}

strcpy(file,argv[1]);
strcat(file,".Chiara");

if ((fdw = open(file,O_WRONLY)) <0 ){
printf("errore apertura file \n");
exit(6);
}


//pipe

piped = malloc( B * sizeof(pipe_t));
if (piped == NULL){
printf("errore allocazione pipe \n");
exit(7);
}


for ( q = 0; q < B ; ++q){
if ( pipe(piped[q]) < 0 ){
printf("errore creazione pipe \n");
exit(8);
}

}

// creazione figli

for (q = 0 ; q < B ; ++q){

if (( pid = fork()  ) < 0){
printf("errore creazione figli \n");
exit(9);
}


//codice figlio

if ( pid == 0){


// nel caso di errore nel figlio ritorno -1


//codice figlio

for ( int j = 0; j < B ; ++j){

close(piped[j][0]);

if (q != j){
close(piped[j][1]);
}

}


if ((fd = open(argv[1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}

long int pos;

pos = q*L/B ;

buff=malloc((L/B) * sizeof(char));
if (buff == NULL){
printf("errore allocazione pipe \n");
exit(-1);
}

dim = L/B;

lseek(fd,pos,SEEK_SET);

read(fd,buff,dim);

write(piped[q][1],&(buff[dim - 1]), 1); 
 
exit(dim);
}

}

// codice padre

for ( int j = 0; j < B ; ++j){

close(piped[j][1]);

}


for ( q = 0  ; q < B ; ++q ){

read(piped[q][0],&c ,1);

write(fdw,&c,1);

}



for( q = 0 ; q < B; ++q){

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
printf ( "figlio %d di indice %d ha ritornato %d (dimensione del blocco) \n",pidFiglio,q,ritorno);

}

}

exit(0);
}
