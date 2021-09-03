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
int Q;
int L;
int nlinea;
int occ;
char c;
char ch;
int q;
int fd;
int status;
int ritorno;
pipe_t *piped; // tra padre e figlio

//controllo parametri


if (argc < 5 ){
printf("numero parametri scorretto \n");
exit(1);
}

if ((fd = open(argv[1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(2);
}

L = atoi(argv[2]);
if ( L <= 0  ){
printf("numero non positivo o maggiore di 255 \n");
exit(3);
}

for (q = 0 ; q < argc - 3 ;++q) {

if ( strlen(argv[q+3]) != 1  ){
printf("non singolo carattere \n");
exit(4);
}

}

Q = argc - 3;

//pipe

piped = malloc( Q * sizeof(pipe_t));
if (piped == NULL){
printf("errore allocazione pipe \n");
exit(5);
}


for ( q = 0; q < Q ; ++q){
if ( pipe(piped[q]) < 0 ){
printf("errore creazione pipe \n");
exit(6);
}

}

// creazione figli

for (q = 0 ; q < Q ; ++q){

if (( pid = fork()  ) < 0){
printf("errore creazione figli \n");
exit(7);
}


//codice figlio

if ( pid == 0){


// nel caso di errore nel figlio ritorno -1


//codice figlio

for ( int j = 0; j < Q ; ++j){

close(piped[j][0]);

if (q != j){
close(piped[j][1]);
}

}


if ((fd = open(argv[1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}

occ = 0;
c = argv[q+3][0];

while (read(fd,&ch,1)){

if ( c == ch ){

occ++;

}


if (ch == '\n'){

// read(p[q][0],&sincronizzazione,1);

write(piped[q][1],&occ,sizeof(int));

occ = 0;

}


}


exit(occ);
}

}

// codice padre

for ( int j = 0; j < L ; ++j){

close(piped[j][1]);

// chiusura altra pipe

}


for ( nlinea = 1 ; nlinea <= L ; ++nlinea){

printf("Linea %d : \n",nlinea);

for ( q = 0  ; q < Q ; ++q ){

// write(p[q][1],&sincronizzazione,1);

read(piped[q][0],&occ,sizeof(int));


printf("il figlio di indice %d ha trovato %d occorrenze di '%c' \n",q,occ,argv[q+3][0]);


}

}


for( q = 0 ; q < Q; ++q){

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
