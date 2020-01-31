#include "mosaico.h"

//Faz a leitura do cabecalho de uma imagem
void LE_CABECALHO(FILE* fimg, char* p, char* nomeimg, int* li, int* co, int* hi) {
	int i;
	char com[50];	//temp para verificacao de comentarios na imagem
	
	fgets(p,4,fimg); //verifica o tipo do arquivo - P5, etc.
	if ((p[1] == '2') || (p[1] == '3')) {
		fprintf(stderr,"LE_CABECALHO: Formato incorreto de imagem!");
		exit(1);
	}
	if ((com[0]=getc(fimg))=='#') { //verificacao de comentarios na imagem
		fgets(com,50,fimg);
		fscanf(fimg,"%d %d", co, li); //le as linhas e colunas da foto
	}
	else {
		i=1;
		while ((com[i]=getc(fimg))!=' ') i++;
		*co=atoi(com);
		fscanf(fimg, "%d", li); //le a area da figura
	}
	getc(fimg); 
	fscanf(fimg,"%d", hi); //le o valor maximo de brilho
}

// Valida arquivo
void CHK_ARQ(int arg, int n) {
	if (arg!=n) {			//valida o numero de argumentos
		fprintf(stderr,"CHK_ARQ: Erro na argumentacao!\n");
		exit(1);
	}
}

