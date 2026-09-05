#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char nome[32];
    int periodo;
    int deadline;
    int duracao;
    int ordem;
} Tarefa;

int main(int argc, char *argv[]) {

    if (argc != 3) {
        fprintf(stderr, "Erro: uso %s (rate ou edf) <arquivo>\n", argv[0]);
        return -1;
    }

    int usaEdf = 0;
    
    if (strcmp(argv[1], "edf") == 0) {

        usaEdf = 1;

    }

    else if (strcmp(argv[1], "rate") == 0) {

        usaEdf = 0;

    }

    else {

        fprintf(stderr, "Erro digitou %s um algoritmo invalido use 'rate' ou 'edf'\n ", argv[1]);
        return -1;

    }

    return 0;

}