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

int pid;
int pidFiglio;
int N; 
char Cz;
char c;
char ch; // per la comunicazione y --> si , n --> no
int occ;
long int pos;
long int *v;
int fd;
int status;
int ritorno;
pipe_t *piped; // tra padre e figlio
pipe_t *p; // tra figlio e padre


//controllo parametri


if (argc < 4 ){
printf("numero parametri scorretto pochi \n");
exit(1);
}

N = argc - 2 ;

for ( int i = 0 ; i < N ; ++i ){

if ((fd = open(argv[i+2],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(2);
}

}


if (strlen(argv[1]) != 1 ){
printf("non singolo carattere \n");
exit(3);
}
Cz=argv[1][0];

// array

v = calloc( N , sizeof(int));
if (v == NULL){
printf("errore allocazione vettore di posizioni \n");
exit(4);
}


//pipe

piped = malloc( N * sizeof(pipe_t));
if (piped == NULL){
printf("errore allocazione pipe \n");
exit(5);
}

p = malloc( N * sizeof(pipe_t));
if (p == NULL){
printf("errore allocazione pipe \n");
exit(6);
}

for ( int i = 0; i < N ; ++i){
if ( pipe(piped[i]) < 0 ){
printf("errore creazione pipe \n");
exit(7);
}

if ( pipe(p[i]) < 0 ){
printf("errore creazione pipe \n");
exit(8);
}
}

// creazione figli

for (int i = 0 ; i < N ; ++i){

if (( pid = fork()  ) < 0){
printf("errore creazione figli \n");
exit(9);
}


//codice figlio

if ( pid == 0){


// nel caso di errore nel figlio ritorno -1


//codice figlio

for ( int j = 0; j < N ; ++j){

close(piped[j][0]);
close(p[j][1]);

if (i != j){
close(piped[j][1]);
close(p[j][0]);
}

}


if ((fd = open(argv[i+2],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}


occ = 0;
pos = 0;
int j = 1; // considero 1 come posizione di partenza

while ( read(fd,&c,1) ){

if ( c == Cz){

occ = occ +1;

pos = j;

++j;

write(piped[i][1],&pos,sizeof(long int));

read(p[i][0],&ch,1);

if ( ch == 'y'){

printf("il figlio %d di pid %d ha trovato il carattere %c in posizione %ld nel file %s \n",i,getpid(),Cz,pos,argv[i+2]);

}


}

else
{
++j;
}
}

 
exit(occ);
}

}

// codice padre

for ( int j = 0; j < N ; ++j){

close(piped[j][1]);
close(p[j][0]);

}


int finito = 0;
int indice = 0;
pos = 0;
int count;

while (finito == 0){

count = 0;
pos = 0;

for ( int i = 0  ; i < N ; ++i ){

read( piped[i][0],&(v[i]),sizeof(long int));

if ( v[i] == 0 ){

++count;

}

if ( v[i] > pos ){

pos = v[i];
indice = i;

}

}

if ( count == N ){

finito = 1;
break;

}

for (int j = 0; j < N ; ++j ){

if ( j == indice ){
ch = 'y';
write(p[indice][1],&ch,1);

}
else{
ch = 'n';
write(p[j][1],&ch,1);
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
