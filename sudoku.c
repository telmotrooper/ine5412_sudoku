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

/* grid size = 9x9 */
#define SIZE 9

// *********************** Variáveis globais ************************

int grid[9][9];			// Matriz do sudoku // APENAS LEITURA
int errosTotais = 0;	// Número total de erros encontrados

// Variáveis - Linha

int linhaL = 0;
int colunaL = 0;
int colunaAtualL = 0;
int aL = 0; 

// Variáveis - Coluna

int linhaC = 0;
int colunaC = 0;
int linhaAtualC = 0;
int aC = 0;				

// Variáveis - Região

int linha = 0;			// linha base verificada
int coluna = 0;			// coluna base verificada
int linhaAtual = 0;		// índice da linha onde está sendo comparada
int colunaAtual = 0;	// índice da coluna onde está sendo comparada
int regiao = 0;			// região verificada
int regiaoLinha = 0;	// índice auxiliar para verificação da região
int regiaoColuna = 0;	// índice auxiliar para verificação da região
int aR = 0;				// Determina se ainda existem verificações restantes

// Mutex

pthread_mutex_t mutexLinha;	
pthread_mutex_t mutexColuna;
pthread_mutex_t mutexRegiao;
pthread_mutex_t mutexErrosTotais;

// ******************************************************************

void* rotinaThreadRegiao(void* arg) // arg contém o identificador único da thread
{
	int linhaThreadRegiao = 0;
	int colunaThreadRegiao = 0;
	int linhaAtualThreadRegiao = 0;
	int colunaAtualThreadRegiao = 0;
	int regiaoThreadRegiao = 0;
	int regiaoLinhaThreadRegiao = 0;
	int regiaoColunaThreadRegiao = 0;

	while(aR != 1) // Enquanto existirem regiões restantes para verificar
	{
		pthread_mutex_lock(&mutexRegiao);

		// Primeiro copia o valor atual das variáveis globais para si mesma
		
		linhaThreadRegiao = linha;
		colunaThreadRegiao = coluna;
		linhaAtualThreadRegiao = linhaAtual;
		colunaAtualThreadRegiao = colunaAtual;
		regiaoThreadRegiao = regiao;
		regiaoLinhaThreadRegiao = regiaoLinha;
		regiaoColunaThreadRegiao = regiaoColuna;

		// Após isto atualiza as variáveis globais para as outras threads poderem usar o valor atualizado

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
									aR++;
								}
							}
						}
					}
				}
			}
		}

		pthread_mutex_unlock(&mutexRegiao);

		// Por fim executa sua verificação com o valor previamente salvo em suas variáveis locais

		if(grid[(linhaThreadRegiao + regiaoLinhaThreadRegiao)][(colunaThreadRegiao + regiaoColunaThreadRegiao)] == grid[(linhaAtualThreadRegiao + regiaoLinhaThreadRegiao)][(colunaAtualThreadRegiao + regiaoColunaThreadRegiao)])
		{
			if((linhaThreadRegiao + regiaoLinhaThreadRegiao) != (linhaAtualThreadRegiao + regiaoLinhaThreadRegiao) || (colunaThreadRegiao + regiaoColunaThreadRegiao) != (colunaAtualThreadRegiao + regiaoColunaThreadRegiao))
			{
				printf("THREAD %li: REGIÃO: %i. POSSUI NÚMERO:%i ERRADO!\n", ((long)arg + 1), (regiaoThreadRegiao + 1), grid[(linhaAtualThreadRegiao + regiaoLinhaThreadRegiao)][(colunaAtualThreadRegiao + regiaoColunaThreadRegiao)]); 
				pthread_mutex_lock(&mutexErrosTotais);
				errosTotais++;
				pthread_mutex_unlock(&mutexErrosTotais);
			}
		}
	}
	// Se já não existe mais trabalho encerra a thread
	pthread_exit(NULL); // Thread encerra execução.
}

void* rotinaThreadColuna(void* arg) // arg contém o identificador único da thread
{
	int linhaThreadColuna = 0;
	int colunaThreadColuna = 0;
	int linhaAtualThreadColuna = 0;

	while(aC != 1) // Enquanto existirem colunas restantes para verificar
	{
		pthread_mutex_lock(&mutexColuna);

		// Primeiro copia o valor atual das variáveis globais para si mesma
			
		linhaThreadColuna = linhaC;
		colunaThreadColuna = colunaC;
		linhaAtualThreadColuna = linhaAtualC;

		// Após isto atualiza as variáveis globais para as outras threads poderem usar o valor atualizado

		linhaAtualC++;
		if(linhaAtualC > 8)
		{
			linhaC++;
			linhaAtualC = linhaC;
			if(linhaC > 8)
			{
				colunaC++;
				linhaC = 0;
				linhaAtualC = linhaC;
				if(colunaC > 8)
				{
					colunaC = 0;
					aC++;
				}
			}
		}

		pthread_mutex_unlock(&mutexColuna);

		// Por fim executa sua verificação com o valor previamente salvo em suas variáveis locais

		if(grid[linhaThreadColuna][colunaThreadColuna] == grid[linhaAtualThreadColuna][colunaThreadColuna] && linhaThreadColuna != linhaAtualThreadColuna)
		{
			printf("THREAD %li: COLUNA: %i. POSSUI NÚMERO:%i ERRADO!\n", ((long)arg + 1), (colunaThreadColuna + 1), grid[linhaAtualThreadColuna][colunaThreadColuna]);
			pthread_mutex_lock(&mutexErrosTotais);
			errosTotais++;
			pthread_mutex_unlock(&mutexErrosTotais);
		}
	}
	// Se já acabou de verificar todas as colunas passa para as regiões
	rotinaThreadRegiao(arg);
}

