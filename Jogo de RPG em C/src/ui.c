#include <stdio.h>
#include "ui.h"
#define RESET "\033[0m"
#define CIANO "\033[36m"
#define MAGENTA "\033[35m"
#define AMARELO "\033[33m"
#define VERDE "\033[32m"
#define CINZA "\033[90m"

void ui_limpar(void) {
    printf("\033[2J\033[H");
}
void ui_banner(const char *titulo, const char *subtitulo) {
    ui_limpar();
    printf(CIANO);
    puts("  /====================================================================\\");
    puts("  |                                                                    |");
    puts("  |       ____  ____   ____       _    ____  _____ _   _ _____         |");
    puts("  |      |  _ \\/ ___| / ___|     / \\  / ___|| ____| \\ | |_   _|        |");
    puts("  |      | |_) \\___ \\| |  _     / _ \\ \\___ \\|  _| |  \\| | | |          |");
    puts("  |      |  _ < ___) | |_| |   / ___ \\ ___) | |___| |\\  | | |          |");
    puts("  |      |_| \\_\\____/ \\____|  /_/   \\_\\____/|_____|_| \\_| |_|          |");
    puts("  |                                                                    |");
    puts("  \\====================================================================/");
    printf(RESET MAGENTA "                         %s\n" RESET, titulo);
    if (subtitulo && subtitulo[0]) printf(CINZA "                    %s\n" RESET, subtitulo);
    puts("");
}
void ui_secao(const char *titulo) {
    printf("\n" AMARELO "  +--------------------[ %s ]" RESET "\n", titulo);
    puts("  |");
}
void ui_barra(const char *rotulo, int atual, int maximo, int largura) {
    int preenchido = 0;
    if (maximo > 0 && atual > 0) preenchido = (atual * largura) / maximo;
    if (preenchido > largura) preenchido = largura;
    if (preenchido < 0) preenchido = 0;
    printf("  %-8s [" VERDE, rotulo);
    for (int i = 0; i < largura; i++) printf("%s", i < preenchido ? "#" : "-");
    printf(RESET "] %d/%d\n", atual, maximo);
}
void ui_personagem(const char *nome, int nivel, int hp, int hp_max,
                  int mana, int mana_max, int stamina, int stamina_max) {
    puts(CIANO "  .--------------------------------------------------------------." RESET);
    printf(CIANO "  |" RESET " %-28s " AMARELO "NIVEL %-3d" CIANO "                     |\n" RESET, nome, nivel);
    ui_barra("HP", hp, hp_max, 20);
    ui_barra("MANA", mana, mana_max, 20);
    ui_barra("FOLEGO", stamina, stamina_max, 20);
    puts(CIANO "  '--------------------------------------------------------------'" RESET);
}
void ui_moldura(const char *linha1, const char *linha2) {
    puts(MAGENTA "  +--------------------------------------------------------------+" RESET);
    printf(MAGENTA "  |" RESET " %-60s" MAGENTA "|\n" RESET, linha1 ? linha1 : "");
    if (linha2) printf(MAGENTA "  |" RESET " %-60s" MAGENTA "|\n" RESET, linha2);
    puts(MAGENTA "  +--------------------------------------------------------------+" RESET);
}
