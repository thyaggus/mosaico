#include "mosaico.h"

//Marca uma imagem como inativa no banco
void REMOVE_BD(FILE* freg, int id, int n) {
	char vid[5], ch;
	int vvid;
	int i,c=0;

	while (ch != EOF) {			//percorre banco de imagens
		ch=fgetc(freg);
		if (ch=='[') {
			for (i=0;ch!=']';i++) {
				ch=fgetc(freg);
				vid[i]=ch;
			}
			if ((vvid=atoi(vid)) == id) {	//procura se id ja existe
				c++;
				fputc('*', freg);
			}
			for (i=0;i<5;i++)
				vid[i]='\0';
		}
	}
	if (c==0) //valida id da imagem
		fprintf(stderr,"ID %d inexistente no banco de imagens!\n", id);
}

//Adiciona uma imagem no banco
void INSERE_BD(FILE* freg, int id, char* nome, int n, int m) {
	
	FILE* ftest;
	
	if ((ftest=fopen(nome,"r"))==NULL) { //testa se o arquivo da imagem existe
		fprintf(stderr,"Erro ao abrir imagem %s!\n",nome);
		exit(1);
	}
	
	REMOVE_BD(freg, id, 2);	//checa se id ja existe
	fprintf(freg,"[%d]  %s %d %d\n", id,nome,n,m); //modifica o arquivo com os novos dados da imagem
}

//movimenta imagem no banco de dados de imagens verificando se o ID da imagem ja existe e atualizando o banco de imagens
void MOVE_BD(FILE* freg, int id, int n, int m)
{
	char st1[5], st2[30];
	int naux, maux, vid;
	int i,c;
	
	while (!feof(freg)) {
		c=0;
		fgetc(freg);
		fscanf(freg, "%s %s %d %d", st1, st2, &naux, &maux);
		fgetc(freg);
		vid=atoi(st1);
		for (i=0;i<5;i++)
			if (st1[i]=='*') c=1;
		if (vid==id) {
			if (c!=1) {
				fclose(freg);
				if((freg=fopen(NOMEBANCO, "r+t")) == NULL) {
					fprintf(stderr,"MOVE_BD: ERRO na leitura do BANCO de DADOS!");
					exit(1);
				}
				REMOVE_BD(freg, id, 0);
				while (!feof(freg)) getc(freg);
				fprintf(freg, "[%s  %s %d %d\n", st1, st2, n, m);
			}
		}
	}
}

//Ordena arquivo de banco de imagens por id, atualiza informacoes, e limpa informacao desnecessaria.
void REFRESH_BD(FILE* freg, int* nimgss) {

	char buf[250], **str;
	int i,j,k; 
	int *vid, id;
	int aux, nimgs=0;
	
	str=(char**) malloc(sizeof(char*));	//matriz para leitura de imagem
	str[nimgs]=(char*) malloc(sizeof(char));
	
	vid=(int*)malloc(sizeof(int));	//vetor de id`s de imagens
	while (!feof(freg)) {
		fgetc(freg);
		fscanf(freg,"%d", &id);
		fgets(buf,240,freg); 
		if (buf[1]!='*') {
			str=(char**)realloc(str,(nimgs+1)*sizeof(char*));
			str[nimgs]=(char*)realloc(str[nimgs],250*sizeof(char));
			vid=(int*)realloc(vid,(nimgs+1)*sizeof(int));
			strcpy(str[nimgs],buf);
			vid[nimgs]=id;
			nimgs++;
		}
	}
	nimgs--; //Patch - Remove lixo
	k=nimgs-1;
	
	//Ordenacao das imagens por ID`s
	for(i=0; i<nimgs; i++) {
		for(j=0; j<k; j++) {
			if(vid[j]>vid[j+1]) {
				//swap
				aux=vid[j];
				vid[j]=vid[j+1];
				vid[j+1]=aux;
				//swap
				strcpy(buf,str[j]);	
				strcpy(str[j],str[j+1]);
				strcpy(str[j+1],buf);
			}
		}
	}
	//atualiza arquivo banco
	if((freg=fopen(NOMEBANCO,"wt"))==NULL) {
		fprintf(stderr,"Erro ao abrir arquivo de registro %s!", NOMEBANCO);
		exit(1);
	}
	for (i=0; i<nimgs;i++)
		fprintf(freg,"[%d%s", vid[i], str[i]);
	*nimgss=nimgs;
	fclose(freg);	//fecha arquivo
}

