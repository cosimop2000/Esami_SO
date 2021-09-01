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


int main(int argc,char **argv){

int pid;
int pidFiglio;
int N; 
int len;
int nlinea;
char ch; // y or n
int fd;
int status;
int ritorno;
char linea[256];
pipe_t *piped; // tra padre e figlio
pipe_t *p; // tra figlio e padre


//controllo parametri


if (argc != 3 ){
printf("numero parametri scorretto \n");
exit(1);
}

N = 2 ;

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

// creazione figli

for (int i = 0 ; i < N ; ++i){

if (( pid = fork()  ) < 0){
printf("errore creazione figli \n");
exit(7);
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
len = 0;
nlinea = 0;

while ( read(fd,&(linea[j]),1) ){

if ( linea[j] == '\n'){

linea[j+1] = '\0';

len = j+1;

printf("il figlio %d ha ricevuto la linea %s",getpid(),linea);

j = 0;

write(piped[i][1], &len,sizeof(len));

read(p[i][0],&ch,1);

if ( ch == 'y' ){

write(piped[i][1],linea,len);

++nlinea;
}


}
else
{
++j;
}
}

 
exit(nlinea);
}

}

// codice padre

for ( int j = 0; j < N ; ++j){

close(piped[j][1]);
close(p[j][0]);

}


int valore;

nlinea=0;

while ( read(piped[0][0],&valore,sizeof(valore)) ){

++nlinea;

read(piped[1][0],&len,sizeof(len));

if ( len == valore ){

ch='y';

}
else{

ch='n';
}

write(p[0][1],&ch,1);
write(p[1][1],&ch,1);


if ( ch == 'y'){

read(piped[0][0],linea,valore);

linea[len-1] = '\0';

printf("la linea %d --> %s nel file 1 \n",nlinea,linea);

read(piped[1][0],linea,len);

linea[len-1] = '\0';

printf("la linea %d --> %s nel file 2 \n",nlinea,linea);

}
else{

printf("la linea %d e' diversa nei due file \n",nlinea);

}


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
printf ( "figlio ha ritornato -1 problemi \n");
}
else
printf ( "figlio %d ha ritornato %d \n",pidFiglio,ritorno);

}

}

exit(0);
}
