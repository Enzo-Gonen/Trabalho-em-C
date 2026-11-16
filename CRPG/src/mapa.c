#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "mapa.h"
#include "ui.h"
#include "utils.h"
#include "combate.h"
#include "inimigo.h"
#include "arquivo.h"

#define ALTURA 9
#define LARGURA 15

static const char *MUNDO[ALTURA] = {
    "###############",
    "#...#.....#...#",
    "#T..#..M..#..C#",
    "#...###.###...#",
    "#..V....S....T#",
    "#...###.###...#",
    "#M..#.....#...#",
    "#...#..T..#...#",
    "###############"
};

static int coletado[ALTURA][LARGURA];
static int derrotado[ALTURA][LARGURA];
static int fonte_usada;

void mapa_reiniciar(void) {
    for (int y = 0; y < ALTURA; y++) {
        for (int x = 0; x < LARGURA; x++) {
            coletado[y][x] = 0;
            derrotado[y][x] = 0;
        }
    }
    fonte_usada = 0;
}

static void desenhar_mapa(int px, int py) {
    ui_banner("TERRAS DE ALVORADA", "Exploracao livre | W A S D para mover | Q para sair");
    puts("       NORTE ^");
    puts("      +---------------+");
    for (int y = 0; y < ALTURA; y++) {
        printf("      |");
        for (int x = 0; x < LARGURA; x++) {
            char c = MUNDO[y][x];
            if (x == px && y == py) c = '@';
            else if ((c == 'T' && coletado[y][x]) ||
                     (c == 'M' && derrotado[y][x])) c = '.';
            putchar(c);
        }
        puts("|");
    }
    puts("      +---------------+");
    puts("  @ Voce   # Rocha   . Trilha   T Bau   M Monstro");
    puts("  S Fonte   V Vila   C Caverna do Orc");
}

static int mover_valido(int x, int y) {
    return x >= 0 && x < LARGURA && y >= 0 && y < ALTURA &&
           MUNDO[y][x] != '#';
}

static int evento_celula(Personagem *p, int x, int y) {
    char local = MUNDO[y][x];
    Inimigo inimigo;
    int resultado;

    if (local == 'T' && !coletado[y][x]) {
        coletado[y][x] = 1;
        p->dinheiro += 20 + rolar_dado(15);
        printf("\n[TESOURO] Voce encontrou um bau com moedas. Saldo: %d.\n",
               p->dinheiro);
        registrar_evento(p, "Encontrou um bau nas Terras de Alvorada");
        pausar();
    } else if (local == 'M' && !derrotado[y][x]) {
        TipoInimigo tipo = (y == 2) ? INIMIGO_GOBLIN : INIMIGO_LOBO;
        inimigo = inimigo_criar(tipo);
        printf("\n[AMEACA] Um %s bloqueia sua passagem!\n", inimigo.nome);
        resultado = combate_executar(p, &inimigo);
        if (resultado == COMBATE_VITORIA) {
            derrotado[y][x] = 1;
            registrar_evento(p, "Derrotou uma criatura durante a exploracao");
        } else if (resultado == COMBATE_DERROTA) {
            return 0;
        } else {
            /* Ao fugir, retorna para a posicao anterior no loop principal. */
            return 2;
        }
        pausar();
    } else if (local == 'S' && !fonte_usada) {
        fonte_usada = 1;
        jogador_descansar(p);
        printf("\n[FONTE ANTIGA] A agua restaura HP, mana e stamina.\n");
        registrar_evento(p, "Encontrou a Fonte Antiga");
        pausar();
    } else if (local == 'V') {
        printf("\n[VILA] As muralhas de Alvorada estao logo ali. O caminho leva ao centro.\n");
        pausar();
    } else if (local == 'C') {
        printf("\n[CAVERNA] Um rugido ecoa la dentro. Grukh aguarda no covil.\n");
        printf("Volte ao centro da vila e escolha 'Enfrentar o Orc Grukh'.\n");
        pausar();
    }
    return 1;
}

void mapa_explorar(Personagem *p) {
    int x = 3, y = 4;
    int anterior_x, anterior_y;
    char comando[32];

    if (p == NULL || p->hp <= 0) return;
    for (;;) {
        desenhar_mapa(x, y);
        ui_secao("EXPLORACAO");
        printf("  Posicao: X=%d Y=%d | Moedas: %d | Nivel: %d\n",
               x, y, p->dinheiro, p->nivel);
        printf("  Movimento [W/A/S/D] | [Q] voltar: ");
        if (fgets(comando, sizeof(comando), stdin) == NULL) return;
        if (strchr(comando, '\n') == NULL && !feof(stdin)) limpar_buffer();
        char tecla = (char)tolower((unsigned char)comando[0]);
        if (tecla == 'q') return;

        anterior_x = x;
        anterior_y = y;
        if (tecla == 'w') y--;
        else if (tecla == 's') y++;
        else if (tecla == 'a') x--;
        else if (tecla == 'd') x++;
        else {
            printf("\nComando desconhecido. Use W, A, S, D ou Q.\n");
            pausar();
            continue;
        }

        if (!mover_valido(x, y)) {
            x = anterior_x;
            y = anterior_y;
            printf("\nUma parede de rocha bloqueia o caminho.\n");
            pausar();
            continue;
        }

        int evento = evento_celula(p, x, y);
        if (evento == 0) return;
        if (evento == 2) {
            x = anterior_x;
            y = anterior_y;
        }
        if (p->hp <= 0) return;
    }
}
