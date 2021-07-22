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
	int c1;
	int c2;
	} struttura;


int main(int argc,char **argv){

int pid;
int pidFiglio;
char C;
int N;
int  Y; // numeri all interno del file
int fd;
int num;
int status;
int ritorno;
struttura cur;
// struttura *cur;
struttura tmp;
pipe_t *piped; // schema di comunicazione a pipeline 
//si legge dalla pipe i + 1 e si scrive sulla pipe i


//controllo parametri

if (argc < 3){
printf("numero parametri scorretto \n");
exit(1);
}

for ( int i = 0  ; i < argc - 1 ; ++i ){

if ( (fd=open ( argv[i+1] , O_RDONLY ) ) < 0 ){
printf("errore nella apertura del file \n");
exit(2);
}

}

N = argc - 1;

Y = 0;


while ( read(fd,&C,1)){

Y++;

}

// creazione struct
/*
cur = malloc ( Y * sizeof(struttura));
if ( cur == NULL ){
printf("errore nella allocazione della memoria \n");
exit(3);
}
*/

//pipe

piped = malloc( N * sizeof(pipe_t));
if (piped == NULL){
printf("errore allocazione pipe \n");
exit(4);
}

for ( int i = 0; i < N ; ++i){
if ( pipe(piped[i]) < 0 ){
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

for ( int j = 0 ; j < N ; ++j){

//chiudo in scrittura tutte le pipe eccetto i e in lettura tutte eccetto
//i+1

if (i != j  &&  j != i+1){
close(piped[j][1]);
close(piped[j][0]);
}

close(piped[i][0]);

// chiudo cosi anche piped[0][1] per il figlio Pn-1
close(piped[(i+1) % N][1]);

}

// nel caso di errore nel figlio ritorno -1

//codice figlio


if ((fd = open(argv[i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}

int nr;

while ( read ( fd,&C,1)){

cur.c1 = getpid();
num = C - 48 ;
cur.c2 = num;  


if ( i != N - 1 ){

nr = read ( piped[i+1][0],&tmp,sizeof(tmp) );
if ( nr != sizeof(tmp) ){
printf("errore in lettura \n");
exit(-1);
}

if(tmp.c2 >= cur.c2){
cur.c2 = tmp.c2;
cur.c1 = tmp.c1;
}

}

write(piped[i][1],&cur,sizeof(cur));

}



exit(C);
}
}



// codice padre

for ( int j = 0; j < N ; ++j){

close(piped[j][1]);

if ( j != 0 ){
close(piped[j][0]);
} 

}

// int nr = 0;

while (read (piped[0][0] , &tmp ,sizeof(tmp) ) ){

/*
if ( nr != sizeof(tmp) ){
printf("errore in lettura \n");
exit(7);
}
*/

printf("numero %d  dal figlio con pid %d \n",tmp.c2,tmp.c1);


}


for( int i = 0 ; i < N; ++i){

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
printf ( "figlio ha ritornato -1 ==> problemi \n");
}
else
printf ( "figlio %d ha ritornato %d \n",pidFiglio,ritorno);

}

}

exit(0);
}
