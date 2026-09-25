/* Autor: Lyvia Vitoria */
#include <stdio.h>
#include "historia.h"
#include "inimigo.h"
#include "combate.h"
#include "habilidades.h"
#include "npc.h"
#include "arquivo.h"
#include "utils.h"

/* Executa um combate, registra no historico e mostra game over se necessario. */
static int lutar(Personagem *p, Inimigo *inimigo) {
    char msg[100];
    int resultado;

    printf("\nUm %s aparece!\n", inimigo->nome);
    resultado = combate_executar(p, inimigo);

    if (resultado == COMBATE_VITORIA) {
        snprintf(msg, sizeof(msg), "Venceu o combate contra %s", inimigo->nome);
    } else if (resultado == COMBATE_FUGA) {
        snprintf(msg, sizeof(msg), "Fugiu de %s", inimigo->nome);
    } else {
        snprintf(msg, sizeof(msg), "Morreu para %s", inimigo->nome);
        printf("\n============================\n");
        printf("         VOCE MORREU\n");
        printf("============================\n");
    }
    registrar_evento(p, msg);
    return resultado;
}

/* Retorna 1 se o jogador sobreviveu ao prologo. */
int historia_prologo(Personagem *p) {
    Inimigo inimigo;

    printf("\n");
    linha('=');
    printf("              AVENTURA\n");
    linha('=');
    printf("\nVoce acorda em uma floresta, %s. Esta escuro,\n", p->nome);
    printf("o vento gelido ecoa pelos seus ouvidos.\n");
    printf("\nO que voce deseja fazer?\n");
    printf("1 - Seguir pela floresta\n");
    printf("2 - Procurar uma saida\n");

    if (ler_inteiro("> ", 1, 2) == 1) {
        printf("\nVoce continua andando pela floresta...\n");
        inimigo = inimigo_criar(INIMIGO_GOBLIN);
        if (lutar(p, &inimigo) == COMBATE_DERROTA) {
            return 0;
        }
    } else {
        printf("\nVoce tenta se orientar pelas estrelas...\n");
        if (teste_habilidade(p, ATR_INTELIGENCIA, 12, "Orientacao").sucesso) {
            printf("Voce encontra uma trilha segura!\n");
            jogador_ganhar_xp(p, 20);
        } else {
            printf("Voce se perde e um lobo faminto te embosca!\n");
            inimigo = inimigo_criar(INIMIGO_LOBO);
            if (lutar(p, &inimigo) == COMBATE_DERROTA) {
                return 0;
            }
        }
    }

    printf("\nVoce encontra a pequena Vila Alvorada e sobrevive a noite!\n");
    registrar_evento(p, "Chegou a Vila Alvorada");
    pausar();
    return 1;
}

static void evento_bau(Personagem *p) {
    ResultadoTeste r;

    printf("\nVoce encontra um bau antigo com uma trava suspeita.\n");
    printf("1 - Abrir com cuidado (Destreza)\n");
    printf("2 - Arrombar (Forca)\n");
    printf("3 - Ignorar\n");

    switch (ler_inteiro("> ", 1, 3)) {
        case 1: r = teste_habilidade(p, ATR_DESTREZA, 13, "Desarmar trava"); break;
        case 2: r = teste_habilidade(p, ATR_FORCA, 12, "Arrombar"); break;
        default:
            printf("\nVoce segue seu caminho.\n");
            return;
    }
    if (r.sucesso) {
        p->dinheiro += 25;
        printf("O bau abre! Voce encontra 25 moedas.\n");
    } else {
        p->hp -= 10;
        if (p->hp < 1) {
            p->hp = 1;
        }
        printf("Uma armadilha dispara! Voce perde 10 HP (HP: %d).\n", p->hp);
    }
}

static void evento_viajante(Personagem *p) {
    printf("\nUm viajante ferido pede ajuda na beira da trilha.\n");
    printf("1 - Ajudar (gasta 1 pocao)\n");
    printf("2 - Ignorar\n");
    printf("3 - Roubar (Destreza)\n");

    switch (ler_inteiro("> ", 1, 3)) {
        case 1:
            if (p->pocoes > 0) {
                p->pocoes--;
                p->reputacao++;
                p->dinheiro += 15;
                printf("\nO viajante agradece e lhe da 15 moedas. (reputacao +1)\n");
                jogador_ganhar_xp(p, 25);
                registrar_evento(p, "Ajudou um viajante ferido");
            } else {
                printf("\nVoce nao tem pocoes para ajudar...\n");
            }
            break;
        case 3:
            if (teste_habilidade(p, ATR_DESTREZA, 12, "Furto").sucesso) {
                p->dinheiro += 20;
                p->reputacao--;
                printf("\nVoce leva 20 moedas sem ser notado. (reputacao -1)\n");
            } else {
                p->reputacao -= 2;
                printf("\nEle grita e voce foge sem nada. (reputacao -2)\n");
            }
            registrar_evento(p, "Tentou roubar um viajante");
            break;
        default:
            printf("\nVoce segue seu caminho.\n");
            break;
    }
}

