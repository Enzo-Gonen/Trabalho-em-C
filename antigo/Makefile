CC = gcc
CFLAGS = -Wall -Wextra -std=c11
SRC = main.c utils.c jogador.c habilidades.c inimigo.c combate.c npc.c historia.c arquivo.c

rpg: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o rpg

clean:
	rm -f rpg savegame.dat historico.txt
