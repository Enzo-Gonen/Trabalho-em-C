# A Lenda de Alvorada — RPG em C

**Versao 3.1 | Estrutura reorganizada e inicializacao automatizada**

RPG de terminal em C11 com classes, combate por turnos, NPCs, progressao,
eventos narrativos, save local e exploracao em mapa ASCII.

## Inicializacao rapida — Windows

1. Extraia o ZIP para uma pasta.
2. Execute `INICIAR.bat`.
3. Na primeira inicializacao, se o GCC estiver ausente, o script tenta instalar
   o MSYS2 pelo `winget`, atualizar os pacotes e instalar o GCC UCRT64.
4. O jogo e compilado e iniciado automaticamente.

Para preparar o ambiente separadamente, execute `INSTALAR_AMBIENTE.bat`.

**Requisitos do instalador automatico:** Windows 10/11 atualizado, `winget`
(App Installer) e conexao com a internet. A instalacao do compilador pode
solicitar permissao do Windows. Se o `winget` nao estiver disponivel, instale
MSYS2 manualmente em https://www.msys2.org/ e execute o instalador novamente.

## Dependencias

- GCC (C11) — necessario para compilar.
- MSYS2 UCRT64 — instalado automaticamente pelo script quando possivel.
- VS Code — opcional.
- Extensao C/C++ da Microsoft — opcional; o instalador tenta instala-la
  automaticamente quando o comando `code` esta disponivel.
- Make — opcional para compilar via `make`.

O jogo nao usa bibliotecas C externas. Nao e necessario baixar pacotes
adicionais para executar a logica do RPG.

## Compilar manualmente

### Windows (MSYS2 UCRT64)
```sh
gcc -std=c11 -Wall -Wextra -Wpedantic -O2 -Iinclude src/*.c -o build/rpg.exe
./build/rpg.exe
```

### Linux / WSL
```sh
make
./build/rpg
```

## Estrutura do projeto

```text
Jogo de RPG em C/
├── .vscode/              # Configuracoes e extensoes recomendadas
├── build/                # Executaveis e objetos gerados
├── include/              # Arquivos de cabecalho (.h)
├── src/                  # Implementacao dos modulos (.c)
├── .gitignore
├── INICIAR.bat           # Compila e inicia o jogo
├── INSTALAR_AMBIENTE.bat # Prepara GCC/MSYS2 e extensao opcional
├── Makefile
└── README.md
```

### Modulos
| Modulo | Responsabilidade |
|---|---|
| `main.c` | Menu e ciclo principal |
| `jogador.c` | Personagem, classes, atributos e progressao |
| `combate.c` | Combate por turnos |
| `habilidades.c` | Testes de habilidade d20 |
| `inimigo.c` | Catalogo de inimigos |
| `npc.c` | Dialogos, comercio e rumores |
| `historia.c` | Prologo, vila, eventos e chefe |
| `mapa.c` | Mapa ASCII e movimentacao WASD |
| `arquivo.c` | Save/load e historico |
| `ui.c` | Interface ASCII e barras de status |
| `utils.c` | Entrada validada e utilitarios |

## Dados locais

O jogo grava `savegame.dat` e `historico.txt` na pasta de execucao.
Esses arquivos sao locais; o jogo nao envia dados para servidores.

## Observacoes

- O mapa e os eventos de exploracao ainda sao reiniciados ao iniciar uma nova
  campanha e nao sao persistidos no save.
- Saves de versoes anteriores podem nao ser compativeis.
- Os codigos-fonte originais e seus comentarios de autoria foram preservados.
