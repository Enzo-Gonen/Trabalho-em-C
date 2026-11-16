/* Autor: Eloi Junior */
#include <stdio.h>
#include <string.h>
#include "jogador.h"
#include "utils.h"

/* Modificador estilo RPG de mesa: 10-11 = 0, 12-13 = +1, 8-9 = -1... */
int modificador(int atributo) {
    if (atributo >= 10) {
        return (atributo - 10) / 2;
    }
    return -((11 - atributo) / 2);
}

void jogador_criar(Personagem *p) {
    const char *nomes[4] = {"Forca", "Destreza", "Inteligencia", "Carisma"};
    int *atributos[4];
    int pontos = 12;

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

    p->hp_max = 100 + modificador(p->forca) * 5;
    p->mana_max = 20 + p->inteligencia * 2;
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
    linha('=');
    printf("%s  (Nivel %d, XP %d/%d)\n", p->nome, p->nivel, p->xp, p->nivel * 50);
    linha('-');
    printf("HP: %d/%d | Mana: %d/%d | Stamina: %d/%d\n",
           p->hp, p->hp_max, p->mana, p->mana_max, p->stamina, p->stamina_max);
    printf("FOR %d | DES %d | INT %d | CAR %d\n",
           p->forca, p->destreza, p->inteligencia, p->carisma);
    printf("Arma: %s (dano %d) | Pocoes: %d | Moedas: %d\n",
           p->arma, p->dano_arma, p->pocoes, p->dinheiro);
    printf("Reputacao na vila: %d\n", p->reputacao);
    linha('=');
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
    p->hp = p->hp_max;
    p->mana = p->mana_max;
    p->stamina = p->stamina_max;
}
