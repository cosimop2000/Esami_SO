
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ctype.h>

typedef int pipe_t[2];


int main(int argc,char **argv){

int pid;
int pidFiglio;
int pidN;
int pidNipote;
char Cx;
char  c;
int N;
int fd;
int fdN;
int status;
int ritorno;
pipe_t *piped; // tra padre e figlio
pipe_t *p; // tra padre e nipote
long int occ;

//controllo parametri

if (argc < 3){
printf("numero parametri scorretto \n");
exit(1);
}


N = argc- 1;

//pipe

piped = malloc( N * sizeof(pipe_t));
if (piped == NULL){
printf("errore allocazione pipe \n");
exit(2);
}

p = malloc( N * sizeof(pipe_t));
if (p == NULL){
printf("errore allocazione pipe \n");
exit(2);
}

for ( int i = 0; i < N ; ++i){
if ( pipe(piped[i]) < 0 ){
printf("errore creazione pipe \n");
exit(3);
}
}

for ( int i = 0; i < N ; ++i){
if ( pipe(p[i]) < 0 ){
printf("errore creazione pipe \n");
exit(3);
}
}


// creazione figli

for (int i = 0 ; i < N ; ++i){

if (( pid = fork()  ) < 0){
printf("errore creazione figli \n");
exit(4);
}


//codice figlio

if ( pid == 0){

for ( int j = 0; j < N ; ++j){
close(piped[j][0]);
if (i != j){
close(piped[j][1]);
}

}

// nel caso di errore nel figlio o nel nipote ritorno -1



if ( (pidN = fork()) < 0){
printf("errore creazione nipote \n");
exit(-1);
}

// codice nipote
if ( pidN == 0){

//chiudo i lati delle pipe non utilizzate

for ( int j = 0; j < N ; ++j){
close(p[j][0]);
if (i != j){
close(p[j][1]);
}

}

if ((fdN = open(argv[i+1],O_RDWR)) <0 ){
printf("errore apertura file \n");
exit(-1);
}

occ = 0;

while ( read(fdN,&Cx,1)){

if ( isdigit(Cx) ){

lseek (fdN,-1L,SEEK_CUR);

c=' ';
write(fdN,&c,1);

occ = occ +1;

}

}

/*printf("Nel file sono stati trasformati %ld numeri in spazi \n",occ);
*/
write(p[i][1],&occ,sizeof(long int));

occ = occ/256;

exit(occ);

}

//codice figlio


for (int j = 0; j < N ; ++j){

close(p[j][0]);
close(p[j][1]);
}

if ((fd = open(argv[i+1],O_RDWR)) <0 ){
printf("errore apertura file \n");
exit(-1);
}

occ = 0;

while ( read(fd,&Cx,1)){

if ( isupper(Cx) ){

lseek (fd,-1L,SEEK_CUR);

c=tolower(Cx);
write(fd,&c,1);

occ = occ +1;

}

}
/*
printf("Nel file sono stati trasformati %ld maiuscole in minuscole \n",occ);
*/
write(piped[i][1],&occ,sizeof(long int));

occ = occ/256;


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

close(piped[j][1]);
close(p[j][1]);

}


for ( int i = 0; i < N; ++i){

read(p[i][0],&occ,sizeof(long int));
printf("il nipote ha operato %ld trasformazioni (numeri -> spazi) nel file %s   \n" ,occ , argv[i+1]);


read(piped[i][0],&occ,sizeof(long int));
printf("il figlio ha operato %ld trasformazioni (maiuscole -> minuscole) nel file %s   \n" ,occ , argv[i+1]);

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
printf ( "figlio o nipote hanno ritornato -1 ==> problemi \n");
}
else
printf ( "figlio %d ha ritornato %d \n",pidFiglio,ritorno);

}

}

exit(0);
}
