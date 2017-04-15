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

// grid size = 9x9
#define SIZE 9

// Função que lê um grid do arquivo "filename" e o armazena em uma matriz
int load_grid(int grid[][SIZE], char *filename) {
	FILE *input_file = fopen(filename, "r");

	if (input_file != NULL) {
		for(int i = 0; i < SIZE; i++)
			for(int j = 0; j < SIZE; j++)
				fscanf(input_file, "%d", &grid[i][j]);
		fclose(input_file);
		return 1;
	}

	return 0;
}

int main(int argc, char *argv[]) {

	if(argc != 2) {
		printf("Erro: informe o arquivo de entrada!\nUso: %s <arquivo de entrada>\n\n", argv[0]);
		return 1;
	}

	// Lê o grid do arquivo, armazena na matriz grid e imprime
	int grid[9][9];
	if(load_grid(grid, argv[1])) {
		printf("Quebra-cabecas fornecido:\n");
		for(int i = 0; i < 9; i++) {
			for(int j = 0; j < 9; j++)
				printf("%d ", grid[i][j]);
			printf("\n");
		}
		printf("\n");
	}

	return 0;
}
