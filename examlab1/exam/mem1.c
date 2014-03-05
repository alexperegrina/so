#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define DEFAULT_ITERATIONS 	3
#define REGION_SIZE		4096

char *b;

void Usage() {
	char buf[80];
	sprintf(buf,"Usage: argv[1] , 1 <= argv[1] <= 5 \n");
	write(1,buf,strlen(buf));
	exit(0);
}

void error_y_exit(char *msg,int exit_status) {
       perror(msg);
       exit(exit_status);
}

int main (int argc, char *argv[])
{
	char buff[80];
	char *punterB;
	char *a;
	a = sbrk(REGION_SIZE);
	b = malloc(1024);
	punterB = b;
	if(argc <= 1) Usage();	


	sprintf (buff, "punterB: %s, A: %s, B: %s \n",punterB,a,b);
	write(1,buff,strlen(buff));

}
