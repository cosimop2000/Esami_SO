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

/*testo B */

typedef int pipe_t[2];
typedef struct{
	char c;
	long int pos;
	} struttura;



int main(int argc,char **argv){

int pid;
int pidFiglio;
int pidN;
int pidNipote;
char C1;
char C2;
char Cx;
int N;
int fd;
int fdN;
int status;
int ritorno;
struttura curF;
struttura curN;
pipe_t *pipes; // tra padre e nipote
pipe_t p; // tra figlio e nipote
int occ;

//controllo parametri

if (argc < 5){
printf("numero parametri scorretto \n");
exit(1);
}

if ( (strlen(argv[argc-2]) != 1 ) || (strlen(argv[argc-1]) != 1 )){
printf("non caratteri \n");
exit(2);
}

C1 = argv[argc-2][0];
C2 = argv[argc-1][0];
N = argc- 3;

//pipe

pipes = malloc( N * sizeof(pipe_t));
if (pipes == NULL){
printf("errore allocazione pipe \n");
exit(3);
}

for ( int i = 0; i < N ; ++i){
if ( pipe(pipes[i]) < 0 ){
printf("errore creazione pipe \n");
exit(4);
}
}


// creazione figli

for (int i = 0 ; i < N ; ++i){

if (( pid = fork()  ) < 0){
printf("errore creazione figli \n");
exit(5);
}


//codice figlio

if ( pid == 0){

for ( int j = 0; j < N ; ++j){
close(pipes[j][0]);
if (i != j){
close(pipes[j][1]);
}

}

// nel caso di errore nel figlio o nel nipote ritorno -1

if ( pipe(p) < 0 ){
printf("errore creazione pipe \n");
exit(-1);
}

if ( (pidN = fork()) < 0){
printf("errore creazione nipote \n");
exit(-1);
}

// codice nipote
if ( pidN == 0){

//chiudo i lati delle pipe non utilizzate

close(pipes[i][1]);

close(p[0]);

if ((fdN = open(argv[i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}

occ = 0;
curN.c = C2;

while ( read(fdN,&Cx,1)){

if ( Cx == C2){

curN.pos = lseek (fdN,0L,SEEK_CUR) - 1;

write(p[1],&curN,sizeof(struttura));

occ = occ +1;

}

}


exit(occ);
}

//codice figlio


close(p[1]);

if ((fd = open(argv[i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}

occ = 0;

while ( read( fd, &Cx, 1)){

curF.c = C1;

if ( Cx == C1){


curF.pos = lseek(fd,0L,SEEK_CUR) -1;

int nr = read(p[0],&curN,sizeof(struttura));
if ( nr != 0){

if ( curF.pos >= curN.pos) {
curF.pos = curN.pos;
curF.c = curN.c;

}

write(pipes[i][1],&curF,sizeof(struttura));

++occ;

}
else break;

}

}


//aspetto il nipote

pidNipote = wait(&status);
if ( pidNipote < 0){
printf("errore in wait \n");
exit(-1);
}
if ( (status & 0xFF) != 0){
printf ("nipote %d terminato in modo anomalo \n",pidNipote);
}
else{
status = status >> 8;
ritorno = status & 0xFF;
printf ( "nipote %d ha ritornato %d \n",pidNipote,ritorno);

}


exit(occ);
}

}

// codice padre

for ( int j = 0; j < N ; ++j){

close(pipes[j][1]);

}

for ( int i = 0; i < N; ++i){

while ( read(pipes[i][0] , &curF , sizeof(struttura))){

if ( curF.c == C1){
printf("il figlio di indice %d ha trovato il carattere %c nella posizione %ld nel file %s \n",i,curF.c,curF.pos,argv[i+1]);
}
else printf("il nipote di indice %d ha trovato il carattere %c nella posizione %ld nel file %s \n",i,curF.c,curF.pos,argv[i+1]);
}
}

for( int i = 0 ; i < N; ++i){

pidFiglio = wait(&status);
if ( pidFiglio < 0){
printf("errore in wait \n");
exit(-1);
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
