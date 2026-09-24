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

Cada arquivo `.c`/`.h` tem um comentário `/* Autor: ... */` indicando o responsável.

**Importante para a nota individual (o professor avalia pelo histórico real de commits):**
cada integrante deve clonar o repositório com a própria conta do GitHub, revisar/ajustar
o(s) arquivo(s) sob sua responsabilidade (entender o código, comentar, corrigir o que
achar necessário) e dar o `git add` / `commit` / `push` **de dentro da própria conta**.
Não adianta só copiar os arquivos prontos — o grupo deve ler o código junto, porque a
apresentação é com arguição individual sobre qualquer parte do projeto.

## Conceitos exigidos no trabalho
- Entrada e saida: `printf`, `fgets`, `strtol` (leitura validada)
- Controle: `while`, `do-while`, `for`, `if`, `switch`
- Funcoes e modularizacao: 9 modulos .c com seus .h
- Homogeneas: vetores de `char`, `RUMORES[]`, vetores de ponteiros para atributos
- Heterogeneas: `Personagem`, `Inimigo`, `NPC`, `ResultadoTeste`, vetores de structs
- Arquivos: `savegame.dat` (wb/rb) e `historico.txt` (a/r), sempre com `fclose`
- Validacoes: erro ao abrir/criar arquivo, falha de `malloc`, save corrompido, busca sem resultado

## Mecanicas
- **Atributos:** Forca, Destreza, Inteligencia, Carisma (12 pontos para distribuir).
- **Teste:** d20 + modificador do atributo >= dificuldade. 20 natural sempre passa; 1 sempre falha.
- **Combate:** iniciativa (Destreza), depois turnos alternados: atacar, magia, pocao, defender, fugir.
- **Escolhas com consequencia:** reputacao, desconto do ferreiro, fraqueza do Orc, tres formas de enfrentar o chefe.
