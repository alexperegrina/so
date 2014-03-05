#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main (int argc,char * argv[])
{
	char buff[256];
	while(1) {
	sprintf(buff,"ALARMA pid=%d\n",getpid());
	write(1, buff, strlen(buff) );
	}
}