//Cria um mosaico a partir de um banco de dados de imagem.
void CRIA_IMG (FILE* freg, linha* est, char* imagem) {

	FILE *mosaico, *img;
	char** mat;
	char str[5];
	int c=0, l=0, mx=0;
	int cof=0,lif=0;
	int i,j,k,p=5;
	int nimgs, mediamx=0;
	int tambyte=1;


	//Atualiza arquivo banco de imagens
	REFRESH_BD(freg,&nimgs);

	if (((mosaico=fopen(imagem, "w+"))==NULL) || ((freg=fopen(NOMEBANCO,"r"))==NULL)) { //leitura do arquivo banco de imagens
		fprintf(stderr,"Erro ao criar imagem!\n");
		exit(1);
	}

	//calculo de posicao, e acumulo do valor maximo de brilho das imagens
	for (i=0;i<nimgs;i++) {
		fscanf(freg, "%s %s %d %d", str, est->nome, &est->n, &est->m);
		fgetc(freg);
		if ((img=fopen(est->nome, "r"))==NULL) {
			fprintf(stderr,"Erro na abertura de imagem!\n");
			exit(1);
		}
		LE_CABECALHO(img, est->head.tipo, est->nome, &est->head.l, &est->head.c, &est->head.max);
		if ((est->n+est->head.c)>c)
			c=est->n+est->head.c;
		if ((est->m+est->head.l)>l)
			l=est->m+est->head.l;
		if (est->head.max > mx);
			mx=est->head.max;
		if (est->head.max>255) //verifica se a imagem tem 1 ou 2 bytes
			tambyte=2;
		mediamx+=mx;
		if (est->head.tipo[1]=='6') p=6; //verifica se a imagem eh ppm (rbg)
	}
	rewind(freg); //robobina o arquivo banco de imagens

	mat=(char**)malloc(l*sizeof(char*));	//aloca matriz do mosaico de figuras
	for (i=0;i<l;i++)
		mat[i]=(char*)malloc(c*sizeof(char));

	for (i=0;i<l;i++)
		for (j=0;j<c;j++)
			mat[i][j]=mediamx/nimgs;		//preenche o fundo do mosaico com a media dos valores maximos de brilho das figuras
	
	for (k=0;k<nimgs;k++) {		//faz releitura do arquivo de banco de imagens para inserir no mosaico
		fgetc(freg);
		fscanf(freg, "%s %s %d %d", str, est->nome, &est->n, &est->m);
		if ((img=fopen(est->nome, "r"))==NULL) {
			fprintf(stderr,"Erro na abertura de imagem!\n");
			exit(1);
		}
		fgetc(freg);
		est->id=atoi(str);
		LE_CABECALHO(img, est->head.tipo, est->nome, &est->head.l, &est->head.c, &est->head.max);

		if ((lif=est->head.l+est->m)>lif)
			lif=est->head.l+est->m;				//verifica posicao da imagem no mosaico (colunas)
		if ((cof=est->head.c+est->n)>cof)
			cof=est->head.c+est->n;				//verifica posicao da imagem no mosaico (linhas)

		for (i=est->m;i<lif;i++)		//armazena conteudo da imagem em uma matriz
			for (j=est->n;j<cof;j++)
				if ((j < c) && (i < l))
					fread(&mat[i][j],tambyte*sizeof(char),1,img);
	}

	fprintf(mosaico, "%s%d %d\n%d\n", est->head.tipo,c,l,mx); //grava cabecalho do moasico de imagens
	for (i=0;i<l;i++)
		for (j=0;j<c;j++)
			fwrite(&mat[i][j],tambyte*sizeof(char),1,mosaico); //imprime as imagens da matriz no arquivo mosaico

	fclose(mosaico);
	fclose(img);
}
