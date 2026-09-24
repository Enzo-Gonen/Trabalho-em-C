/* Autor: Pedro Lucas */
#ifndef COMBATE_H
#define COMBATE_H

#include "jogador.h"
#include "inimigo.h"

enum { COMBATE_VITORIA, COMBATE_DERROTA, COMBATE_FUGA };

int combate_executar(Personagem *p, Inimigo *inimigo);

#endif
