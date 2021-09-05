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

int main(int argc,char **argv){

int pid;
int pidFiglio;
int N;
int len;
int nlinee;
int n;
int fd;
int fdw;
int status;
int ritorno;
char linea[200];
pipe_t p;
pipe_t *piped; // tra padre e figlio

//controllo parametri


if (argc < 3 ){
printf("numero parametri scorretto \n");
exit(1);
}

N = argc - 1;

for ( n = 0 ; n < N ; ++n){

if ((fd = open(argv[n+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(2);
}

}


// creazione file

if ((fdw = creat("/tmp/CosimoPavone",PERM)) <0 ){
printf("errore creazione file \n");
exit(3);
}


//pipe

if ( pipe(p) < 0 ){
printf("errore creazione pipe \n");
exit(4);
} 

// figlio speciale

if (( pid = fork()  ) < 0){
printf("errore creazione figli \n");
exit(5);
}

if ( pid == 0){

close(p[0]);

close(0);

if ((fd = open(argv[1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}

close(1);
dup(p[1]);

close(p[1]);

execlp("wc","wc","-l",(char *)0);

// solo in caso di errore
exit(-1);
}

//codice padre

close(p[1]);

int j = 0;

while ( read(p[0],&(linea[j]),1)){

++j;

}

if ( j != 0 ){

linea[j] = '\0';
nlinee = atoi(linea);

}else{

exit(-1);

}

close(p[0]);

//pipe

piped = malloc( N * sizeof(pipe_t));
if (piped == NULL){
printf("errore allocazione pipe \n");
exit(6);
}


for ( n = 0; n < N ; ++n){
if ( pipe(piped[n]) < 0 ){
printf("errore creazione pipe \n");
exit(7);
}

}


// creazione figli

for (n = 0 ; n < N ; ++n){

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

if ( j != n ){
close(piped[j][1]);
}


}


if ((fd = open(argv[n+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}


int j = 0;
len = 0;

while (read(fd,&(linea[j]),1)){

if ( linea[j] == '\n' ){

linea[j+1] = '\0';

printf("il figlio con indice %d e pid %d ha letto la linea %s \n",n,getpid(),linea);

len = j+1;

write(piped[n][1],&len,sizeof(int));
write(piped[n][1],linea,len);

j = 0;

}

else{
++j;
}

}

exit(len);
}

}


// codice padre

for ( int j = 0; j < N ; ++j){

close(piped[j][1]);

}


for ( int j = 1 ; j <= nlinee ; ++j){


for (n = 0 ; n < N ; ++n){

read(piped[n][0],&len,sizeof(int));

read(piped[n][0],linea,len);

linea[len] = '\0';

printf("il figlio con indice %d ha letto la linea numero %d %s \n",n,j,linea);


write(fdw,linea,len);

}

}

for( n = 0 ; n < N+1; ++n){

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
