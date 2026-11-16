/* Autor: Pedro Lucas */
#ifndef HABILIDADES_H
#define HABILIDADES_H

#include "jogador.h"

typedef enum {
    ATR_FORCA,
    ATR_DESTREZA,
    ATR_INTELIGENCIA,
    ATR_CARISMA
} Atributo;

typedef struct {
    int dado;
    int bonus;
    int total;
    int sucesso;
    int critico;
} ResultadoTeste;

const char *nome_atributo(Atributo atr);
ResultadoTeste teste_habilidade(const Personagem *p, Atributo atr,
                                int dificuldade, const char *descricao);

#endif
