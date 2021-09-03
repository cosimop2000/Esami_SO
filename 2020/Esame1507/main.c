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

typedef struct{

	int c1; // pid
	int c2; // secondo carattere
	int c3; // penultimo carattere

} struttura;

int main(int argc,char **argv){

int pid;
int pidFiglio;
int L;
char secondo;
char penultimo;
int nlinea;
int q;
int fd;
int nr;
int status;
int ritorno;
char linea[250];
struttura cur;
pipe_t *piped; // tra padre e figlio

//controllo parametri


if (argc != 3 ){
printf("numero parametri scorretto \n");
exit(1);
}

if ((fd = open(argv[1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(2);
}

L = atoi(argv[2]);
if ( L <= 0 || L > 255 ){
printf("numero non positivo o maggiore di 255 \n");
exit(3);
}


//pipe

piped = malloc( L * sizeof(pipe_t));
if (piped == NULL){
printf("errore allocazione pipe \n");
exit(4);
}


for ( q = 0; q < L ; ++q){
if ( pipe(piped[q]) < 0 ){
printf("errore creazione pipe \n");
exit(5);
}

}

// creazione figli

for (q = 0 ; q < L ; ++q){

if (( pid = fork()  ) < 0){
printf("errore creazione figli \n");
exit(6);
}


//codice figlio

if ( pid == 0){


// nel caso di errore nel figlio ritorno -1


//codice figlio

for ( int j = 0; j < L ; ++j){

close(piped[j][0]);

if (q != j){
close(piped[j][1]);
}

}


if ((fd = open(argv[1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}

nlinea = 0;
int j = 0;
cur.c1 = getpid();
cur.c2 = 0;
cur.c3 = 0;

while (read(fd,&(linea[j]),1)){


if (linea[j] == '\n'){

++nlinea;


if ( nlinea == q+1){

linea[j+1] = '\0';
secondo = linea[1];
penultimo = linea[j-1];

printf("il figlio di pid %d ha trovato la linea %s \n",cur.c1,linea);

break;
}


j = 0;

}
else{
++j;
}

}


cur.c2 = secondo;
cur.c3 = penultimo;

write(piped[q][1],&cur,sizeof(struttura));
 
exit(nlinea);
}

}

// codice padre

for ( int j = 0; j < L ; ++j){

close(piped[j][1]);

}


for ( q = 0  ; q < L ; ++q ){

nr = read(piped[q][0],&cur,sizeof(struttura));

if (nr != sizeof(struttura)){
printf("errore in lettura \n");
exit(7);
}

if (cur.c2 == cur.c3){

printf("il figlio di indice %d di pid %d ha travato nella linea %d i caratteri %c e %c uguali (secondo e penultimo) nel file %s \n",q,cur.c1,q+1,cur.c2,cur.c3,argv[1]);

}else printf("il padre non stampa nulla i caratteri sono diversi \n");
 

}



for( q = 0 ; q < L; ++q){

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
