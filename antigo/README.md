# RPG - Aventura em C

Jogo de RPG em texto com combate por turnos, testes de habilidade, NPCs e escolhas do jogador.

## Como compilar e rodar
```
make
./rpg
```

## Estrutura do projeto
| Arquivo | Responsabilidade |
|---|---|
| `main.c` | Menu principal, alocacao do personagem (malloc/free) |
| `utils.c/.h` | Entrada validada, dado, pausa |
| `jogador.c/.h` | Struct `Personagem`, criacao, status, XP e nivel |
| `habilidades.c/.h` | Testes de habilidade (d20 + modificador vs dificuldade) |
| `inimigo.c/.h` | Vetor de structs `Inimigo` (catalogo) |
| `combate.c/.h` | Combate por turnos com iniciativa |
| `npc.c/.h` | Vetor de structs `NPC` e dialogos com escolhas |
| `historia.c/.h` | Prologo, hub da vila, eventos e chefe final |
| `arquivo.c/.h` | Save/load binario, historico em texto, busca |

## Divisão do grupo

| Integrante | Arquivos |
|---|---|
| Pedro Lucas | `combate.c/.h`, `habilidades.c/.h`, `inimigo.c/.h` |
| Lyvia Vitoria | `historia.c/.h`, `npc.c/.h` |
| Eloi Junior | `jogador.c/.h`, `utils.c/.h` |
| Enzo Goncalves | `arquivo.c/.h`, `main.c`, `Makefile`, `README.md` |

## Mecanicas
- **Atributos:** Forca, Destreza, Inteligencia, Carisma (12 pontos para distribuir).
- **Teste:** d20 + modificador do atributo >= dificuldade. 20 natural sempre passa; 1 sempre falha.
- **Combate:** iniciativa (Destreza), depois turnos alternados: atacar, magia, pocao, defender, fugir.
- **Escolhas com consequencia:** reputacao, desconto do ferreiro, fraqueza do Orc, tres formas de enfrentar o chefe.
