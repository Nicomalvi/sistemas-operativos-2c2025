#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int n;
int numero_maldito;
void handler_muerte_hijo(int algo);
int pids[10];

int generate_random_number() {
    //Funcion para cargar nueva semilla para el numero aleatorio
    srand(time(NULL) ^ getpid());
    return (rand() % n);
}

void handler_maldito(int sig) { //el parametro de los handler era el numero de la señal
    int n_azar = generate_random_number();
    if (n_azar == numero_maldito) {
        printf("papa soy yo !!! NOOOOOOO, %d\n", getpid());
        exit(0);
    }
}

void subrutina_hijo() {
    signal(SIGTERM, handler_maldito);
    while (1); //espero a la señal de papá, sino seguriía el ciclo de pids
    //el while funciono mejor que wait(null)
}

int main(int argc, char const *argv[]) {

    n = atoi(argv[1]);
    int rondas = atoi(argv[2]);
    numero_maldito = atoi(argv[3]);

    // Completar
    int pid_padre = getpid();

    for (int i = 0; i < n; i++){
        pids[i] = fork();
        if (pids[i] == 0) {
            printf("entro a subrutina hijo\n");
            subrutina_hijo(); //duermo al hijo, solo actúa al recibir señal
        }
    }   //a partir de acá solo el padre debería ejecutar el código:

    signal(SIGCHLD, handler_muerte_hijo);
    for (int i = 0; i < rondas; i++){
        printf("Ronda %d:\n", i);
        for (int j = 0; j < n; j++){
            if (pids[j] != 0) { //si el hijo sigue vivo...
            kill(pids[j], 15);
            sleep(1);
            }
        }
    }

    for (int i = 0; i < n; i++){
        if (pids[i] != 0) {
        printf("ha ganado el hijo numero %d, nombre %d\n", i, pids[i]);
        kill(pids[i], 9); //lo mato
        }
    }

    exit(0);
}

void handler_muerte_hijo(int algo) {
    //si haces un wait luego de un sigchild, funcionan "a la vez"
    //wait entonces devuelve el pid del hijo que murió y causo el sigchild

    int pid_hijo = wait(NULL);
    for (int k = 0; k < n; k++){
        if (pids[k] == pid_hijo) {
        pids[k] = 0;
        //la marca de muerte es pid = 0 que sabemos no es un proceso importante
        }
    }
}