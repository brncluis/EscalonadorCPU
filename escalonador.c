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

int parseIntPositivo(char *texto, int *destino) {

    char *fim;
    long valor = strtol(texto, &fim, 10);

    if (fim == texto) {
        return -1;
    }

    if (*fim != '\0' && *fim != '\n' && *fim != '\r') {
        return -1;
    }

    if (valor <= 0) {
        return -1;
    }

    *destino = (int) valor;
    return 0;
    
}

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

    FILE *arquivo = fopen(argv[2], "r");

    if (arquivo == NULL) {
        fprintf(stderr, "Erro arquivo nao foi aberto \n");
        return -1;
    }

    char linha[256];
    int tempoTotal;

    if (fgets(linha, sizeof(linha), arquivo) == NULL) {
        fprintf(stderr, "Arquivo vazio\n");
        fclose(arquivo);
        return -1;
    }

    if (parseIntPositivo(linha, &tempoTotal) != 0) {
        fprintf(stderr, "Erro: tempo invalido\n");
        fclose(arquivo);
        return -1;
    }

    fclose(arquivo);

    return 0;

}