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
char c;
char Camilla[8] = "Camilla"; 
int num;
int q;
int fd;
int fdw;
int status;
int ritorno;
char linea[250];
pipe_t *piped; // tra padre e figlio

//controllo parametri


if (argc < 3 ){
printf("numero parametri scorretto pochi \n");
exit(1);
}

Q = argc - 1 ;

for ( int i = 0 ; i < Q ; ++i ){

if ((fd = open(argv[i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(2);
}

}

if ((fdw = creat(Camilla,PERM)) <0 ){
printf("errore creazione file \n");
exit(3);
}


//pipe

piped = malloc( Q * sizeof(pipe_t));
if (piped == NULL){
printf("errore allocazione pipe \n");
exit(4);
}


for ( int i = 0; i < Q ; ++i){
if ( pipe(piped[i]) < 0 ){
printf("errore creazione pipe \n");
exit(5);
}

}

// creazione figli

for (q = 0 ; q < Q ; ++q){

if (( pid = fork()  ) < 0){
printf("errore creazione figli \n");
exit(6);
}


//codice figlio

if ( pid == 0){


// nel caso di errore nel figlio ritorno -1


//codice figlio

for ( int j = 0; j < Q ; ++j){

close(piped[j][0]);

if (q != j){
close(piped[j][1]);
}

}


if ((fd = open(argv[q+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}


int nlinea = 0;
int j = 0;
int primo = 1; 


while ( read(fd,&(linea[j]),1) ){

if (primo == 1 ){

c = linea[j];

primo = 0;

if ( isdigit(c) ){
num = 1;
}else num = 0;

// per carattere alfabetico isalpha(c)
// per carattere non alfabetico !isalpha(c)

}

if ( linea[j] == '\n'){

primo = 1;

linea[j+1] = '\0';

printf("Il figlio di pid %d ha trovato la linea %s ",getpid(),linea);

if ( j < 10 && num == 1 ) {

write(piped[q][1],linea,j+1);
++nlinea;

}

j = 0;

}

else
{
++j;
}
}

 
exit(nlinea);
}

}

// codice padre

for ( int j = 0; j < Q ; ++j){

close(piped[j][1]);

}

int j ;

for ( q = 0  ; q < Q ; ++q ){

num = 0;
j = 0;

while ( read(piped[q][0],&(linea[j]),1) ){

if ( linea[j] == '\n' ){

++num;

linea[j+1] = '\0';

printf("il figlio di indice %d ha trovato nel file %s la linea numero %d --> %s \n",q,argv[q+1],num,linea);

write(fdw,linea,j+1);

j=0;
}else ++j;

}

}



for( q = 0 ; q < Q; ++q){

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
printf ( "figlio ha ritornato -1 problemi \n");
}
else
printf ( "figlio %d ha ritornato %d \n",pidFiglio,ritorno);

}

}

exit(0);
}
