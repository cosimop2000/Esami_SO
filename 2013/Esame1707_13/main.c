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

int main(int argc,char **argv){

int pid;
int pidFiglio;
char C;
int M;
long int occ;
int fd;
int num;
int status;
int ritorno;
pipe_t *piped; // figlio e padre
pipe_t *p; // padre invia i caratteri al figlio


//controllo parametri

if (argc < 2){
printf("numero parametri scorretto \n");
exit(1);
}

for ( int i = 0  ; i < argc - 1 ; ++i ){

if ( (fd=open ( argv[i+1] , O_RDONLY ) ) < 0 ){
printf("errore nella apertura del file \n");
exit(2);
}

}

M = argc - 1;


//pipe

piped = malloc( M * sizeof(pipe_t));
if (piped == NULL){
printf("errore allocazione pipe \n");
exit(3);
}

p = malloc( M * sizeof(pipe_t));
if (p == NULL){
printf("errore allocazione pipe \n");
exit(4);
}

for ( int i = 0; i < M ; ++i){
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

for (int i = 0 ; i < M ; ++i){

if (( pid = fork()  ) < 0){
printf("errore creazione figli \n");
exit(7);
}


//codice figlio

if ( pid == 0){

for ( int j = 0 ; j < M ; ++j){

close(piped[j][0]);
close(p[j][1]);

if ( i != j ){
close(piped[j][1]);
close(p[j][0]);
}

}

// nel caso di errore nel figlio ritorno -1

//codice figlio


if ((fd = open(argv[i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}

num = 0;

char ch;

while ( read(p[i][0],&ch,1) ){

occ = 0;

while (read (fd , &C ,1) > 0){

if( ch == C){
occ++;
num = 1;
}
}

lseek(fd, 0L ,0);

write(piped[i][1],&occ,sizeof(long int));

}


exit(num);
}
}



// codice padre

for ( int j = 0; j < M ; ++j){

close(piped[j][1]);
close(p[j][0]);

} 

while( read(0,&C,1)){

// leggo dallo stdin fino alla EOF ( ctrl + D )


for ( int j = 0 ; j < M ; ++j){
write(p[j][1],&C,1);
}

for ( int j = 0 ; j < M ; ++j){
read(piped[j][0],&occ,sizeof(long int));

printf("il figlio di indice %d ha trovato %ld occorrenze del carattere %c nel file %s \n",j,occ,C,argv[j+1]);
}


}

// chiudo i lati di scrittura delle pipe
// in modo che i figli si sblocchino
// dalle letture 
for ( int i = 0 ; i < M ; ++i ){
close(piped[i][0]);
close(p[i][1]);
}




for( int i = 0 ; i < M; ++i){

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
