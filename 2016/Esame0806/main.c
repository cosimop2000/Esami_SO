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



int mia_random(int n){

int casuale;
casuale = rand() % n;


return casuale;
}



int main(int argc,char **argv){

int pid;
int pidFiglio;
int N;
int H; 
int num;
int len;
int giusto;
int fdw;
int fd;
int status;
int ritorno;
char linea[256];
pipe_t *piped; // tra padre e figlio
pipe_t *p; // tra figlio e padre


//controllo parametri


if (argc < 6 ){
printf("numero parametri scorretto pochi \n");
exit(1);
}


N = argc - 2 ;
H = atoi(argv[argc-1]);

if (H <= 0 || H >= 255){
printf("errore numero \n");
exit(2);
}

for ( int i = 0 ; i < N ; ++i ){

if ((fd = open(argv[i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(2);
}

}

srand(time(NULL));

//pipe

piped = malloc( N * sizeof(pipe_t));
if (piped == NULL){
printf("errore allocazione pipe \n");
exit(3);
}


p = malloc( N * sizeof(pipe_t));
if (p == NULL){
printf("errore allocazione pipe \n");
exit(4);
}

for ( int i = 0; i < N ; ++i){

if ( pipe(piped[i]) < 0 ){
printf("errore creazione pipe \n");
exit(5);
}

if ( pipe(p[i]) < 0 ){
printf("errore creazione pipe \n");
exit(6);
}
}

if ((fdw = creat("/tmp/creato",PERM)) <0 ){
printf("errore creazione file \n");
exit(7);
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

if ((fd = open(argv[i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}


int j = 0;
int count= 0;

while ( read(fd,&(linea[j]),1) ){


if ( linea[j] == '\n'){


linea[j+1] = '\0';

printf("il figlio %d ha ricevuto la linea %s \n",getpid(),linea);


len = j + 1;

write(piped[i][1],&len,sizeof(len));

read(p[i][0],&num,sizeof(num));

if (num < len) {

write(fdw,&(linea[num]),1);
++count;

}

j = 0;

}
else{
++j;
}

}





//il figlio torna il numero di caratteri scritti su Fcreato
exit(count);
}

}

// codice padre

for ( int j = 0; j < N ; ++j){

close(piped[j][1]);
close(p[j][0]);

}


for ( int j = 1 ; j <= H ; ++j ){

num = mia_random(N);

for ( int i = 0  ; i < N ; ++i){

read(piped[i][0],&len,sizeof(len));

if ( i == num ){

giusto = len;

}
}

num= mia_random(giusto);

for(int k = 0 ; k < N ; ++k){

write(p[k][1],&num,sizeof(num));

}


}





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
printf ( "figlio ha ritornato -1 problemi \n");
}
else
printf ( "figlio %d ha ritornato %d \n",pidFiglio,ritorno);

}

}

exit(0);
}
