# A Lenda de Alvorada — RPG em C

**Versao 3.2 — manutencao, validacao e testes**

RPG de terminal em C11 com classes, combate por turnos, NPCs, progressao,
mapa ASCII e salvamento local.

## Novidades da 3.2
- Corrigido o menu da vila: a opcao 9 agora aparece e permite abrir o mapa.
- Validacao centralizada dos dados do personagem antes de salvar/carregar.
- Save v3 com checksum FNV-1a, deteccao de truncamento e rejeicao de dados extras.
- Leitura de saves v2 mantida para compatibilidade; novos saves sao gravados em v3.
- Substituicao de save existente corrigida para Windows (`MoveFileEx`) e POSIX (`rename`).
- Leitura numerica mais rigorosa, dados com limites e tratamento de entradas longas.
- Rolagem de dados protegida contra lados invalidos e com reducao de vies modulo.
- Poção não é consumida quando HP já está cheio.
- Cerveja não é cobrada quando stamina já está cheia.
- Dano de combate limitado ao HP restante; validacao de argumentos.
- Menos alocacao dinamica no menu principal.
- Testes automatizados e flags adicionais de compilacao.

## Windows: iniciar
1. Extraia o ZIP.
2. Execute `INICIAR.bat`.
3. Se GCC estiver ausente, o inicializador chama `INSTALAR_AMBIENTE.bat`, que tenta instalar MSYS2/GCC via `winget` e `pacman`.
4. O jogo compila em `build/rpg.exe` e inicia.

Requisitos da instalacao automatica: Windows 10/11, `winget`, conexao com internet e permissao para instalar pacotes. Se a atualizacao do MSYS2 pedir reinicio do terminal, execute `INSTALAR_AMBIENTE.bat` novamente.

## Compilar e testar

```sh
make
make test
./build/rpg
```

No Windows com MSYS2 UCRT64, execute os mesmos comandos dentro do terminal UCRT64.

## Estrutura
```text
├── include/       # Interfaces publicas (.h)
├── src/           # Implementacao (.c)
├── tests/         # Testes automatizados
├── build/         # Binarios gerados (ignorado pelo Git)
├── .vscode/       # Tarefas e configuracao do editor
├── INICIAR.bat
├── INSTALAR_AMBIENTE.bat
├── Makefile
└── README.md
```

## Persistencia
- `savegame.dat`: personagem salvo.
- `historico.txt`: eventos da aventura.
- O formato novo usa versao 3 e checksum; saves v2 continuam aceitos.
- O mapa e os eventos coletados ainda nao sao persistidos no save.

## Seguranca e privacidade
O jogo nao usa telemetria nem envia saves/historico para servidores. A instalacao do compilador e da extensao do VS Code depende de ferramentas oficiais externas (`winget`, MSYS2/pacman e VS Code Marketplace).
