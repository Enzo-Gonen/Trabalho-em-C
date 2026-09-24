/* Autor: Eloi Junior */
#ifndef JOGADOR_H
#define JOGADOR_H

#define TAM_NOME 50
#define TAM_ARMA 30

typedef struct {
    char nome[TAM_NOME];
    char arma[TAM_ARMA];
    int hp, hp_max;
    int mana, mana_max;
    int stamina, stamina_max;
    int forca, destreza, inteligencia, carisma;
    int nivel, xp;
    int dinheiro, pocoes, dano_arma;
    /* progresso e escolhas da historia */
    int reputacao;
    int sabe_fraqueza;
    int desconto;
    int barganhou;
    int ajuda_anciao;
    int chefe_derrotado;
} Personagem;

void jogador_criar(Personagem *p);
void jogador_status(const Personagem *p);
void jogador_ganhar_xp(Personagem *p, int xp);
void jogador_descansar(Personagem *p);
int  modificador(int atributo);

#endif
