/* Autor: Enzo Goncalves */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "jogador.h"
#include "historia.h"
#include "arquivo.h"
#include "utils.h"

int main(void) {
    Personagem *jogador;
    char termo[50];
    int opcao;

    srand((unsigned)time(NULL));

    jogador = malloc(sizeof(*jogador));
    if (jogador == NULL) {
        printf("Erro: nao foi possivel alocar memoria.\n");
        return EXIT_FAILURE;
    }

    do {
        printf("\n");
        linha('=');
        printf("        RPG - AVENTURA\n");
        linha('=');
        printf("1 - Novo jogo\n");
        printf("2 - Continuar jogo salvo\n");
        printf("3 - Ver historico\n");
        printf("4 - Buscar no historico\n");
        printf("0 - Sair\n");
        opcao = ler_inteiro("> ", 0, 4);

        switch (opcao) {
            case 1:
                jogador_criar(jogador);
                registrar_evento(jogador, "Novo jogo iniciado");
                if (historia_prologo(jogador)) {
                    historia_vila(jogador);
                }
                break;
            case 2:
                if (carregar_jogo(jogador)) {
                    printf("\nBem-vindo de volta, %s!\n", jogador->nome);
                    historia_vila(jogador);
                }
                break;
            case 3:
                exibir_historico();
                break;
            case 4:
                ler_texto("Termo de busca: ", termo, sizeof(termo));
                buscar_historico(termo);
                break;
            default:
                break;
        }
    } while (opcao != 0);

    free(jogador);
    printf("\nAte a proxima aventura!\n");
    return EXIT_SUCCESS;
}
