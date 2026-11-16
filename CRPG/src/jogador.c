/* Autor: Eloi Junior */
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "jogador.h"
#include "utils.h"
#include "ui.h"

/* Modificador estilo RPG de mesa: 10-11 = 0, 12-13 = +1, 8-9 = -1... */
int modificador(int atributo) {
    if (atributo >= 10) {
        return (atributo - 10) / 2;
    }
    return -((11 - atributo) / 2);
}

/* Valida invariantes do personagem antes de salvar ou usar dados externos. */
int jogador_validar(const Personagem *p) {
    if (p == NULL) return 0;
    if (p->nome[0] == '\0' || memchr(p->nome, '\0', TAM_NOME) == NULL) return 0;
    if (p->arma[0] == '\0' || memchr(p->arma, '\0', TAM_ARMA) == NULL) return 0;
    if (p->classe < CLASSE_GUERREIRO || p->classe > CLASSE_LADINO) return 0;
    if (p->nivel < 1 || p->nivel > 100000 || p->xp < 0 || p->xp >= p->nivel * 50LL) return 0;
    if (p->hp_max < 1 || p->hp_max > 10000000 || p->hp < 1 || p->hp > p->hp_max) return 0;
    if (p->mana_max < 0 || p->mana_max > 10000000 || p->mana < 0 || p->mana > p->mana_max) return 0;
    if (p->stamina_max < 0 || p->stamina_max > 10000000 || p->stamina < 0 || p->stamina > p->stamina_max) return 0;
    if (p->forca < 0 || p->forca > 1000 || p->destreza < 0 || p->destreza > 1000 ||
        p->inteligencia < 0 || p->inteligencia > 1000 || p->carisma < 0 || p->carisma > 1000) return 0;
    if (p->dinheiro < 0 || p->dinheiro > INT_MAX / 2 || p->pocoes < 0 || p->pocoes > 1000000) return 0;
    if (p->dano_arma < 1 || p->dano_arma > 1000000) return 0;
    if (p->reputacao < -1000000 || p->reputacao > 1000000) return 0;
    if ((p->sabe_fraqueza & ~1) || (p->desconto & ~1) || (p->barganhou & ~1) ||
        (p->ajuda_anciao & ~1) || (p->chefe_derrotado & ~1)) return 0;
    return 1;
}

void jogador_criar(Personagem *p) {
    const char *nomes[4] = {"Forca", "Destreza", "Inteligencia", "Carisma"};
    int *atributos[4];
    int pontos = 12;

    if (p == NULL) return;
    memset(p, 0, sizeof(*p));
    ler_texto("Digite o nome do seu personagem: ", p->nome, TAM_NOME);

    atributos[0] = &p->forca;
    atributos[1] = &p->destreza;
    atributos[2] = &p->inteligencia;
    atributos[3] = &p->carisma;

    printf("\nTodo atributo comeca em 8. Distribua %d pontos.\n", pontos);
    for (int i = 0; i < 4; i++) {
        *atributos[i] = 8;
        if (i < 3) {
            char msg[80];
            printf("Pontos restantes: %d\n", pontos);
            snprintf(msg, sizeof(msg), "Pontos em %s: ", nomes[i]);
            int add = ler_inteiro(msg, 0, pontos);
            *atributos[i] += add;
            pontos -= add;
        } else {
            *atributos[i] += pontos;
            printf("Os %d pontos restantes vao para %s.\n", pontos, nomes[i]);
        }
    }

    printf("\nEscolha sua classe:\n1 - Guerreiro: +10 HP maximo e +3 dano fisico\n2 - Arcano: +15 mana e magia aprimorada\n3 - Ladino: +2 iniciativa e melhor chance de fuga\n");
    p->classe = (Classe)(ler_inteiro("> ", 1, 3) - 1);

    p->hp_max = 100 + modificador(p->forca) * 5 + (p->classe == CLASSE_GUERREIRO ? 10 : 0);
    p->mana_max = 20 + p->inteligencia * 2 + (p->classe == CLASSE_ARCANO ? 15 : 0);
    p->stamina_max = 30 + p->destreza * 2;
    p->hp = p->hp_max;
    p->mana = p->mana_max;
    p->stamina = p->stamina_max;

    strcpy(p->arma, "Espada");
    p->dano_arma = 15;
    p->dinheiro = 20;
    p->pocoes = 2;
    p->nivel = 1;
}

void jogador_status(const Personagem *p) {
    printf("\n");
    ui_secao("STATUS DO AVENTUREIRO");
    ui_personagem(p->nome, p->nivel, p->hp, p->hp_max,
                  p->mana, p->mana_max, p->stamina, p->stamina_max);
    printf("  XP: %d/%d\n", p->xp, p->nivel * 50);
    printf("FOR %d | DES %d | INT %d | CAR %d\n",
           p->forca, p->destreza, p->inteligencia, p->carisma);
    printf("Classe: %s\n", p->classe == CLASSE_GUERREIRO ? "Guerreiro" : p->classe == CLASSE_ARCANO ? "Arcano" : "Ladino");
    printf("Arma: %s (dano %d) | Pocoes: %d | Moedas: %d\n",
           p->arma, p->dano_arma, p->pocoes, p->dinheiro);
    printf("Reputacao na vila: %d\n", p->reputacao);
    puts("  +--------------------------------------------------------------+");
}

static void melhorar_atributo(Personagem *p) {
    printf("Escolha um atributo para +1:\n");
    printf("1 - Forca\n2 - Destreza\n3 - Inteligencia\n4 - Carisma\n");
    switch (ler_inteiro("> ", 1, 4)) {
        case 1: p->forca++; break;
        case 2: p->destreza++; break;
        case 3: p->inteligencia++; break;
        default: p->carisma++; break;
    }
}

void jogador_ganhar_xp(Personagem *p, int xp) {
    if (p == NULL || xp <= 0 || p->nivel < 1) return;
    if (xp > INT_MAX - p->xp) xp = INT_MAX - p->xp;
    printf("Voce ganhou %d XP.\n", xp);
    p->xp += xp;
    while (p->xp >= p->nivel * 50) {
        p->xp -= p->nivel * 50;
        p->nivel++;
        p->hp_max += 10;
        p->mana_max += 5;
        p->stamina_max += 5;
        p->hp = p->hp_max;
        p->mana = p->mana_max;
        p->stamina = p->stamina_max;
        printf("\n*** SUBIU DE NIVEL! Agora voce e nivel %d ***\n", p->nivel);
        melhorar_atributo(p);
    }
}

void jogador_descansar(Personagem *p) {
    if (p == NULL) return;
    p->hp = p->hp_max;
    p->mana = p->mana_max;
    p->stamina = p->stamina_max;
}
