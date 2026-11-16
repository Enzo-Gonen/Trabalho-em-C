/* Autor: Eloi Junior */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

void limpar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

/* Le um inteiro validado entre min e max (repete ate a entrada ser valida). */
int ler_inteiro(const char *msg, int min, int max) {
    char buf[32];
    char *fim;
    long valor;

    while (1) {
        printf("%s", msg);
        if (fgets(buf, sizeof(buf), stdin) == NULL) {
            printf("\nEntrada encerrada. Saindo do jogo.\n");
            exit(0);
        }
        if (strchr(buf, '\n') == NULL) {
            limpar_buffer();
            printf("Entrada muito longa. Tente novamente.\n");
            continue;
        }
        valor = strtol(buf, &fim, 10);
        if (fim == buf || *fim != '\n' || valor < min || valor > max) {
            printf("Entrada invalida. Digite um numero entre %d e %d.\n", min, max);
            continue;
        }
        return (int)valor;
    }
}

/* Le uma string nao vazia, sem o '\n'. */
void ler_texto(const char *msg, char *destino, int tamanho) {
    while (1) {
        printf("%s", msg);
        if (fgets(destino, tamanho, stdin) == NULL) {
            printf("\nEntrada encerrada. Saindo do jogo.\n");
            exit(0);
        }
        if (strchr(destino, '\n') == NULL) {
            limpar_buffer();
        }
        destino[strcspn(destino, "\n")] = '\0';
        if (destino[0] != '\0') {
            return;
        }
        printf("O texto nao pode ser vazio.\n");
    }
}

int rolar_dado(int lados) {
    return rand() % lados + 1;
}

void linha(char c) {
    for (int i = 0; i < 40; i++) {
        putchar(c);
    }
    putchar('\n');
}

void pausar(void) {
    char buf[8];
    printf("\n[ENTER para continuar]");
    if (fgets(buf, sizeof(buf), stdin) != NULL && strchr(buf, '\n') == NULL) {
        limpar_buffer();
    }
}
