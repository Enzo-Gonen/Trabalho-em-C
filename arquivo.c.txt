/* Autor: Enzo Goncalves */
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "arquivo.h"

int salvar_jogo(const Personagem *p) {
    FILE *f = fopen(ARQUIVO_SAVE, "wb");
    size_t escritos;

    if (f == NULL) {
        printf("Erro: nao foi possivel criar o arquivo de save.\n");
        return 0;
    }
    escritos = fwrite(p, sizeof(*p), 1, f);
    fclose(f);
    if (escritos != 1) {
        printf("Erro: falha ao gravar o save.\n");
        return 0;
    }
    printf("Jogo salvo com sucesso!\n");
    return 1;
}

int carregar_jogo(Personagem *p) {
    FILE *f = fopen(ARQUIVO_SAVE, "rb");
    size_t lidos;

    if (f == NULL) {
        printf("Nenhum jogo salvo encontrado.\n");
        return 0;
    }
    lidos = fread(p, sizeof(*p), 1, f);
    fclose(f);
    if (lidos != 1) {
        printf("Erro: arquivo de save corrompido ou incompleto.\n");
        return 0;
    }
    p->nome[TAM_NOME - 1] = '\0';
    p->arma[TAM_ARMA - 1] = '\0';
    if (p->hp <= 0 || p->hp > p->hp_max || p->nivel < 1) {
        printf("Erro: dados do save invalidos.\n");
        return 0;
    }
    return 1;
}

void registrar_evento(const Personagem *p, const char *evento) {
    FILE *f = fopen(ARQUIVO_HISTORICO, "a");
    time_t agora = time(NULL);
    char data[32];

    if (f == NULL) {
        printf("Aviso: nao foi possivel gravar no historico.\n");
        return;
    }
    strftime(data, sizeof(data), "%d/%m/%Y %H:%M:%S", localtime(&agora));
    fprintf(f, "[%s] %s (Nv %d): %s\n", data, p->nome, p->nivel, evento);
    fclose(f);
}

void exibir_historico(void) {
    FILE *f = fopen(ARQUIVO_HISTORICO, "r");
    char linha_txt[256];
    int total = 0;

    if (f == NULL) {
        printf("\nNenhum historico registrado ainda.\n");
        return;
    }
    printf("\n===== HISTORICO DE AVENTURAS =====\n");
    while (fgets(linha_txt, sizeof(linha_txt), f) != NULL) {
        printf("%s", linha_txt);
        total++;
    }
    fclose(f);
    if (total == 0) {
        printf("O historico esta vazio.\n");
    }
}

void buscar_historico(const char *termo) {
    FILE *f = fopen(ARQUIVO_HISTORICO, "r");
    char linha_txt[256];
    int encontrados = 0;

    if (f == NULL) {
        printf("\nNenhum historico registrado ainda.\n");
        return;
    }
    while (fgets(linha_txt, sizeof(linha_txt), f) != NULL) {
        if (strstr(linha_txt, termo) != NULL) {
            printf("%s", linha_txt);
            encontrados++;
        }
    }
    fclose(f);
    if (encontrados == 0) {
        printf("\nA busca por \"%s\" nao teve resultados.\n", termo);
    }
}
