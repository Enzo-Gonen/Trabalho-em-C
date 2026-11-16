/* Autor: Pedro Lucas */
#include <stdio.h>
#include <stdlib.h>
#include "combate.h"
#include "habilidades.h"
#include "utils.h"

#define CUSTO_ATAQUE 10
#define CUSTO_MAGIA  10

static int limitar(int valor, int maximo) {
    return valor > maximo ? maximo : valor;
}

static void mostrar_situacao(const Personagem *p, const Inimigo *i) {
    printf("\n");
    linha('-');
    printf("%s | HP %d/%d | Mana %d/%d | Stamina %d/%d | Pocoes %d\n",
           p->nome, p->hp, p->hp_max, p->mana, p->mana_max,
           p->stamina, p->stamina_max, p->pocoes);
    printf("%s | HP %d/%d\n", i->nome, i->hp > 0 ? i->hp : 0, i->hp_max);
    linha('-');
}

static int acao_atacar(Personagem *p, Inimigo *i) {
    ResultadoTeste r;
    int dano;

    if (p->stamina < CUSTO_ATAQUE) {
        printf("\nVoce esta sem stamina! Use Defender para recuperar o folego.\n");
        return 0;
    }
    p->stamina -= CUSTO_ATAQUE;
    printf("\nVoce ataca o %s com sua %s!\n", i->nome, p->arma);
    r = teste_habilidade(p, ATR_FORCA, i->defesa, "Ataque");
    if (r.sucesso) {
        dano = p->dano_arma + rolar_dado(5) + modificador(p->forca);
        if (r.critico) {
            dano *= 2;
            printf("GOLPE CRITICO!\n");
        }
        if (dano < 1) {
            dano = 1;
        }
        i->hp -= dano;
        printf("Voce causou %d de dano.\n", dano);
    } else {
        printf("Voce errou o golpe!\n");
    }
    return 1;
}

static int acao_magia(Personagem *p, Inimigo *i) {
    int dano;

    if (p->mana < CUSTO_MAGIA) {
        printf("\nVoce nao tem mana suficiente!\n");
        return 0;
    }
    p->mana -= CUSTO_MAGIA;
    dano = 20 + rolar_dado(6) + modificador(p->inteligencia) * 3;
    if (dano < 1) {
        dano = 1;
    }
    i->hp -= dano;
    printf("\nVoce lancou uma bola de fogo! Causou %d de dano.\n", dano);
    return 1;
}

static int acao_pocao(Personagem *p) {
    if (p->pocoes <= 0) {
        printf("\nVoce nao possui pocoes!\n");
        return 0;
    }
    p->pocoes--;
    p->hp = limitar(p->hp + 30, p->hp_max);
    printf("\nVoce tomou uma pocao e recuperou 30 HP.\n");
    return 1;
}

static int acao_fugir(Personagem *p, const Inimigo *i, int *fugiu) {
    ResultadoTeste r;

    printf("\nVoce tenta fugir!\n");
    r = teste_habilidade(p, ATR_DESTREZA, 10 + i->bonus_ataque, "Fuga");
    if (r.sucesso) {
        printf("Voce conseguiu fugir!\n");
        *fugiu = 1;
    } else {
        printf("O inimigo bloqueou sua fuga!\n");
    }
    return 1;
}

/* Retorna 1 se o jogador fugiu. */
static int turno_jogador(Personagem *p, Inimigo *i, int *defendendo) {
    int agiu = 0;
    int fugiu = 0;

    *defendendo = 0;
    p->stamina = limitar(p->stamina + 3, p->stamina_max);
    mostrar_situacao(p, i);

    while (!agiu) {
        printf("\nSeu turno! O que voce deseja fazer?\n");
        printf("1 - Atacar (%d stamina)\n", CUSTO_ATAQUE);
        printf("2 - Usar magia (%d mana)\n", CUSTO_MAGIA);
        printf("3 - Usar pocao\n");
        printf("4 - Defender (recupera stamina)\n");
        printf("5 - Fugir\n");

        switch (ler_inteiro("> ", 1, 5)) {
            case 1: agiu = acao_atacar(p, i); break;
            case 2: agiu = acao_magia(p, i); break;
            case 3: agiu = acao_pocao(p); break;
            case 4:
                *defendendo = 1;
                p->stamina = limitar(p->stamina + 15, p->stamina_max);
                printf("\nVoce assume postura defensiva (+15 stamina).\n");
                agiu = 1;
                break;
            default: agiu = acao_fugir(p, i, &fugiu); break;
        }
    }
    return fugiu;
}

static void turno_inimigo(Personagem *p, const Inimigo *i, int defendendo) {
    int alvo = 10 + modificador(p->destreza) + (defendendo ? 4 : 0);
    int rolagem = rolar_dado(20) + i->bonus_ataque;
    int dano;

    printf("\nTurno do %s! (ataque %d contra defesa %d)\n", i->nome, rolagem, alvo);
    if (rolagem >= alvo) {
        dano = i->dano_min + rand() % (i->dano_max - i->dano_min + 1);
        if (defendendo) {
            dano /= 2;
        }
        if (dano < 1) {
            dano = 1;
        }
        p->hp -= dano;
        printf("O %s acertou e voce perdeu %d HP.\n", i->nome, dano);
    } else {
        printf("O %s errou o ataque!\n", i->nome);
    }
    if (p->hp < 0) {
        p->hp = 0;
    }
}

int combate_executar(Personagem *p, Inimigo *i) {
    int iniciativa_j = rolar_dado(20) + modificador(p->destreza);
    int iniciativa_i = rolar_dado(20) + i->bonus_ataque;
    int vez_jogador = (iniciativa_j >= iniciativa_i);
    int defendendo = 0;

    printf("\n*** COMBATE: %s vs %s ***\n", p->nome, i->nome);
    printf("Iniciativa: voce %d x %d inimigo -> %s comeca.\n",
           iniciativa_j, iniciativa_i, vez_jogador ? "voce" : i->nome);

    while (p->hp > 0 && i->hp > 0) {
        if (vez_jogador) {
            if (turno_jogador(p, i, &defendendo)) {
                return COMBATE_FUGA;
            }
        } else if (i->hp > 0) {
            turno_inimigo(p, i, defendendo);
            defendendo = 0;
        }
        vez_jogador = !vez_jogador;
    }

    if (p->hp <= 0) {
        return COMBATE_DERROTA;
    }

    printf("\n============================\n");
    printf("   VOCE VENCEU O %s!\n", i->nome);
    printf("============================\n");
    p->dinheiro += i->ouro;
    printf("Voce ganhou %d moedas (total: %d).\n", i->ouro, p->dinheiro);
    if (rolar_dado(100) <= 30) {
        p->pocoes++;
        printf("O inimigo deixou cair uma pocao!\n");
    }
    jogador_ganhar_xp(p, i->xp);
    return COMBATE_VITORIA;
}
