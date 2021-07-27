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
#include <stdbool.h>


#define PERM 0644

typedef int pipe_t[2];

typedef struct{
	int len;
	int figlio;

} struttura;


void bubbleSort ( struttura v[] , int dim ){
int i;
struttura tmp;
bool ordinato = false;

while ( dim > 1 && ordinato == false ){

ordinato = true;

for ( i = 0 ; i < dim -1 ; ++i ){

if ( v[i].len > v[i+1].len )

tmp = v[i];
v[i] = v[i+1];
v[i+1] = tmp;

ordinato = false;
}


dim--;

}


}

int main(int argc,char **argv){

int pid;
int pidFiglio;
struttura *cur; // struct 
// char c;
int N;
int K;
int X; // minore uguale a K
int fd;
int status;
int ritorno;
char linea[256]; 
pipe_t *piped; // tra padre e figlio
pipe_t *pipes; // per le linee ordinate

//controllo parametri

if (argc < 3){
printf("numero parametri scorretto pochi \n");
exit(1);
}

N = argc - 2  ;

for ( int i = 0 ; i < N ; ++i ){

if ((fd = open(argv[i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(2);
}

}

K = atoi (argv[argc - 1]);

if ( K <= 0 ){
printf("K deve essere maggiore di 0 \n");
exit(3);
}

printf("chiedo numero all'utente minore di %d \n",K);
scanf(" %d \n",&X);

if ( X <= 0 || X > K ){
printf("X deve essere maggiore di 0 e minore uguale a K \n");
exit(4);
}

cur = malloc( N * sizeof(struttura));
if (cur == NULL){
printf("errore allocazione struct \n");
exit(5);
}


//pipe

piped = malloc( N * sizeof(pipe_t));
if (piped == NULL){
printf("errore allocazione pipe \n");
exit(6);
}

pipes = malloc( N * sizeof(pipe_t));
if (pipes == NULL){
printf("errore allocazione pipe \n");
exit(7);
}

for ( int i = 0; i < N ; ++i){

if ( pipe(piped[i]) < 0 ){
printf("errore creazione pipe \n");
exit(8);
}

if ( pipe(pipes[i]) < 0 ){
printf("errore creazione pipe \n");
exit(9);
}
}


// creazione figli

for (int i = 0 ; i < N ; ++i){

if (( pid = fork()  ) < 0){
printf("errore creazione figli \n");
exit(10);
}


//codice figlio

if ( pid == 0){


// nel caso di errore nel figlio ritorno -1


//codice figlio


for ( int j = 0; j < N ; ++j){

close(piped[j][0]);
close(pipes[j][1]);
if (i != j){
close(piped[j][1]);
close(pipes[j][0]);
}

}


if ((fd = open(argv[i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}

int count = 1; // variabile che indica la linea corrente
int j = 0;
int tmp;

/*
while ( read (fd, &c ,1) ){

if ( count == X ){

linea[j] = c;

if ( c == '\n'){
count++;

cur[i].figlio = i;
cur[i].len = j + 1;
linea[j+1] = '\0';
tmp = j;

write(piped[i][1],&j,sizeof(int));

}

}

if ( (c == '\n') && (count != X)){

count++;

j = 0;
}


++j;
}
*/

while ( read(fd,&(linea[j]),1) ){

if ( linea[j] == '\n'){

if ( count == X ){
// esco dal ciclo di lettura
break;
}
else{
j = 0;
count++;
}

}
else{  j++; }

}
j++;
tmp = j;

write(piped[i][1],&tmp,sizeof(tmp));

read(pipes[i][0],&j,sizeof(j));

write(1,linea,tmp);
write(1,"\n",1);

ritorno = (tmp) % 255 ; 

//il figlio torna il resto tra len e 255
exit(ritorno);
}

}

// codice padre

for ( int j = 0; j < N ; ++j){

close(piped[j][1]);
close(pipes[j][0]);

}



for ( int i = 0 ; i < N ; ++i ){

read(piped[i][0],&(cur[i].len),sizeof(int));


cur[i].figlio = i;
}

bubbleSort(cur , N);

for ( int i = 0 ; i < N ; ++i ){

printf("nome del file %s che contiene la linea di lunghezza %d \n",argv[(cur[i].figlio + 1)],cur[i].len);

write(pipes[(cur[i].figlio)][1],&(cur[i].len) ,sizeof(int));

sleep(2);

}



for( int i = 0 ; i < N; ++i){

pidFiglio = wait(&status);
if ( pidFiglio < 0){
printf("errore in wait \n");
exit(11);
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
