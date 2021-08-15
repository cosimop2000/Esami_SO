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

#define PERM 0644

typedef int pipe_t[2];


int main(int argc,char **argv){

int *pid;
int pidFiglio;
int len;
int N;
int fd;
int fdw;
int nr;
char linea[250];
int status;
int ritorno;
pipe_t *piped; // tra padre e figlio


//controllo parametri

if (argc < 3){
printf("numero parametri scorretto \n");
exit(1);
}

N=argc-1;

if ( (fdw = creat("Ultime_Linee",PERM)) <0 ){
printf("errore creazione file \n");
exit(2);
}
close(fdw);

pid = malloc( N * sizeof(int));
if (pid == NULL){
printf("errore allocazione array \n");
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


// creazione figli

for (int i = 0 ; i < N ; ++i){

if (( pid[i] = fork()  ) < 0){
printf("errore creazione figli \n");
exit(6);
}


//codice figlio

if ( pid == 0){

// nel caso di errore nel figlio ritorno -1


//codice figlio

for( int j = 0 ; j < N ; ++j){

close(piped[j][0]);

if ( j != i ){
close(piped[j][1]);
}

}


if ((fd = open(argv[i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}

int j = 0;
int count = 1;

while ( read(fd,&(linea[j]),1) ){

if ( linea[j] == '\n' ){

//linea[j+1] = '\0';
len = j+1;

/*
for (int k = len - 1; k < strlen(linea) ; ++k){

linea[k] = ' ';
}
*/
count++;

j = 0;
}
else{

++j;

}

}

printf("il figlio di indice %d ha trovato la linea di numero %d \n" ,i,count);

write(piped[i][1], linea ,len -1);

exit(len);
}

}

// codice padre

for ( int j = 0; j < N ; ++j){

close(piped[j][1]);

}

if ( (fdw = open ("Ultime_Linee",O_WRONLY) ) < 0 ){
printf("errore apertura file \n");
exit(7);
}

for (int i = 0 ; i < N ; ++i ){

nr = read(piped[i][0] , linea , sizeof(linea));

if ( nr != sizeof(linea) ){
printf("errore lettura \n");
exit(8);
}

int j = 0 ;
while( linea[j] != '\n'){

write(fdw, &(linea[j]),1);
++j;

}
write(fdw, &(linea[j]),1);

printf("il figlio di pid %d ha scritto ultima linea sul file Ultime_Linee \n",pid[i]);
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
