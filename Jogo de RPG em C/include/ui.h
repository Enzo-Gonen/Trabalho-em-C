#ifndef UI_H
#define UI_H
void ui_limpar(void);
void ui_banner(const char *titulo, const char *subtitulo);
void ui_secao(const char *titulo);
void ui_barra(const char *rotulo, int atual, int maximo, int largura);
void ui_personagem(const char *nome, int nivel, int hp, int hp_max,
                  int mana, int mana_max, int stamina, int stamina_max);
void ui_moldura(const char *linha1, const char *linha2);
#endif