static void explorar(Personagem *p) {
    Inimigo inimigo;

    printf("\nVoce se aventura pelos arredores da vila...\n");
    switch (rolar_dado(5)) {
        case 1:
        case 2:
            inimigo = inimigo_criar((TipoInimigo)(INIMIGO_LOBO + rolar_dado(2) - 1));
            lutar(p, &inimigo);
            break;
        case 3:
            evento_bau(p);
            break;
        case 4:
            evento_viajante(p);
            break;
        default:
            p->hp += 10;
            if (p->hp > p->hp_max) {
                p->hp = p->hp_max;
            }
            printf("Voce encontra ervas medicinais e recupera 10 HP (HP: %d).\n", p->hp);
            break;
    }
}

static void confrontar_chefe(Personagem *p) {
    Inimigo orc;
    int resultado;

    if (p->nivel < 3) {
        printf("\nOs aldeoes avisam: o Orc e muito forte (recomendado nivel 3+).\n");
        printf("1 - Ir mesmo assim\n2 - Voltar para a vila\n");
        if (ler_inteiro("> ", 1, 2) == 2) {
            return;
        }
    }

    orc = inimigo_criar(INIMIGO_ORC);
    printf("\nNa caverna, o Orc Grukh ronca sobre uma pilha de tesouros...\n");
    if (p->sabe_fraqueza) {
        orc.hp -= 20;
        printf("Gracas as runas de Elias, voce sabe onde ele esta ferido (-20 HP no Orc).\n");
    }

    printf("\nComo voce vai agir?\n");
    printf("1 - Atacar de frente\n");
    printf("2 - Emboscada furtiva (Destreza)\n");
    printf("3 - Tentar negociar (Carisma, dificil)\n");

    switch (ler_inteiro("> ", 1, 3)) {
        case 2:
            if (teste_habilidade(p, ATR_DESTREZA, 15, "Emboscada").sucesso) {
                orc.hp -= 25;
                printf("Voce acerta um golpe traicoeiro! (-25 HP no Orc)\n");
            } else {
                printf("Um graveto estala... o Orc acorda!\n");
            }
            break;
        case 3:
            if (teste_habilidade(p, ATR_CARISMA, 18 - p->reputacao, "Negociacao").sucesso) {
                printf("\nGrukh aceita deixar a regiao em troca de um tributo. A vila esta a salvo!\n");
                p->chefe_derrotado = 1;
                p->dinheiro += 50;
                jogador_ganhar_xp(p, 100);
                registrar_evento(p, "Negociou a paz com o Orc Grukh");
                return;
            }
            printf("\nGrukh gargalha e parte para cima de voce!\n");
            break;
        default:
            break;
    }

    if (orc.hp < 1) {
        orc.hp = 1;
    }
    resultado = lutar(p, &orc);
    if (resultado == COMBATE_VITORIA) {
        p->chefe_derrotado = 1;
        registrar_evento(p, "Derrotou o Orc Grukh - fim da aventura");
    } else if (resultado == COMBATE_FUGA) {
        printf("\nVoce volta correndo para a vila.\n");
    }
}

static void epilogo(const Personagem *p) {
    printf("\n");
    linha('=');
    printf("        FIM DA AVENTURA\n");
    linha('=');
    printf("\nA Vila Alvorada esta a salvo gracas a %s!\n", p->nome);
    jogador_status(p);
}

void historia_vila(Personagem *p) {
    int opcao = -1;

    while (p->hp > 0 && !p->chefe_derrotado && opcao != 0) {
        printf("\n========== VILA ALVORADA ==========\n");
        printf("1 - Conversar com o Anciao Elias\n");
        printf("2 - Visitar o ferreiro Borin\n");
        printf("3 - Ir a taverna de Mara\n");
        printf("4 - Descansar na estalagem (5 moedas)\n");
        printf("5 - Explorar os arredores\n");
        printf("6 - Enfrentar o Orc Grukh\n");
        printf("7 - Ver status\n");
        printf("8 - Salvar jogo\n");
        printf("0 - Voltar ao menu principal\n");
        opcao = ler_inteiro("> ", 0, 8);

        switch (opcao) {
            case 1: npc_conversar(p, NPC_ANCIAO); break;
            case 2: npc_conversar(p, NPC_FERREIRO); break;
            case 3: npc_conversar(p, NPC_TAVERNEIRA); break;
            case 4:
                if (p->dinheiro >= 5) {
                    p->dinheiro -= 5;
                    jogador_descansar(p);
                    printf("\nVoce dorme bem e acorda recuperado!\n");
                } else {
                    printf("\nVoce nao tem moedas para a estalagem.\n");
                }
                break;
            case 5: explorar(p); break;
            case 6: confrontar_chefe(p); break;
            case 7: jogador_status(p); break;
            case 8: salvar_jogo(p); break;
            default: break;
        }
    }

    if (p->chefe_derrotado) {
        epilogo(p);
    }
}
