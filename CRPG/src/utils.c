/* Autor: Eloi Junior */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <limits.h>
#include "utils.h"

void limpar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

/* Le um inteiro validado entre min e max (repete ate a entrada ser valida). */
int ler_inteiro(const char *msg, int min, int max) {
    char buf[64];
    char *inicio, *fim;
    long valor;

    if (min > max) {
        fprintf(stderr, "Intervalo de entrada invalido.\n");
        return min;
    }
    for (;;) {
        printf("%s", msg);
        if (fgets(buf, sizeof(buf), stdin) == NULL) {
            printf("\nEntrada encerrada. Saindo do jogo.\n");
            exit(EXIT_SUCCESS);
        }
        if (strchr(buf, '\n') == NULL && !feof(stdin)) {
            limpar_buffer();
            printf("Entrada muito longa. Tente novamente.\n");
            continue;
        }
        inicio = buf;
        while (isspace((unsigned char)*inicio)) ++inicio;
        errno = 0;
        valor = strtol(inicio, &fim, 10);
        if (inicio == fim || errno == ERANGE) {
            printf("Entrada invalida. Digite um numero entre %d e %d.\n", min, max);
            continue;
        }
        while (isspace((unsigned char)*fim)) ++fim;
        if (*fim != '\0' || valor < min || valor > max || valor < INT_MIN || valor > INT_MAX) {
            printf("Entrada invalida. Digite um numero entre %d e %d.\n", min, max);
            continue;
        }
        return (int)valor;
    }
}

void ler_texto(const char *msg, char *destino, int tamanho) {
    if (destino == NULL || tamanho < 2) return;
    for (;;) {
        size_t n;
        printf("%s", msg);
        if (fgets(destino, tamanho, stdin) == NULL) {
            printf("\nEntrada encerrada. Saindo do jogo.\n");
            exit(EXIT_SUCCESS);
        }
        n = strlen(destino);
        if (n > 0 && destino[n - 1] == '\n') {
            destino[--n] = '\0';
        } else if (!feof(stdin)) {
            limpar_buffer();
            destino[0] = '\0';
            printf("Texto muito longo. Limite de %d caracteres.\n", tamanho - 1);
            continue;
        }
        if (n > 0 && destino[n - 1] == '\r') destino[--n] = '\0';
        while (n > 0 && isspace((unsigned char)destino[n - 1])) destino[--n] = '\0';
        if (destino[0] != '\0') return;
        printf("O texto nao pode ser vazio.\n");
    }
}

int rolar_dado(int lados) {
    unsigned int faixa;
    unsigned int limite;
    unsigned int valor;

    if (lados <= 0 || lados > RAND_MAX) return 0;
    faixa = (unsigned int)RAND_MAX + 1u;
    limite = faixa - (faixa % (unsigned int)lados);
    do {
        valor = (unsigned int)rand();
    } while (valor >= limite);
    return (int)(valor % (unsigned int)lados) + 1;
}

void linha(char c) {
    for (int i = 0; i < 40; i++) {
        putchar(c);
    }
    putchar('\n');
}

void pausar(void) {
    char buf[128];
    printf("\n[ENTER para continuar]");
    if (fgets(buf, sizeof(buf), stdin) != NULL && strchr(buf, '\n') == NULL && !feof(stdin)) {
        limpar_buffer();
    }
}
