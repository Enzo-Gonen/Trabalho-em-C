/* Autor: Lyvia Vitoria */
#include <stdio.h>
#include <string.h>
#include "npc.h"
#include "habilidades.h"
#include "utils.h"

static const NPC NPCS[TOTAL_NPCS] = {
    {"Elias", "Anciao da vila",  "Ah, viajante... os ventos me disseram que voce viria."},
    {"Borin", "Ferreiro",        "Aco bom custa caro, mas salva vidas!"},
    {"Mara",  "Taverneira",      "Sente-se! Aqui ninguem passa fome nem sede."}
};

static const char *RUMORES[] = {
    "Dizem que o Orc Grukh teme runas antigas gravadas nas paredes da caverna.",
    "Um viajante ferido foi visto nos arredores. Ele costuma agradecer com moedas.",
    "Borin faz desconto para quem sabe negociar com educacao."
};
#define TOTAL_RUMORES (int)(sizeof(RUMORES) / sizeof(RUMORES[0]))

static void apresentar(const NPC *n) {
    printf("\n%s, o %s: \"%s\"\n", n->nome, n->titulo, n->saudacao);
}

static int preco(const Personagem *p, int base) {
    return p->desconto ? base * 8 / 10 : base;
}

static void dialogo_anciao(Personagem *p) {
    const NPC *n = &NPCS[NPC_ANCIAO];
    int op;

    apresentar(n);
    do {
        printf("\n1 - Perguntar sobre a ameaca na caverna\n");
        printf("2 - Pedir ajuda para a jornada (Carisma)\n");
        printf("3 - Perguntar sobre a fraqueza do Orc (Inteligencia)\n");
        printf("0 - Despedir-se\n");
        op = ler_inteiro("> ", 0, 3);

        if (op == 1) {
            printf("\nElias: \"O Orc Grukh se instalou na caverna ao norte e aterroriza a vila.\n");
            printf("Recomendo chegar la pelo menos no nivel 3.\"\n");
        } else if (op == 2) {
            if (p->ajuda_anciao) {
                printf("\nElias: \"Ja lhe dei o que eu podia, jovem.\"\n");
            } else if (teste_habilidade(p, ATR_CARISMA, 12, "Persuadir").sucesso) {
                p->pocoes += 2;
                p->ajuda_anciao = 1;
                p->reputacao++;
                printf("\nElias sorri e entrega 2 pocoes a voce!\n");
            } else {
                p->reputacao--;
                printf("\nElias: \"Nao confio em quem pede tanto...\" (reputacao -1)\n");
            }
        } else if (op == 3) {
            if (p->sabe_fraqueza) {
                printf("\nElias: \"Voce ja conhece o segredo. Use-o bem.\"\n");
            } else if (teste_habilidade(p, ATR_INTELIGENCIA, 13, "Decifrar runas").sucesso) {
                p->sabe_fraqueza = 1;
                printf("\nVoce decifra as runas: o Orc tem um ferimento antigo no flanco!\n");
                printf("(O Orc comecara a luta com 20 HP a menos.)\n");
            } else {
                printf("\nAs runas sao complicadas demais para voce...\n");
            }
        }
    } while (op != 0);
}

static void dialogo_ferreiro(Personagem *p) {
    const NPC *n = &NPCS[NPC_FERREIRO];
    int op;

    apresentar(n);
    do {
        printf("\nMoedas: %d | Dano da arma: %d\n", p->dinheiro, p->dano_arma);
        printf("1 - Comprar pocao (%d moedas)\n", preco(p, 15));
        printf("2 - Afiar arma +5 dano (%d moedas)\n", preco(p, 40));
        printf("3 - Barganhar (Carisma, uma tentativa)\n");
        printf("0 - Sair\n");
        op = ler_inteiro("> ", 0, 3);

        if (op == 1) {
            if (p->dinheiro >= preco(p, 15)) {
                p->dinheiro -= preco(p, 15);
                p->pocoes++;
                printf("\nVoce comprou uma pocao.\n");
            } else {
                printf("\nBorin: \"Volte quando tiver moedas.\"\n");
            }
        } else if (op == 2) {
            if (p->dano_arma >= 30) {
                printf("\nBorin: \"Sua arma ja esta no limite do que sei fazer.\"\n");
            } else if (p->dinheiro < preco(p, 40)) {
                printf("\nBorin: \"Isso custa %d moedas, amigo.\"\n", preco(p, 40));
            } else {
                p->dinheiro -= preco(p, 40);
                p->dano_arma += 5;
                snprintf(p->arma, TAM_ARMA, "Espada +%d", (p->dano_arma - 15) / 5);
                printf("\nBorin afia sua lamina. Agora voce empunha: %s.\n", p->arma);
            }
        } else if (op == 3) {
            if (p->barganhou) {
                printf("\nBorin: \"Ja discutimos preco. Chega.\"\n");
            } else {
                p->barganhou = 1;
                if (teste_habilidade(p, ATR_CARISMA, 14, "Barganha").sucesso) {
                    p->desconto = 1;
                    printf("\nBorin ri: \"Voce tem lingua boa! 20%% de desconto.\"\n");
                } else {
                    printf("\nBorin fecha a cara: \"Preco e preco.\"\n");
                }
            }
        }
    } while (op != 0);
}

static void dialogo_taverneira(Personagem *p) {
    const NPC *n = &NPCS[NPC_TAVERNEIRA];
    int op;

    apresentar(n);
    do {
        printf("\nMoedas: %d\n", p->dinheiro);
        printf("1 - Ouvir rumores (5 moedas)\n");
        printf("2 - Beber uma cerveja (5 moedas, +30 stamina)\n");
        printf("3 - Jogo de dados (aposta de 10 moedas, Destreza)\n");
        printf("0 - Sair\n");
        op = ler_inteiro("> ", 0, 3);

        if (op >= 1 && op <= 3) {
            int custo = (op == 3) ? 10 : 5;
            if (p->dinheiro < custo) {
                printf("\nMara: \"Sem moedas, sem servico.\"\n");
                continue;
            }
            p->dinheiro -= custo;
            if (op == 1) {
                printf("\nMara sussurra: \"%s\"\n", RUMORES[rolar_dado(TOTAL_RUMORES) - 1]);
            } else if (op == 2) {
                p->stamina += 30;
                if (p->stamina > p->stamina_max) {
                    p->stamina = p->stamina_max;
                }
                printf("\nVoce bebe uma caneca gelada. Stamina: %d/%d.\n", p->stamina, p->stamina_max);
            } else if (teste_habilidade(p, ATR_DESTREZA, 12, "Jogo de dados").sucesso) {
                p->dinheiro += 30;
                printf("\nVoce ganhou a rodada! +20 moedas de lucro.\n");
            } else {
                printf("\nVoce perdeu a aposta de 10 moedas.\n");
            }
        }
    } while (op != 0);
}

void npc_conversar(Personagem *p, IdNpc id) {
    switch (id) {
        case NPC_ANCIAO:    dialogo_anciao(p); break;
        case NPC_FERREIRO:  dialogo_ferreiro(p); break;
        case NPC_TAVERNEIRA: dialogo_taverneira(p); break;
        default: break;
    }
}
