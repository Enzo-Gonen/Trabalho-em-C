# Relatorio de auditoria tecnica — A Lenda de Alvorada 3.2

## Escopo
Revisao estatico-funcional da base 3.1, com foco em fluxo do jogo, integridade de dados,
entrada do usuario, combate, compilacao e instalacao no Windows.

## Defeitos identificados e corrigidos

| Area | Problema na 3.1 | Correcao na 3.2 |
|---|---|---|
| Navegacao | Opcao 9 do mapa nao aparecia no menu e o limite de entrada era 8. | Menu agora apresenta opcao 9 e aceita intervalo 0-9. |
| Save no Windows | `rename()` nao substitui de forma portavel um destino existente no Windows. | `MoveFileExA` com substituicao e write-through no Windows; `rename()` atomico em POSIX. |
| Integridade do save | Save sem checksum e verificacao limitada. | Novo formato v3 com checksum FNV-1a, validacao dos campos, truncamento e bytes extras. |
| Compatibilidade | Atualizacao de formato poderia bloquear saves existentes. | Leitor continua aceitando formato v2; novos saves sao escritos em v3. |
| Validacao | Varios atributos, flags, limites e strings nao eram verificados. | Nova funcao `jogador_validar()` usada antes de salvar e depois de carregar. |
| Entrada numerica | Tratamento incompleto de overflow, espacos e linhas longas. | `strtol` com `errno`, verificacao de intervalo e descarte de entradas longas. |
| Entrada textual | Texto longo podia ser aceito truncado. | Linhas longas agora sao rejeitadas e solicitadas novamente. |
| RNG | `rolar_dado(0)` causava operacao invalida e `%` tinha pequeno vies. | Validacao de lados e amostragem por rejeicao. |
| Combate | Pocao era consumida com HP cheio; dano podia deixar HP negativo. | Pocao preservada quando HP esta cheio; HP do inimigo limitado a zero. |
| Taverna | Cerveja era cobrada mesmo com stamina cheia. | Compra bloqueada sem cobrar moedas quando nao ha recuperacao possivel. |
| Morte no mapa | Derrota em encontro do mapa nao mostrava a tela de derrota central. | Mensagem de game over exibida ao sair da vila com HP zerado. |
| Memoria | Alocacao dinamica desnecessaria para um unico personagem. | Personagem principal agora tem armazenamento automatico na pilha. |
| Enum | Nome de atributo podia indexar array fora dos limites. | Enum invalido retorna `Desconhecido`; testes de habilidade tratam ponteiro nulo. |

## Melhorias de manutencao
- Flags adicionais: `-Wconversion` e `-Wshadow`.
- Testes automatizados em `tests/test_core.c`.
- Alvo `make test`.
- Tarefas do VS Code para compilar e testar.
- Inicializador Windows enumera os arquivos `.c`, compila e so inicia apos compilacao bem-sucedida.
- Instalador verifica atualizacoes do MSYS2 e falhas do `pacman`.

## Testes executados

| Teste | Resultado |
|---|---|
| GCC C11, `-Wall -Wextra -Wpedantic -Wconversion -Wshadow` | Aprovado, sem warnings |
| Build incremental via Makefile | Aprovado |
| Testes de modificadores de atributos | Aprovado |
| Validacao de personagem e classe invalida | Aprovado |
| Rolagem de 10.000 dados d20 dentro dos limites | Aprovado |
| Fabrica de inimigos e enum invalido | Aprovado |
| Save v3, sobrescrita, load e checksum corrompido | Aprovado |
| Leitura de save legado v2 | Aprovado |
| Smoke test do menu e encerramento | Aprovado |
| AddressSanitizer + UndefinedBehaviorSanitizer | Aprovado, sem diagnosticos |
| Clang com warnings rigorosos | Aprovado, sem warnings |

## Riscos e limitacoes remanescentes

1. **Persistencia do mapa:** posicao, baus, monstros derrotados e fonte usada ainda nao sao gravados no save. Ao carregar, o estado do mapa reinicia.
2. **Formato binario:** o payload continua gravando a struct `Personagem` diretamente. O checksum detecta corrupcao acidental, mas nao torna o formato portavel entre arquiteturas, compiladores ou layouts diferentes. Uma futura versao deve serializar campo a campo com tipos de largura fixa.
3. **Checksum:** FNV-1a e uma verificacao de integridade contra corrupcao acidental, nao uma assinatura criptografica nem protecao contra adulteracao intencional.
4. **Instalador Windows:** scripts foram revisados estaticamente, mas a instalacao real de MSYS2/winget nao pode ser confirmada neste ambiente Linux. A primeira instalacao exige internet, `winget` e permissao do sistema.
5. **Aleatoriedade:** `rand()` e adequado para mecanicas simples de jogo, nao para seguranca criptografica.
6. **Testes de interface:** o fluxo de entrada principal foi smoke-tested; nao existe ainda uma suite completa de testes automatizados para todas as escolhas narrativas e batalhas.

## Parecer geral
A base 3.2 esta mais segura contra saves invalidos, entradas malformadas e falhas de compilacao. O fluxo de exploracao ficou acessivel pelo menu e a manutencao ganhou uma suite minima de regressao. A arquitetura modular de 3.1 foi preservada.

**Proxima prioridade tecnica recomendada:** separar o modelo de save da struct em memoria e persistir tambem o estado do mapa. Isso elimina a principal limitacao estrutural restante antes de adicionar novas regioes e campanhas.
