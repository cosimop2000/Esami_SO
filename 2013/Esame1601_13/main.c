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
	char c;
	long int occ;
	char cx;
	long int occx;
	} struttura;



int main(int argc,char **argv){

int pid;
int pidFiglio;
char C;
char cx;
int N;
int fd;
int status;
int ritorno;
struttura *cur;
struttura tmp;
pipe_t *piped; // tra padre e figli


//controllo parametri

if (argc < 4){
printf("numero parametri scorretto \n");
exit(1);
}

for ( int i = 0  ; i < argc - 2 ; ++i ){

if ( (fd=open ( argv[i+1] , O_RDONLY ) ) < 0 ){
printf("errore nella apertura del file \n");
exit(2);
}

}

N = argc - 2 ;
C = *argv[argc -1 ];

if ( (strlen(argv[argc - 1]) !=1) || (isalpha(C) == 0) ){
printf("no singolo carattere alfabetico \n");
exit(3);
}

// creazione struct

cur = malloc ( N * sizeof(struttura));
if ( cur == NULL ){
printf("errore nella allocazione della memoria \n");
exit(4);
}


//pipe

piped = malloc( N * sizeof(pipe_t));
if (piped == NULL){
printf("errore allocazione pipe \n");
exit(5);
}

for ( int i = 0; i < N ; ++i){
if ( pipe(piped[i]) < 0 ){
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

for ( int j = 0; j < N ; ++j){

close(piped[j][0]);
if (i != j){
close(piped[j][1]);
}

}

// nel caso di errore nel figlio ritorno -1

//codice figlio


if ((fd = open(argv[i+1],O_RDONLY)) <0 ){
printf("errore apertura file \n");
exit(-1);
}

cur[i].c = C;

char tmp;

if ( islower(C) ){
tmp = toupper(C);
}
else{
tmp = tolower(C);

}

cur[i].cx = tmp;

while ( read (fd, &cx ,1)){

if ( cx == C ){
cur[i].occ = cur[i].occ  +1; 
}

if ( cx == tmp ){
cur[i].occx = cur[i].occx  +1; 
}

}

write(piped[i][1], &(cur[i]), sizeof(struttura));

if ( cur[i].occ > cur[i].occx){
exit(1);
} 
else exit(0);


}
}



// codice padre

for ( int j = 0; j < N ; ++j){

close(piped[j][1]);

}

int nr = 0;

for ( int i = 0 ; i < N ; ++i ){

nr = read(piped[i][0] , &tmp ,sizeof(struttura));

if ( nr != sizeof(struttura) ){
printf("errore in lettura \n");
exit(8);
}

printf("il figlio di indice %d ha trovato il carattere %c %ld volte ed il carattere %c %ld volte allo interno del file %s \n",i,tmp.c,tmp.occ,tmp.cx,tmp.occx,argv[i+1]);


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
printf ( "figlio ha ritornato -1 ==> problemi \n");
}
else
printf ( "figlio %d ha ritornato %d \n",pidFiglio,ritorno);

}

}

exit(0);
}
