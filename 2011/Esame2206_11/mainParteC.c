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

typedef int pipe_t[2];

typedef struct{
	int ind;
	int occ;
	} struttura;



int main(int argc,char **argv){

int pid;
int pidFiglio;
char C;
int H;
int N;
int fd;
int status;
int ritorno;
struttura cur;
pipe_t *pipes; // tra figlio P0 fino a Pn-1
pipe_t p; // tra figli per conferma


//controllo parametri

if (argc < 4){
printf("numero parametri scorretto \n");
exit(1);
}

if ( strlen(argv[argc-2]) != 1  ){
printf("non carattere \n");
exit(2);
}

C = argv[argc-2][0];
H = atoi(argv[argc-1]);

if ( H < 0  ){
printf("non numero positivo \n");
exit(3);
}

N = argc- 3;

//pipe

pipes = malloc( N * sizeof(pipe_t));
if (pipes == NULL){
printf("errore allocazione pipe \n");
exit(4);
}

for ( int i = 0; i < N ; ++i){
if ( pipe(pipes[i]) < 0 ){
printf("errore creazione pipe \n");
exit(5);
}

}


// creazione figli

for (int i = 0 ; i < N ; ++i){

if (( pid = fork()  ) < 0){
printf("errore creazione figli \n");
exit(6);
}


//codice figlio

if ( pid == 0){

for ( int j = 0; j < N ; ++j){

//leggo dal figlio prima i-1 e scrivo su quello dopo i
if ( (i == 0) || (j != i -1)){
close(pipes[j][0]);
}
if (i != j){
close(pipes[j][1]);
}

}

// nel caso di errore nel figlio ritorno -1

//codice figlio


if ((fd = open(argv[i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}

char c;

for ( int i = 0 ; i < H ; ++i){

int count = 0;

while (read(fd,&c,1)){

if ( c == '\n'){
break;
}

count++;

}

}




}


}



// codice padre

for ( int j = 0; j < N ; ++j){

close(pipes[j][1]);
close(pipes[j][0]);
close(p[1]);
close(p[0]);
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
printf ( "figlio ha ritornato -1 ==> problemi \n");
}
else
printf ( "figlio %d ha ritornato %d \n",pidFiglio,ritorno);

}

}

exit(0);
}
