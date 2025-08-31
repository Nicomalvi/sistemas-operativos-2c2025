#include <signal.h> /* constantes como SIGINT*/
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int ciclos = 0;

void handler_ya_va2(int sig){
	write("ya va!\n");

	if(ciclos==5){
	kill(SIGINT, getppid());
	//exec
	exit(0);
	}
}
void subrutina_hijo(){
	signal(SIGURG, handler_ya_va2);
	while(1);
}

int main(int argc, char* argv[]) {
	// Completar
	int pid_hijo;
	pid_hijo = fork();
	if(pid_hijo != 0){
		for(int i = 1; i<6; i++){
			write("sup\n");
			kill(pid_hijo, SIGURG);
			sleep(1);
			ciclos++;
		}
	}
	else {
	subrutina_hijo();
	}
}