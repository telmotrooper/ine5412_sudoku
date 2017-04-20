	//#######################################################################
	//#		Sistemas Operacionais I - Trabalho I - Verificador de Sudoku	#
	//#		Professor: Márcio Castro										#
	//#		Alunos:    Gustavo José Carpeggiani, 							#
	//#				   Luiz Gustavo Coelho Dutra,							#
	//#				   Telmo Henrique Valverde da Silva.					#
	//#		Grupo: K														#
	//#######################################################################

	#include <stdio.h>
	#include <stdlib.h>
	#include <pthread.h>
	#include <assert.h>

	/* grid size = 9x9 */
	#define SIZE 9

	// *********************** Variáveis globais ************************

	int grid[9][9];			// grid é a matriz do sudoku
	int a = 0;				// a determina o tipo de verificação
	int linha = 0;			// linha base verificada
	int coluna = 0;			// coluna base verificada
	int linhaAtual = 0;		// índice da linha onde está sendo comparada
	int colunaAtual = 0;	// índice da coluna onde está sendo comparada
	int regiao = 0;			// região verificada
	int regiaoLinha = 0;	// índice auxiliar para verificação da região
	int regiaoColuna = 0;	// índice auxiliar para verificação da região
	int errosTotais = 0;	// Número total de erros encontrados
	pthread_mutex_t mutex;	// Mutex para sicronização da seção crítica

	// ******************************************************************

	void rotinaLinha(void* arg)
	{
		//pthread_mutex_lock(&mutex);
		if(grid[linha][coluna] == grid[linha][colunaAtual] && coluna != colunaAtual)
		{
			printf("THREAD %li: LINHA:  %i. POSSUI NÚMERO:%i ERRADO!\n", (long)arg, (linha + 1), grid[linha][colunaAtual]);
			errosTotais++;
		}

		colunaAtual++;
		if(colunaAtual > 8)
		{
			coluna++;
			colunaAtual = coluna;
			if(coluna > 8)
			{
				linha++;
				coluna = 0;
				colunaAtual = coluna;
				if(linha > 8)
				{
					linha = 0;
					a++;
				}
			}
		}
		//pthread_mutex_unlock(&mutex);
	}

	void rotinaColuna(void* arg)
	{
		//pthread_mutex_lock(&mutex);
		if(grid[linha][coluna] == grid[linhaAtual][coluna] && linha != linhaAtual)
		{
			printf("THREAD %li: COLUNA: %i. POSSUI NÚMERO:%i ERRADO!\n", (long)arg, (coluna + 1), grid[linhaAtual][coluna]);
			errosTotais++;
		}
		
		linhaAtual++;
		if(linhaAtual > 8)
		{
			linha++;
			linhaAtual = linha;
			if(linha > 8)
			{
				coluna++;
				linha = 0;
				linhaAtual = linha;
				if(coluna > 8)
				{
					coluna = 0;
					a++;
				}
			}
		}
		//pthread_mutex_unlock(&mutex);
	}
	
	void rotinaRegiao(void* arg)
	{
		//pthread_mutex_lock(&mutex);
		if(grid[(linha + regiaoLinha)][(coluna + regiaoColuna)] == grid[(linhaAtual + regiaoLinha)][(colunaAtual + regiaoColuna)])
		{
			if((linha + regiaoLinha) != (linhaAtual + regiaoLinha) || (coluna + regiaoColuna) != (colunaAtual + regiaoColuna))
			{
				printf("THREAD %li: REGIÃO: %i. POSSUI NÚMERO:%i ERRADO!\n", (long)arg, (regiao + 1), grid[(linhaAtual + regiaoLinha)][(colunaAtual + regiaoColuna)]); 
				errosTotais++;
			}
		}
		
		colunaAtual++;
		if((colunaAtual + regiaoColuna) > (2 + regiaoColuna))
		{
			colunaAtual = 0;
			linhaAtual++;
			if((linhaAtual + regiaoLinha) > (2 + regiaoLinha))
			{
				linhaAtual = 0;
				coluna++;
				colunaAtual = coluna;
				if((coluna + regiaoColuna) > (2 + regiaoColuna))
				{
					coluna = 0;
					colunaAtual = coluna;
					linha ++;
					linhaAtual = linha;
					{
						if((linha + regiaoLinha) > (2 + regiaoLinha))
						{
							linha = 0;
							linhaAtual = linha;
							regiao++;
							regiaoColuna += 3;
							if(regiaoColuna > 8)
							{
								regiaoColuna = 0;
								regiaoLinha += 3;
								if(regiaoLinha > 8) // 8 = 2 (base) + 6 (regiaoLinha)
								{
									regiaoLinha = 0;
									regiao = 0;
									a++;
									printf("TOTAL DE ERROS:%i\n", errosTotais);
									pthread_mutex_destroy(&mutex);
								}
							}
						}
					}
				}
			}
		}
		//pthread_mutex_unlock(&mutex);
	}

	// Determina o tipo de tarefa da thread
	void* rotinaThread(void* arg)
	{
		while(a < 3) // enquanto existirem verficações restantes
		{
			pthread_mutex_lock(&mutex);
			switch(a) // determine o tipo de verificação e execute-a
			{
				case 0 : rotinaLinha(arg);
					break;

				case 1 : rotinaColuna(arg);
					break;

				case 2 : rotinaRegiao(arg);
					break;
			}
			pthread_mutex_unlock(&mutex);
		}
		pthread_exit(NULL); // Thread encerra execução.
	}

	//******************************Código do Professor*************************

	/* Função que lê um grid do arquivo "filename" e o armazena em uma matriz */
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

	//**************************************************************************

	int main(int argc, char *argv[]) 
	{

		if(argc != 3) 
		{
			printf("ERRO!\nARGUMENTOS INVÁLIDOS!\nUso: %s <arquivo de entrada> <número de threads>\n\n", argv[0]);
			return 1;
		}

	//******************************Código do Professor*************************

		/* Le o grid do arquivo, armazena na matriz grid e imprime */
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

	//**************************************************************************

		// Criação das threads

		int numThreads = atoi(argv[2]);			// Número de threads passadas por parâmetro
		pthread_t arrayThreads[numThreads]; 	// Array que guarda as referências de todas as threads
		printf("NÚMERO DE THREADS: %i\n", numThreads);

		pthread_mutex_init(&mutex, NULL);

		for(long i = 0; i < numThreads; i++)		// Cria as threads e guarda suas referências
		{
			pthread_create(&arrayThreads[i], NULL, rotinaThread, (void*)i);
		}
		
		for(int i = 0; i < numThreads; i++)		// Começa a execução sincronizada das threads
		{
			pthread_join(arrayThreads[i], NULL);
		}
		
		return 0;
	}

	// Fim do arquivo