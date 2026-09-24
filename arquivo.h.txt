/* Autor: Enzo Goncalves */
#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "jogador.h"

#define ARQUIVO_SAVE      "savegame.dat"
#define ARQUIVO_HISTORICO "historico.txt"

int  salvar_jogo(const Personagem *p);
int  carregar_jogo(Personagem *p);
void registrar_evento(const Personagem *p, const char *evento);
void exibir_historico(void);
void buscar_historico(const char *termo);

#endif
