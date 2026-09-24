/* Autor: Pedro Lucas */
#ifndef INIMIGO_H
#define INIMIGO_H

typedef enum {
    INIMIGO_GOBLIN,
    INIMIGO_LOBO,
    INIMIGO_ESQUELETO,
    INIMIGO_ORC,
    TOTAL_INIMIGOS
} TipoInimigo;

typedef struct {
    char nome[30];
    int hp, hp_max;
    int dano_min, dano_max;
    int defesa;
    int bonus_ataque;
    int xp;
    int ouro;
} Inimigo;

Inimigo inimigo_criar(TipoInimigo tipo);

#endif
