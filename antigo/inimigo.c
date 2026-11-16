/* Autor: Pedro Lucas */
#include "inimigo.h"

/* nome, hp, hp_max, dano_min, dano_max, defesa, bonus_ataque, xp, ouro */
static const Inimigo CATALOGO[TOTAL_INIMIGOS] = {
    {"Goblin",     40, 40,  5, 10, 10, 2,  30,  10},
    {"Lobo",       35, 35,  6, 11, 11, 3,  35,   5},
    {"Esqueleto",  50, 50,  6, 12, 12, 2,  45,  15},
    {"Orc Grukh",  90, 90,  8, 16, 13, 4, 150, 100}
};

Inimigo inimigo_criar(TipoInimigo tipo) {
    if (tipo < 0 || tipo >= TOTAL_INIMIGOS) {
        tipo = INIMIGO_GOBLIN;
    }
    return CATALOGO[tipo];
}
