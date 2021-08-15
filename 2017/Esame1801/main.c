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

typedef int pipe_t[2];

typedef struct {
	int c1; // indice processo
	int c2; // numero corrispondente a Cn

} struttura;


int main(int argc,char **argv){

int *pid;
int pidFiglio;
int N;
int fd;
long int sum;
char c;
char Cn;
int status;
int ritorno;
int nr;
struttura *cur;
struttura *tmp;
pipe_t *piped; // schema di comunicazione a pipeline 
//si legge dalla pipe i-1 e si scrive sulla pipe i
// da figlio 1 a figlio n-1 poi a padre


//controllo parametri


if (argc < 2){
printf("numero parametri scorretto \n");
exit(1);
}

N = argc - 1;

for (int i = 0 ; i < N ; ++i){
if ( (fd=open ( argv[i+1] , O_RDONLY ) ) < 0 ){
printf("errore nella apertura del file \n");
exit(2);
}
}

// array

pid = malloc( N * sizeof(int));
if (pid == NULL){
printf("errore allocazione vettore di pid \n");
exit(3);
}
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


cur = malloc( N * sizeof(struttura));
if (cur == NULL){
printf("errore allocazione struct \n");
exit(6);
}


// creazione figli

for (int i = 0 ; i < N ; ++i){

if (( pid[i] = fork()  ) < 0){
printf("errore creazione figli \n");
exit(7);
}


//codice figlio

if ( pid[i] == 0){

for ( int j = 0 ; j < N ; ++j){

//chiudo in scrittura tutte le pipe eccetto i e in lettura tutte eccetto
//i-1

if ( j != i  ){
close(piped[j][1]);
}

if ( (i == 0) || (j != i - 1)){
close(piped[j][0]);
}

}


// nel caso di errore nel figlio ritorno -1

//codice figlio


if ((fd = open(argv[i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}

nr = 0;
int x = 0;

while ( read ( fd,&c,1) ){

if ( isdigit(c) ){
Cn = c;
break;
}

}

x = Cn - 48;
// x = Cn - '0';

tmp = malloc( (i+1) * sizeof(struttura));
if (tmp == NULL){
printf("errore allocazione struct \n");
exit(-1);
}


if ( i != 0 ){

nr = read(piped[i-1][0],tmp,i * sizeof(struttura));
if ( nr != i * sizeof(struttura) ){
printf("errore lettura \n");
exit(-1);
}


}

tmp[i].c1 = i;
tmp[i].c2 = x;

printf("il figlio di pid %d di indice %d ha trovato il carattere numerico %d \n",getpid(),tmp[i].c1,tmp[i].c2);

write(piped[i][1],tmp,(i+1) * sizeof(struttura));



exit(Cn);
}
}



// codice padre

for ( int j = 0; j < N ; ++j){

close(piped[j][1]);

if ( j != N - 1  ){
close(piped[j][0]);
} 

}


nr = read(piped[N - 1][0],cur, N * sizeof(struttura) );

if ( nr != N * sizeof(struttura) ){
printf("errore in lettura \n");
exit(8);
}

sum = 0;

for ( int j = 0 ; j < N ; ++j){

printf("il figlio di indice %d di pid %d ha trovato il carattere numerico %d \n",cur[j].c1,pid[j],cur[j].c2);

sum = sum + cur[j].c2;

}

printf("somma primi caratteri numerici trovati in tutti i file %ld \n",sum);


for( int i = 0 ; i < N; ++i){

pidFiglio = wait(&status);
if ( pidFiglio < 0){
printf("errore in wait \n");
exit(9);
}
if ( (status & 0xFF) != 0){
printf ("figlio %d terminato in modo anomalo \n",pidFiglio);
}
else{
status = status >>8;
ritorno = status & 0xFF;
if (ritorno == 255){
printf ( "figlio ha ritornato -1 --> problemi \n");
}
else
printf ( "figlio %d ha ritornato %d %c \n",pidFiglio,ritorno,ritorno);

}

}

exit(0);
}
