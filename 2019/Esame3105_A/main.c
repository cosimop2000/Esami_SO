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

typedef struct {
	int c1; // pid nipote
	int c2; // lunghezza linea
	char c3[250]; // linea

} struttura ;


int main(int argc,char **argv){

int pid;
int pidFiglio;
int pidN;
int pidNipote;
int N; 
int len;
int fd;
int fdN;
int nr;
int status;
int ritorno;
char linea[250];
struttura cur;
pipe_t *piped; // tra padre e figlio
pipe_t p; // tra figlio e nipote


//controllo parametri


if (argc < 4 ){
printf("numero parametri scorretto pochi \n");
exit(1);
}

N = argc - 1  ;

for ( int i = 0 ; i < N ; ++i ){

if ((fd = open(argv[i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(2);
}

}


//pipe

piped = malloc( N * sizeof(pipe_t));
if (piped == NULL){
printf("errore allocazione pipe \n");
exit(3);
}

for ( int i = 0; i < N ; ++i){
if ( pipe(piped[i]) < 0 ){
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

close(piped[i][1]);
close(piped[i][0]);
close(p[0]);


close(0);


if ((fdN = open(argv[i + 1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}


close(1);
dup(p[1]);

close(p[1]);

close(2);
open("/dev/null",O_WRONLY);

// per ignorare maiuscole e minuscole
execlp("sort","sort","-f",(char *)0);

// per ordine inverso
// execlp("sort","sort","-r",(char *)0);

// esco con -1 solo in caso di errore
exit(-1);
}

//codice figlio

close(p[1]);

for ( int j = 0; j < N ; ++j){

close(piped[j][0]);
if (i != j){
close(piped[j][1]);

}

}


int j = 0;
len = 0;

while ( read(p[0],&(linea[j]),1) ){

if ( linea[j] == '\n'){

linea[j+1] = '\0';

len = j+1;

printf("il figlio %d ha ricevuto la linea %s",getpid(),linea);

j = 0;

cur.c1 = pidN;
cur.c2 = len;

/*
for ( int k = len ; k < strlen(cur.c3) ; ++k){

cur.c3[k] = ' ';

}
*/
strncpy(cur.c3 , linea, len);

write(piped[i][1], &cur,sizeof(struttura) );

break;
}
else
{
++j;
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

//il figlio torna la lunghezza della linea
exit(len-1);
}

}

// codice padre

for ( int j = 0; j < N ; ++j){

close(piped[j][1]);

}


for ( int i = 0  ; i < N ; ++i ){

nr = read( piped[i][0],&cur,sizeof(struttura));

if ( nr != sizeof(struttura) ){

printf("errore in lettura \n");
exit(6);

}

strncpy(linea,cur.c3, cur.c2);

linea[cur.c2] = '\0';


printf("figlio di indice %d --> struttura ricevuta pid nipote: %d lunghezza linea :%d linea: %s nel file %s \n",i,cur.c1,cur.c2,linea,argv[i+1]);


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
