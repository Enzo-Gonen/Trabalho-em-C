/* Autor: Enzo Goncalves */
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "arquivo.h"

#include <stdint.h>
#include <errno.h>
#ifdef _WIN32
#include <windows.h>
#endif

typedef struct {
    char magic[8];
    uint32_t version;
    uint32_t checksum;
} CabecalhoSave;

static uint32_t checksum_fnv1a(const unsigned char *dados, size_t tamanho) {
    uint32_t hash = UINT32_C(2166136261);
    for (size_t i = 0; i < tamanho; ++i) {
        hash ^= dados[i];
        hash *= UINT32_C(16777619);
    }
    return hash;
}

static int substituir_arquivo(const char *origem, const char *destino) {
#ifdef _WIN32
    return MoveFileExA(origem, destino, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH) != 0;
#else
    return rename(origem, destino) == 0;
#endif
}

int salvar_jogo(const Personagem *p) {
    const char *temporario = ARQUIVO_SAVE ".tmp";
    FILE *f;
    CabecalhoSave h = {{'R','P','G','C','S','A','V','E'}, 3u, 0u};
    int ok;

    if (!jogador_validar(p)) {
        printf("Erro: os dados do personagem nao passaram na validacao.\n");
        return 0;
    }
    h.checksum = checksum_fnv1a((const unsigned char *)p, sizeof(*p));
    f = fopen(temporario, "wb");
    if (f == NULL) {
        printf("Erro: nao foi possivel criar o arquivo temporario de save.\n");
        return 0;
    }

    ok = fwrite(&h, sizeof(h), 1, f) == 1 &&
         fwrite(p, sizeof(*p), 1, f) == 1 && fflush(f) == 0;
    if (fclose(f) != 0) ok = 0;

    if (!ok || !substituir_arquivo(temporario, ARQUIVO_SAVE)) {
        remove(temporario);
        printf("Erro ao concluir o salvamento. O save anterior foi preservado quando possivel.\n");
        return 0;
    }
    printf("Jogo salvo e verificado com sucesso!\n");
    return 1;
}

int carregar_jogo(Personagem *p) {
    FILE *f;
    char magic[8];
    uint32_t version;
    uint32_t checksum = 0;
    Personagem temporario;
    int tem_checksum = 0;
    int extra;

    if (p == NULL) return 0;
    f = fopen(ARQUIVO_SAVE, "rb");
    if (f == NULL) {
        printf("Nenhum jogo salvo encontrado.\n");
        return 0;
    }

    if (fread(magic, sizeof(magic), 1, f) != 1 ||
        memcmp(magic, "RPGCSAVE", sizeof(magic)) != 0 ||
        fread(&version, sizeof(version), 1, f) != 1) {
        fclose(f);
        printf("Save invalido ou corrompido.\n");
        return 0;
    }

    if (version == 3u) {
        tem_checksum = 1;
        if (fread(&checksum, sizeof(checksum), 1, f) != 1) {
            fclose(f);
            printf("Cabecalho do save incompleto.\n");
            return 0;
        }
    } else if (version != 2u) {
        fclose(f);
        printf("Versao de save nao suportada.\n");
        return 0;
    }

    if (fread(&temporario, sizeof(temporario), 1, f) != 1) {
        fclose(f);
        printf("Dados do save incompletos.\n");
        return 0;
    }
    extra = fgetc(f);
    {
        int erro_leitura = ferror(f);
        int erro_fechamento = fclose(f);
        if (erro_leitura || erro_fechamento != 0 || extra != EOF) {
            printf("Save possui dados extras ou erro de leitura.\n");
            return 0;
        }
    }

    if (tem_checksum && checksum_fnv1a((const unsigned char *)&temporario, sizeof(temporario)) != checksum) {
        printf("Falha na verificacao de integridade do save.\n");
        return 0;
    }

    if (!jogador_validar(&temporario)) {
        printf("Os dados do personagem no save sao invalidos.\n");
        return 0;
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
    {
        struct tm *info = localtime(&agora);
        if (info == NULL || strftime(data, sizeof(data), "%d/%m/%Y %H:%M:%S", info) == 0) {
            strcpy(data, "data-indisponivel");
        }
    }
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
