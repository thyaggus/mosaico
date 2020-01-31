#include "mosaico.h"

//Programa principal
int main (int argc, char* argv[]) {

	FILE* fp_BD;
	linha info;

	if ((fp_BD=fopen(NOMEBANCO, "r+t")) == NULL) {	
		fp_BD=fopen(NOMEBANCO, "w");
		fclose(fp_BD);
		fp_BD=fopen(NOMEBANCO, "r+t");	//abre um arquivo de banco de imagens, e cria se nao existir
	}
	
	strcpy(info.opcao,argv[1]);
	switch (info.opcao[1]) {
		
		case 'a': 						//chama procedimento de insercao
			CHK_ARQ(argc,6);
			info.id=atoi(argv[2]);
			strcpy(info.nome,argv[3]);
			info.n=atoi(argv[4]);
			info.m=atoi(argv[5]);
			INSERE_BD(fp_BD,info.id, info.nome, info.n, info.m);
			break;
			
		case 'r':						//chama procedimento de remocao
			CHK_ARQ(argc,3);
			info.id=atoi(argv[2]);
			REMOVE_BD(fp_BD, info.id, 0);
			break;
			
		case 'm':						//chama procedimento para mover uma imagem
			CHK_ARQ(argc,5);
			info.id=atoi(argv[2]);
			info.n=atoi(argv[3]);
			info.m=atoi(argv[4]);
			MOVE_BD(fp_BD, info.id, info.n, info.m);
			break;
			
		case 'p':						//chama procedimento de impressao
			CHK_ARQ(argc,3);
			strcpy(info.nomeimg,argv[2]);
			CRIA_IMG(fp_BD,&info,info.nomeimg);
			break;
			
		default: fprintf(stderr,"Main: Argumentacao Invalida!\n");
			break;
	}
	fclose(fp_BD);
}
