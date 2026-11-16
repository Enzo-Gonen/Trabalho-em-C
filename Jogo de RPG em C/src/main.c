/* Autor: Enzo Goncalves */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "jogador.h"
#include "historia.h"
#include "arquivo.h"
#include "utils.h"
#include "ui.h"
#include "mapa.h"

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
        ui_banner("A LENDA DE ALVORADA", "Uma aventura em ASCII | Edicao Visual");
        ui_moldura("Uma vila esquecida. Um destino em suas maos.",
                   "Explore, lute, evolua e escreva sua propria historia.");
        puts("\n   [1]  Novo jogo");
        puts("   [2]  Continuar jogo salvo");
        puts("   [3]  Ver historico");
        puts("   [4]  Buscar no historico");
        puts("   [0]  Sair");
        puts("\n  ---------------------------------------------------------------");
        opcao = ler_inteiro("> ", 0, 4);

        switch (opcao) {
            case 1:
                mapa_reiniciar();
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
