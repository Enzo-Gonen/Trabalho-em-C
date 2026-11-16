/* Autor: Pedro Lucas */
#include <stdio.h>
#include "habilidades.h"
#include "utils.h"

const char *nome_atributo(Atributo atr) {
    static const char *nomes[] = {"Forca", "Destreza", "Inteligencia", "Carisma"};
    if (atr < ATR_FORCA || atr > ATR_CARISMA) return "Desconhecido";
    return nomes[atr];
}

static int valor_atributo(const Personagem *p, Atributo atr) {
    if (p == NULL) return 0;
    switch (atr) {
        case ATR_FORCA:        return p->forca;
        case ATR_DESTREZA:     return p->destreza;
        case ATR_INTELIGENCIA: return p->inteligencia;
        default:               return p->carisma;
    }
}

/* d20 + modificador do atributo contra uma dificuldade. 20 natural sempre passa, 1 sempre falha. */
ResultadoTeste teste_habilidade(const Personagem *p, Atributo atr,
                                int dificuldade, const char *descricao) {
    ResultadoTeste r;

    if (p == NULL) {
        ResultadoTeste invalido = {0, 0, 0, 0, 0};
        return invalido;
    }
    r.dado = rolar_dado(20);
    r.bonus = modificador(valor_atributo(p, atr));
    r.total = r.dado + r.bonus;
    r.critico = (r.dado == 20);
    r.sucesso = r.critico || (r.dado != 1 && r.total >= dificuldade);

    printf("[%s | %s] d20=%d %+d = %d (dif. %d) -> %s\n",
           descricao, nome_atributo(atr), r.dado, r.bonus, r.total, dificuldade,
           r.sucesso ? "SUCESSO" : "FALHA");
    return r;
}
