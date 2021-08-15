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
char Cx;
long int pos;
int sostituzioni;
char ch; // utente
char c;
int fd;
int nr;
int status;
int ritorno; 
pipe_t *piped; // tra padre e figlio
pipe_t *p; // tra figlio e padre


//controllo parametri


if (argc < 3 ){
printf("numero parametri scorretto pochi \n");
exit(1);
}


N = argc - 2;

for ( int i = 0 ; i < N ; ++i ){

if ((fd = open(argv[i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(2);
}

}

if (strlen(argv[argc-1]) != 1){
printf("errore non singolo carattere \n");
exit(3);
}

Cx = argv[argc-1][0];

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

if (( pid = fork()  ) < 0){
printf("errore creazione figli \n");
exit(8);
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

if ((fd = open(argv[i+1],O_RDWR)) <0 ){
printf("errore apertura file \n");
exit(-1);
}

sostituzioni = 0;

// il primo carattere del file viene considerato in posizione 1


while ( read(fd,&c,1) ){

if (c == Cx){

pos = lseek(fd,0L,SEEK_CUR);

write(piped[i][1],&pos,sizeof(long int));

// leggo dalla pipe p per vedere se si devono fare sostitutzioni
// '\n' indica di non fare niente

read(p[i][0],&ch,1);

if ( ch != '\n'){

lseek(fd,-1L,SEEK_CUR);

write(fd,&ch,1);

sostituzioni++;

}

}
 

}

//il figlio torna il numero di sostituzioni effettuate 
exit(sostituzioni);
}

}

// codice padre

for ( int j = 0; j < N ; ++j){

close(piped[j][1]);
close(p[j][0]);

}


int finito = 0;
pos = 0;


while (finito == 0){

finito = 1;

for ( int i = 0 ; i < N ; ++i){

nr = read(piped[i][0],&pos,sizeof(long int));

if ( nr != 0 ){

finito = 0;

printf("il figlio di indice %d ha trovato nel file %s in posizione %ld il carattere %c \n",i,argv[i+1],pos,Cx);

printf("Per sostituire il carattere --> fornire il carattere o una linea vuota \n");

scanf("%c",&ch);


write(p[i][1],&ch,1);


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
