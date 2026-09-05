#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Erro: uso %s (rate ou edf) <arquivo>\n", argv[0]);
        return -1;
    }
    return 0;
}