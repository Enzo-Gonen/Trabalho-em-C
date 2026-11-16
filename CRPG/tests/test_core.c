#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jogador.h"
#include "inimigo.h"
#include "habilidades.h"
#include "utils.h"
#include "arquivo.h"

static Personagem personagem_valido(void) {
    Personagem p;
    memset(&p, 0, sizeof(p));
    strcpy(p.nome, "Teste");
    strcpy(p.arma, "Espada");
    p.classe = CLASSE_GUERREIRO;
    p.hp = p.hp_max = 100;
    p.mana = p.mana_max = 30;
    p.stamina = p.stamina_max = 40;
    p.forca = p.destreza = p.inteligencia = p.carisma = 8;
    p.nivel = 1;
    p.dinheiro = 20;
    p.pocoes = 2;
    p.dano_arma = 15;
    return p;
}

int main(void) {
    Personagem p = personagem_valido();
    Inimigo inimigo;

    assert(modificador(8) == -1);
    assert(modificador(10) == 0);
    assert(modificador(12) == 1);
    assert(modificador(14) == 2);
    assert(jogador_validar(&p));
    p.hp = p.hp_max + 1;
    assert(!jogador_validar(&p));
    p = personagem_valido();
    p.classe = (Classe)99;
    assert(!jogador_validar(&p));
    assert(strcmp(nome_atributo((Atributo)99), "Desconhecido") == 0);
    assert(rolar_dado(0) == 0);
    srand(12345);
    for (int i = 0; i < 10000; ++i) {
        int d = rolar_dado(20);
        assert(d >= 1 && d <= 20);
    }
    inimigo = inimigo_criar(INIMIGO_ORC);
    assert(inimigo.hp > 0 && inimigo.hp == inimigo.hp_max);
    inimigo = inimigo_criar((TipoInimigo)999);
    assert(strcmp(inimigo.nome, "Goblin") == 0);

    /* Persistencia v3: gravar, sobrescrever, carregar e detectar corrupcao. */
    p = personagem_valido();
    remove(ARQUIVO_SAVE);
    assert(salvar_jogo(&p));
    p.dinheiro = 77;
    assert(salvar_jogo(&p));
    {
        Personagem carregado;
        memset(&carregado, 0, sizeof(carregado));
        assert(carregar_jogo(&carregado));
        assert(carregado.dinheiro == 77);
    }
    {
        FILE *f = fopen(ARQUIVO_SAVE, "r+b");
        int byte;
        assert(f != NULL);
        assert(fseek(f, 12L, SEEK_SET) == 0);
        byte = fgetc(f);
        assert(byte != EOF);
        assert(fseek(f, 12L, SEEK_SET) == 0);
        assert(fputc(byte ^ 0x01, f) != EOF);
        fclose(f);
        {
            Personagem carregado;
            assert(!carregar_jogo(&carregado));
        }
    }
    /* Compatibilidade com o cabecalho v2 original. */
    {
        FILE *f = fopen(ARQUIVO_SAVE, "wb");
        const char magic[8] = {'R','P','G','C','S','A','V','E'};
        unsigned version = 2u;
        Personagem legado = personagem_valido();
        assert(f != NULL);
        assert(fwrite(magic, sizeof(magic), 1, f) == 1);
        assert(fwrite(&version, sizeof(version), 1, f) == 1);
        assert(fwrite(&legado, sizeof(legado), 1, f) == 1);
        fclose(f);
        {
            Personagem carregado;
            assert(carregar_jogo(&carregado));
            assert(strcmp(carregado.nome, "Teste") == 0);
        }
    }
    remove(ARQUIVO_SAVE);
    remove(ARQUIVO_SAVE ".tmp");

    puts("Todos os testes principais passaram.");
    return 0;
}
