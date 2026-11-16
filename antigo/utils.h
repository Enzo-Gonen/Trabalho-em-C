/* Autor: Eloi Junior */
#ifndef UTILS_H
#define UTILS_H

void limpar_buffer(void);
int  ler_inteiro(const char *msg, int min, int max);
void ler_texto(const char *msg, char *destino, int tamanho);
int  rolar_dado(int lados);
void linha(char c);
void pausar(void);

#endif
