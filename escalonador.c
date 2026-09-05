#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char nome[32];
    int periodo;
    int deadline;
    int duracao;
    int ordem;

    int restante;
    int deadlineAtual;
    int proximaChegada;
    int concluidas;
    int perdidas;
    int mortas;

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

int tokenizacao(char *entrada, char *tokens[]) {

    int n = 0;
    char *token = strtok(entrada, " \t\r\n");

    while (token != NULL && n < 4) {
        tokens[n] = token;
        n++;
        token = strtok(NULL, " \t\r\n");
    }

    return n;
}

int tokensTarefas(char *entrada, Tarefa *tarefa) {

    char linhaEntrada[256];
    strncpy(linhaEntrada, entrada, sizeof(linhaEntrada) - 1);
    linhaEntrada[sizeof(linhaEntrada) - 1] = '\0';

    char *tokens[4];

    int totalTokens = tokenizacao(linhaEntrada, tokens);

    if (totalTokens < 4) {
        fprintf(stderr, "Erro faltou passar 4 itens\n");
        return -1;
    }

    if (parseIntPositivo(tokens[1], &tarefa->periodo) != 0 ||
        parseIntPositivo(tokens[2], &tarefa->deadline) != 0 ||
        parseIntPositivo(tokens[3], &tarefa->duracao)  != 0) {

        fprintf(stderr, "Erro passe apenas numeros positivos no txt  \n");
        return -1;

    }

    if (tarefa->duracao > tarefa->deadline || tarefa->deadline > tarefa->periodo) {
        fprintf(stderr, "Erro passe conforme essa regra  C <= D <= P\n");
        return -1;
    }

    strncpy(tarefa->nome, tokens[0], sizeof(tarefa->nome) - 1);
    tarefa->nome[sizeof(tarefa->nome) - 1] = '\0';
    return 0;

}

void inicializaTarefas(Tarefa tarefas[], int totalTarefas) {

    for (int i = 0; i < totalTarefas; i++) {

        tarefas[i].restante = 0;
        tarefas[i].deadlineAtual = 0;
        tarefas[i].proximaChegada = 0;
        tarefas[i].concluidas = 0;
        tarefas[i].perdidas = 0;
        tarefas[i].mortas = 0;

    }
}

int temPrioridadeMaior(Tarefa *candidata, Tarefa *melhorAtual, int usaEdf) {

    if (usaEdf == 1) {

        if (candidata->deadlineAtual != melhorAtual->deadlineAtual) {
            return candidata->deadlineAtual < melhorAtual->deadlineAtual;
        }

    } else {

        if (candidata->periodo != melhorAtual->periodo) {
            return candidata->periodo < melhorAtual->periodo;
        }
    }

    return candidata->ordem < melhorAtual->ordem;
}


Tarefa *escolheMelhorTarefa(Tarefa tarefas[], int totalTarefas, int usaEdf) {

    Tarefa *melhor = NULL;

    for (int i = 0; i < totalTarefas; i++) {

        if (tarefas[i].restante == 0) {
            continue;
        }

        if (melhor == NULL || temPrioridadeMaior(&tarefas[i], melhor, usaEdf)) {
            melhor = &tarefas[i];
        }
    }

    return melhor;
}


void escalonaTarefas(Tarefa tarefas[], int totalTarefas, int tempoTotal, int usaEdf) {

    for (int instante = 0; instante < tempoTotal; instante++) {

        for (int i = 0; i < totalTarefas; i++) {

            if (instante == tarefas[i].proximaChegada) {
                tarefas[i].restante = tarefas[i].duracao;
                tarefas[i].deadlineAtual = instante + tarefas[i].deadline;
                tarefas[i].proximaChegada += tarefas[i].periodo;
            }
        }
    }

    Tarefa *executando = escolheMelhorTarefa(tarefas, totalTarefas, usaEdf);

    if (executando != NULL) {
        executando->restante--;

        if (executando->restante == 0) {
                executando->concluidas++;
        }
    }
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

    Tarefa tarefas[64];
    int totalTarefas = 0;

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        if (totalTarefas >= 64) {
            fprintf(stderr, "Erro passe no max 64 tarwfas\n");
            fclose(arquivo);
            return -1;
        }

        if (tokensTarefas(linha, &tarefas[totalTarefas]) != 0) {
            fclose(arquivo);
            return -1;
        }

        tarefas[totalTarefas].ordem = totalTarefas;
        totalTarefas++;
    }

    if (totalTarefas == 0) {
        fprintf(stderr, "Erro nenhuma tarefa econtrada\n");
        fclose(arquivo);
        return -1;
    }

    fclose(arquivo);
    
    inicializaTarefas(tarefas, totalTarefas);
    escalonaTarefas(tarefas, totalTarefas, tempoTotal, usaEdf);

    for (int i = 0; i < totalTarefas; i++) {

    printf("%s: concluidas=%d perdidas=%d mortas=%d\n",
           tarefas[i].nome, tarefas[i].concluidas, tarefas[i].perdidas, tarefas[i].mortas);

    }

    return 0;

}