CC=gcc
C_FLAG=-Wall -g
L_PATH=./lib/
H_PATH=./inc/
O_PATH=./obj/
C_PATH=./src/
B_PATH=./build/
T=testRAG

.PHONY: all distclean clean

all: testimage

testimage: image.o func.o RAG.o $(T).o 
	$(CC) $(O_PATH)$(T).o $(O_PATH)RAG.o $(O_PATH)func.o $(O_PATH)image.o -o $(B_PATH)$@

image.o: $(C_PATH)image.c
	$(CC) $(C_FLAG) $^ -c -I $(H_PATH) -o $(O_PATH)$@

func.o: $(C_PATH)func.c
	$(CC) $(C_FLAG) $^ -c -I $(H_PATH) -o $(O_PATH)$@

RAG.o: $(C_PATH)RAG.c
	$(CC) $(C_FLAG) $^ -c -I $(H_PATH) -o $(O_PATH)$@

$(T).o: $(C_PATH)$(T).c
	$(CC) $(C_FLAG) $^ -c -I $(H_PATH) -o $(O_PATH)$@



distclean:
	rm ./build/*
clean:
	rm ./obj/*.o
