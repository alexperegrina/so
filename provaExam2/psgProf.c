/*
>psg userid
Mitjançant forks,pipes i execlp ha d'executar l'equivalent a la comanda:
$> ps -e -o user -no-heading|grep userid|wc -l
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

void error_y_exit(char*msg, int exit_code) {
	perror(msg);
	exit(exit_code);
}

void Usage(void) {
	char msg[] = "Usage= psg userid\n";
	write(2,msg,strlen(msg));
}
void main(int argc,char *argv[]) {
	int p_ps_g[2],p_g_wc[2];
	int pid_ps,pid_grep,pid_wc;
	if(argc != 2) Usage();
	if(pipe(p_ps_g)<0) error_y_exit("pipe ps i grep",1);
	pid_ps = fork();
	if(pid_ps < 0) error_y_exit("fork ps",1);
	else if(pid_ps == 0) {
		close(1);
		dup(p_ps_g[1]);
		close(p_ps_g[1]);
		close(p_ps_g[0]);
		execlp("ps","ps","-e","-o","user","--no-heading",(char *) NULL);
		error_y_exit("execlp ps",1);
	}
	close(p_ps_g[1]);
	
	if(pipe(p_g_wc)<0) error_y_exit("pipe grep i wc",1);
	pid_grep = fork();
	if(pid_grep < 0) error_y_exit("fork grep",1);
	else if(pid_grep == 0) {
		dup2(p_ps_g[0],0);
		close(p_ps_g[0]);
		close(p_g_wc[0]);
		dup2(p_g_wc[1],1);
		close(p_g_wc[1]);
		
		execlp("grep","grep",argv[1],(char *) NULL);
		error_y_exit("execlp grep",1);
	}
	close(p_ps_g[0]);
	close(p_g_wc[1]);
	
	pid_wc = fork();
	if(pid_wc < 0) error_y_exit("fork wc",1);
	else if(pid_wc == 0) {
		dup2(p_g_wc[0],0);
		close(p_g_wc[0]);
		execlp("wc","wc","-l",(char *) NULL);
		//execlp("cat","cat",(char *) NULL);
		error_y_exit("execlp wc",1);
	}
	close(p_g_wc[0]);
	while(waitpid(-1,NULL,0)>0);
}





