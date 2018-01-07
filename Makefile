CC=gcc
C_FLAG=-Wall -g
L_PATH=./lib/
H_PATH=./inc/
O_PATH=./obj/
C_PATH=./src/
B_PATH=./bin/
B=main
N_PROJECT=TPE_huraut_monfort


.PHONY: all distclean clean archive documentation

all: $(B)

$(B): image.o func.o RAG.o iterative_fusion.o $(B).o 
	$(CC) $(O_PATH)$(B).o $(O_PATH)RAG.o $(O_PATH)func.o $(O_PATH)image.o $(O_PATH)iterative_fusion.o -o $(B_PATH)$@

image.o: $(C_PATH)image.c
	mkdir -p bin
	mkdir -p obj
	mkdir -p images
	mkdir -p images/images
	mkdir -p images/saved_images
	$(CC) $(C_FLAG) $^ -c -I $(H_PATH) -o $(O_PATH)$@

func.o: $(C_PATH)func.c
	$(CC) $(C_FLAG) $^ -c -I $(H_PATH) -o $(O_PATH)$@

RAG.o: $(C_PATH)RAG.c
	$(CC) $(C_FLAG) $^ -c -I $(H_PATH) -o $(O_PATH)$@

iterative_fusion.o: $(C_PATH)iterative_fusion.c 
	$(CC) $(C_FLAG) $^ -c -I $(H_PATH) -o $(O_PATH)$@

$(B).o: $(C_PATH)$(B).c
	$(CC) $(C_FLAG) $^ -c -I $(H_PATH) -o $(O_PATH)$@



distclean:
	rm ./bin/*

clean:
	rm ./obj/*.o

archive:
	make
	make distclean
	make clean
	rm ./*~
	rm ./src/*~
	rm ./inc/*~
	cd ..; tar -cvf $(N_PROJECT).tar $(N_PROJECT)

documentation:
	doxygen
