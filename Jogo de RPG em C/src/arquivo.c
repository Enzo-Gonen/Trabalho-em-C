/* Autor: Enzo Goncalves */
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "arquivo.h"

typedef struct { char magic[8]; unsigned version; } CabecalhoSave;

int salvar_jogo(const Personagem *p) {
    const char *temporario = ARQUIVO_SAVE ".tmp";
    FILE *f = fopen(temporario, "wb");
    CabecalhoSave h = {{'R','P','G','C','S','A','V','E'}, 2};
    int ok = 0;
    if (!f) { printf("Erro: nao foi possivel criar o save.\n"); return 0; }
    if (fwrite(&h, sizeof(h), 1, f) == 1 && fwrite(p, sizeof(*p), 1, f) == 1 && fflush(f) == 0) ok = 1;
    if (fclose(f) != 0) ok = 0;
    if (!ok || rename(temporario, ARQUIVO_SAVE) != 0) {
        remove(temporario); printf("Erro ao concluir o salvamento.\n"); return 0;
    }
    printf("Jogo salvo com seguranca!\n");
    return 1;
}

int carregar_jogo(Personagem *p) {
    FILE *f = fopen(ARQUIVO_SAVE, "rb");
    CabecalhoSave h;
    Personagem temporario;
    if (!f) { printf("Nenhum jogo salvo encontrado.\n"); return 0; }
    if (fread(&h, sizeof(h), 1, f) != 1 || memcmp(h.magic, "RPGCSAVE", 8) != 0 || h.version != 2 ||
        fread(&temporario, sizeof(temporario), 1, f) != 1) {
        fclose(f); printf("Save invalido ou de uma versao antiga. Inicie um novo jogo.\n"); return 0;
    }
    fclose(f);
    temporario.nome[TAM_NOME - 1] = '\0';
    temporario.arma[TAM_ARMA - 1] = '\0';
    if (!temporario.nome[0] || temporario.hp_max < 1 || temporario.hp < 1 || temporario.hp > temporario.hp_max ||
        temporario.mana < 0 || temporario.mana > temporario.mana_max || temporario.stamina < 0 ||
        temporario.stamina > temporario.stamina_max || temporario.nivel < 1 || temporario.classe < CLASSE_GUERREIRO ||
        temporario.classe > CLASSE_LADINO) {
        printf("Dados do save invalidos.\n"); return 0;
    }
    *p = temporario;
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
