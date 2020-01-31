#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOMEBANCO "mosaico.bd"
// Definicoes, prototipos e cabecalhos

typedef struct {
	char tipo[2];
	int c;
	int l;
	int max;
} cabecalho;
typedef struct {
	char opcao[2];
	int id;
	char nome[30];
	char nomeimg[30];
	int m;
	int n;
	cabecalho head;
} linha;

void CHK_ARQ(int, int);
void LE_CABECALHO(FILE*, char*, char*, int*, int*, int*);
void REFRESH_BD(FILE*, int*);
void INSERE_BD(FILE*, int, char*, int, int); 
void REMOVE_BD(FILE*, int, int);
void MOVE_BD(FILE*, int, int, int);
void CRIA_IMG(FILE*, linha*, char*);