void* rotinaThreadLinha(void* arg) // arg contém o identificador único da thread
{
	int linhaThreadLinha = 0;
	int colunaThreadLinha = 0;
	int colunaAtualThreadLinha = 0;
	while(aL != 1) // Enquanto existirem linhas restantes para verificar
	{
		pthread_mutex_lock(&mutexLinha);

		// Primeiro copia o valor atual das variáveis globais para si mesma
			
		linhaThreadLinha = linhaL;
		colunaThreadLinha = colunaL;
		colunaAtualThreadLinha = colunaAtualL;


		// Após isto atualiza as variáveis globais para as outras threads poderem usar o valor atualizado

		colunaAtualL++;
		if(colunaAtualL > 8)
		{
			colunaL++;
			colunaAtualL = colunaL;
			if(colunaL > 8)
			{
				linhaL++;
				colunaL = 0;
				colunaAtualL = colunaL;
				if(linhaL > 8)
				{
					linhaL = 0;
					aL++;
				}
			}
		}

		pthread_mutex_unlock(&mutexLinha);

		// Por fim executa sua verificação com o valor previamente salvo em suas variáveis locais

		if(grid[linhaThreadLinha][colunaThreadLinha] == grid[linhaThreadLinha][colunaAtualThreadLinha] && colunaThreadLinha != colunaAtualThreadLinha)
		{
			printf("THREAD %li: LINHA:  %i. POSSUI NÚMERO:%i ERRADO!\n", ((long)arg + 1), (linhaThreadLinha + 1), grid[linhaThreadLinha][colunaAtualThreadLinha]);
			pthread_mutex_lock(&mutexErrosTotais);
			errosTotais++;
			pthread_mutex_unlock(&mutexErrosTotais);
		}
	}
	// Se já acabou de verificar todas as linhas passa para colunas
	rotinaThreadColuna(arg);
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



	/* Lê o grid do arquivo, armazena na matriz grid e imprime */
	if(load_grid(grid, argv[1])) 
	{
		printf("Quebra-cabeças fornecido:\n");
		for(int i = 0; i < 9; i++) 
		{
			for(int j = 0; j < 9; j++)
				printf("%d ", grid[i][j]);
			printf("\n");
		}
		printf("\n");
	}

//**************************************************************************

	// Codificação de parâmetro

	int numThreads = atoi(argv[2]);			// Número de threads passadas por parâmetro
	pthread_t arrayThreads[numThreads]; 	// Array que guarda as referências de todas as threads
	
	printf("NÚMERO DE THREADS: %i\n", numThreads);

	// Inicialização de mutex

	pthread_mutex_init(&mutexLinha, NULL);
	pthread_mutex_init(&mutexColuna, NULL);
	pthread_mutex_init(&mutexRegiao, NULL);
	pthread_mutex_init(&mutexErrosTotais, NULL);

	// Criação das threads
	int k = 0;
	for(long i = 0; i < numThreads; i++) 
	{
		if(k == 0) // Linha
		{
			pthread_create(&arrayThreads[i], NULL, rotinaThreadLinha, (void*)i);
			k++;
		}
		else
		{
			if(k == 1) // Coluna
			{
				pthread_create(&arrayThreads[i], NULL, rotinaThreadColuna, (void*)i);
				k++;
			}
			else 
			{
				if(k == 2) // Região
				{
					pthread_create(&arrayThreads[i], NULL, rotinaThreadRegiao, (void*)i);
					k = 0;
				}
			}
		}
	}

	// Começa a execução sincronizada das threads

	for(int i = 0; i < numThreads; i++)		
	{
		pthread_join(arrayThreads[i], NULL);
	}

	// Após todas as tarefas serem executadas é impresso número total de erros

	// Imprime erros
	printf("TOTAL DE ERROS:%i\n", errosTotais); 
	// Encerra as mutex
	pthread_mutex_destroy(&mutexLinha); 
	pthread_mutex_destroy(&mutexColuna);
	pthread_mutex_destroy(&mutexRegiao);
	pthread_mutex_destroy(&mutexErrosTotais);
		
	return 0;
}

// Fim do arquivo