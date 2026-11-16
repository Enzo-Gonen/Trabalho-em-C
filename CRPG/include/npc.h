/* Autor: Lyvia Vitoria */
#ifndef NPC_H
#define NPC_H

#include "jogador.h"

typedef enum {
    NPC_ANCIAO,
    NPC_FERREIRO,
    NPC_TAVERNEIRA,
    TOTAL_NPCS
} IdNpc;

typedef struct {
    char nome[30];
    char titulo[40];
    char saudacao[120];
} NPC;

void npc_conversar(Personagem *p, IdNpc id);

#endif
