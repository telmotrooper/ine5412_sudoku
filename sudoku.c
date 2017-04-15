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
#include <stdlib.h>
#include "hashset.h"
#include "queue.h"

// grid size = 9x9
#define SIZE 9

// Reads a grid from a file and stores it in a matrix
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
	if(argc != 3) {
        printf("Erro: parâmetros inválidos!\n"
                       "Uso: %s <arquivo de entrada> <número de threads>\n\n", argv[0]);
        return 1;
    }

    int num[10]; // arrays of numbers from 0 to 9
    int numThreads = atoi(argv[2]);
    int numErrors = 0;

    // Reads the grid, stores it in a matrix and prints it
	int grid[9][9];
	if(load_grid(grid, argv[1])) {
		printf("Quebra-cabeças fornecido:\n");
		for(int i = 0; i < 9; i++) {
			for(int j = 0; j < 9; j++)
				printf("%d ", grid[i][j]);
			printf("\n");
		}
		printf("\n");
	}

    // store numbers from 0 to 9 in an array
    for(int x = 0; x <= 9; x++) {
        num[x] = x;
    }

    hashset_t set = hashset_create();

    if(set == NULL) {
        printf("Erro: Falha na inicialização do conjunto.");
        return 1;
    }

    // every line
    for(int x = 0; x < 9; x++) {
        hashset_t set = hashset_create();

        for(int y = 0; y < 9; y++) {
            int value = grid[x][y];

            if (hashset_add(set, &num[value]) == 0) {
                printf("Erro na linha %d.\n", x+1);
                numErrors++;
            }

        }

        hashset_destroy(set);
    }

    // every column
    for(int x = 0; x < 9; x++) {
        hashset_t set = hashset_create();

        for(int y = 0; y < 9; y++) {
            int temp = grid[y][x];

            if (hashset_add(set, &num[temp]) == 0) {
                printf("Erro na coluna %d.\n", x+1);
                numErrors++;
            }

        }

        hashset_destroy(set);
    }

    // every region
    // part 1 - find all regions
    for(int x = 0; x < 9; x = x + 3) {
        for(int y = 0; y < 9; y = y + 3) {
            hashset_t set = hashset_create(); // for each region a new set is generated

            // part 2 - loop through the content of each region
            for(int z = 0; z < 3; z++) {
                for(int w = 0; w < 3; w++) {
                    int temp = grid[x+z][y+w];

                    if (hashset_add(set, &num[temp]) == 0) {
                        printf("Erro na região %d.\n", x+z+1);
                        numErrors++;
                    }
                }
            }

            hashset_destroy(set);
        }
    }

    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++)
            printf("%d ", grid[i][j]);
        printf("\n");
    }


    printf("Erros encontrados: %d.\n", numErrors);

	return 0;
}
