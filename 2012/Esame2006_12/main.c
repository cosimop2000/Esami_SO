#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ctype.h>

#define PERM 0644

typedef int pipe_t[2];

int main(int argc,char **argv){

int pid;
int pidFiglio;
char  *Fsotto;
char *Fsopra;
int N;
int H;
int fd;
char c;
int len;
char linea[256];
int status;
int ritorno;
pipe_t *piped_sotto; // tra figli e sotto
pipe_t *piped_sopra; // tra figli e sopra


//controllo parametri

if (argc != 4){
printf("numero parametri scorretto \n");
exit(1);
}

if( (fd = open(argv[1],O_RDONLY)) < 0){
printf("errore nella apertura del file \n");
exit(2);
}
close(fd);

N = atoi(argv[2]);

if ( N <= 0){
printf("numero minore uguale a 0 \n");
exit(3);
}

H = atoi(argv[3]);

if ( H <= 0){
printf("numero minore uguale a 0 \n");
exit(4);
}

int x = strlen(argv[1]) + 1 + strlen(".sotto");

Fsotto=malloc ( x * sizeof(char));
Fsopra= malloc ( x * sizeof(char));

if ( Fsotto == NULL || Fsopra == NULL ){
printf("errore nella allocazione \n");
exit(5);
}

strcpy(Fsotto , argv[1]);
strcpy(Fsopra , argv[1]);
strcat(Fsotto , ".sotto");
strcat(Fsotto , ".sopra");
Fsotto[x] = '\0';
Fsopra[x] = '\0';

if ( (fd = creat(Fsotto,PERM)) < 0 ){
printf("errore nella creazione del file \n");
exit(6);
}
close(fd);

if ( (fd = creat(Fsopra,PERM)) < 0 ){
printf("errore nella creazione del file \n");
exit(7);
}
close(fd);


N = N + 2 ;

//pipe

piped_sotto = malloc( (N-2) * sizeof(pipe_t));
if (piped_sotto == NULL){
printf("errore allocazione pipe \n");
exit(8);
}


piped_sopra = malloc( (N-2) * sizeof(pipe_t));
if (piped_sopra == NULL){
printf("errore allocazione pipe \n");
exit(9);
}


for ( int i = 0; i < N-2 ; ++i){

if ( pipe(piped_sotto[i]) < 0 ){
printf("errore creazione pipe \n");
exit(10);
}

if ( pipe(piped_sopra[i]) < 0 ){
printf("errore creazione pipe \n");
exit(11);
}
}


// creazione figli

for (int i = 0 ; i < N ; ++i){

if (( pid = fork()  ) < 0){
printf("errore creazione figli \n");
exit(12);
}


//codice figlio

if ( pid == 0){

for ( int j = 0; j < N - 2; ++j){
close(piped_sotto[j][0]);
close(piped_sopra[j][0]);
if (i != j){
close(piped_sotto[j][1]);
close(piped_sopra[j][1]);

}

}

if ( i == N -1 ){
//sopra
close(piped_sopra[i][1]);
close(piped_sotto[i][1]);

len = 0;
int nr = 0;
 
int fd_sopra;
if ( (fd_sopra =open ( Fsopra, O_RDWR)) < 0 ){
printf("errore apertura file \n");
exit(-1);
}

for ( int k = 0 ; k < N - 2; ++k){

nr = read( piped_sopra[k][0] , linea , sizeof(linea));

if ( nr != 0){
++len;
write(fd_sopra,linea, sizeof(linea));

}

}


exit(len);
}

if ( i == N -2 ){
//sotto
close(piped_sotto[i][1]);
close(piped_sopra[i][1]);

len = 0;
int nr = 0; 
int fd_sotto;
if ( (fd_sotto =open ( Fsotto, O_RDWR)) < 0 ){
printf("errore apertura file \n");
exit(-1);
} 

for ( int k = 0 ; k < N - 2; ++k){

nr = read( piped_sotto[k][0] , linea ,  sizeof(linea));

if ( nr != 0){
++len;
write(fd_sotto,linea, sizeof(linea));

}

}

exit(len);
}


// nel caso di errore nel figlio ritorno -1
//codice figlio

if ( (fd=open(argv[1],O_RDONLY)) < 0 ){
printf("errore apertura file \n");
exit(-1);
}

len = 0;


while ( read(fd,&c,1) ){

if ( c == '\n' ){
linea[len] = '\0';
++len;

if ( len >= H ){
write(piped_sotto[i][1],linea,len);
}
else{
write(piped_sopra[i][1],linea,len);
}

break;
}


linea[len] = c ;
++len;

}


exit(len);
}

}

// codice padre

for ( int j = 0; j < N -2 ; ++j){

close(piped_sotto[j][1]);
close(piped_sotto[j][0]);
close(piped_sopra[j][1]);
close(piped_sopra[j][0]);

}




for( int i = 0 ; i < N; ++i){

pidFiglio = wait(&status);
if ( pidFiglio < 0){
printf("errore in wait \n");
exit(13);
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
