# welyngton Dal Pra - nov/2009
# gera mosaico de imagens

tudo: mosaico.o banco.o imagem.o src/mosaico.h
	gcc -Wall -m32 -o bin/mosaico mosaico.o imagem.o banco.o src/mosaico.h
	@ln -s bin/mosaico mosaico

mosaico.o: src/mosaico.c src/mosaico.h
	gcc -m32 -c src/mosaico.c src/mosaico.h

imagem.o: src/imagem.c src/mosaico.h
	gcc -m32 -c src/imagem.c src/mosaico.h

banco.o: src/banco.c src/mosaico.h
	gcc -m32 -c src/banco.c src/mosaico.h

limpa:
	@rm *~ *bak core*

faxina: 
	@rm *.o bin/mosaico mosaico
	@rm -rf html/ latex/
	@rm *~ *bak core* mosaico.bd mosaico

