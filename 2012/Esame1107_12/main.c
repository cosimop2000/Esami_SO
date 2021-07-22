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
char  *Fout;
int lun_num ;
int N;
int K;
int fd;
char c;
long int len;
char linea[256];
char stringa[25];
int status;
int ritorno;
pipe_t *piped; // tra padre e figli
pipe_t *file; // tra padre e figli per la scrittura


//controllo parametri

if (argc < 4){
printf("numero parametri scorretto \n");
exit(1);
}


for ( int i = 0; i < argc -2 ; ++i){

if( (fd = open(argv[i + 1],O_RDONLY)) < 0){
printf("errore nella apertura del file %s \n",argv[i+1]);
exit(2);
}
close(fd);

}

N = argc - 2;

lun_num = strlen(argv[argc- 1]);
K = atoi(argv[argc -1]);

if ( K <= 0){
printf("numero minore uguale a 0 \n");
exit(3);
}


int x = lun_num + 1 + strlen("output.");

Fout =malloc ( x * sizeof(char));

if ( Fout == NULL ){
printf("errore nella allocazione \n");
exit(4);
}

strcpy(Fout , "output.");
strcat(Fout , argv[argc -1]);
Fout[x] = '\0';

if ( (fd = creat(Fout,PERM)) < 0 ){
printf("errore nella creazione del file \n");
exit(5);
}
close(fd);

//pipe

piped = malloc( N * sizeof(pipe_t));
if (piped == NULL){
printf("errore allocazione pipe \n");
exit(6);
}


file = malloc( N * sizeof(pipe_t));
if (file == NULL){
printf("errore allocazione pipe \n");
exit(7);
}


for ( int i = 0; i < N ; ++i){

if ( pipe(piped[i]) < 0 ){
printf("errore creazione pipe \n");
exit(8);
}

if ( pipe(file[i]) < 0 ){
printf("errore creazione pipe \n");
exit(9);
}
}


// creazione figli

for (int i = 0 ; i < N ; ++i){

if (( pid = fork()  ) < 0){
printf("errore creazione figli \n");
exit(10);
}


//codice figlio

if ( pid == 0){

for ( int j = 0; j < N; ++j){
close(piped[j][0]);
close(file[j][1]);
if (i != j){
close(piped[j][1]);
close(file[j][0]);

}

}

// nel caso di errore nel figlio ritorno -1
//codice figlio

if ( (fd=open(argv[i + 1],O_RDONLY)) < 0 ){
printf("errore apertura file \n");
exit(-1);
}

len = 0;

while( read (fd, &c , 1) ){

len++;

}

write(piped[i][1],&len,sizeof(len));

read(file[i][0] , stringa , sizeof(stringa));


if ( (fd= open (Fout , O_RDWR)) < 0 ){
printf("errore apertura file \n");
exit(-1);
}

strcpy(linea,argv[i + 1]);
strcat(linea, " " );
strcat(linea , stringa);
strcat(linea, "\n" );

int d = strlen(linea);

lseek (fd, 0L , SEEK_END);

write(fd,linea,d);

if (strcmp(stringa , "Equal media") == 0 ){
printf("filgio %d e' %s \n" ,getpid(),stringa);
exit(0);
}
else if (strcmp(stringa , "Sotto media") == 0 ){
printf("filgio %d e' %s \n" ,getpid(),stringa);
exit(1);
} 
else{
printf("filgio %d e' %s \n" ,getpid(),stringa);
exit(2);
}
}

}

// codice padre

for ( int j = 0; j < N  ; ++j){

close(piped[j][1]);
close(file[j][0]);

}

char s1[25] = "Sopra media";
char s2[25] ="Equal media";
char s3[25] = "Sotto media";

for ( int i = 0 ; i < N ; ++i ){

read (piped[i][0] , &len ,sizeof(len));

if ( len > K ){
write(file[i][1],s1,strlen(s1) +1 );
}
else if ( len < K){
write(file[i][1],s3,strlen(s3) +1 );
}
write(file[i][1],s2,strlen(s2) +1 );

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
