/*
 ============================================================================
 Name        : sudoku.c
 Authors     : GJC, LGCD, THVS
 Version     : 1.0
 Copyright   : Public Domain
 Description : Trabalho I de Sistemas Operacionais I
 ============================================================================
 */

#include <stdio.h>
#include <unistd.h> // access to POSIX API
#include <sys/types.h> // data types including "pid_t" and "pthread_t"
#include <pthread.h>
#include <stdlib.h>

/* grid size = 9x9 */
#define SIZE 9

/* Variáveis globais */
pthread_mutex_t mutex;
int numErros = 0;

/* Funcao que le um grid do arquivo "filename" e o armazena em uma matriz */
int load_grid(int grid[][SIZE], char *filename) // QUE?
{
	FILE *input_file = fopen(filename, "r");

	if (input_file != NULL)
	{
		for(int i = 0; i < SIZE; i++)
			for(int j = 0; j < SIZE; j++)
				fscanf(input_file, "%d", &grid[i][j]);
		fclose(input_file);
		return 1;
	}

	return 0;
}

// Primeira função de verificação

void *verifiqueLinha(void *arg)
{
	// TODO Verificação de linha.
}

// Segunda função de verificação

void *verifiqueColuna(void *arg)
{
	// TODO Verificação de coluna.
}

// Terceira função de verificação

void *verifiqueRegiao(void *arg)
{
	// TODO Verificação de seção.
}

void incrementaNumErros() {
	numErros++;
}

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
        printf("Erro: parâmetros inválidos!\n");
        printf("Uso: %s <arquivo de entrada> <número de threads>\n\n", argv[0]);
		return 1;
	}

	/* Le o grid do arquivo, armazena na matriz grid e imprime */
	int grid[9][9];
	if(load_grid(grid, argv[1]))
	{
		printf("Quebra-cabecas fornecido:\n");
		for(int i = 0; i < 9; i++)
		{
			for(int j = 0; j < 9; j++)
				printf("%d ", grid[i][j]);
			printf("\n");
		}
		printf("\n");
	}

	// Com o grid pronto, agora basta criar as threads verificadoras

	// TODO Criar as 27 threads

//    printf(argv[2]);

    int numThreads = atoi(argv[2]);

    for(int i = 0; i < numThreads; i++) {
        // CRIAR THREADS
        printf("AQUI!\n");
    }

	return 0;
}