/* Contador de numeros primos multithread
 *
 * Le da entrada padrao varios numeros inteiros positivos de tamanhao ate 2 elevado a 64
 * Inicializa threads independentes que calculam a primalidade dos numeros
 * Ha um numero maximo de THREADS independentes, N_THREADS + programa principal
 *
 * Victor Ferrao Santolim
 * RA 187888
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


//Numero maximo de threads que calculam primalidade
#define N_THREADS 4

//Prototipo da funcao que os threads executam
void *threadCalcPrimo(void *w);

unsigned long int entrada[100];
char disponivel[100];
int numAteEntao = 0;
int indice = 0;
pthread_mutex_t trava;

int main() {

	pthread_t threads[THREADS]; //Vetor de threads que calculam nùmeros primos
	unsigned long int numLido; //Leitura atual

    int numPrimos = 0;		 //Numero de primos contado ate o momento
    int threadsCriadas = 0;

    //Zera os vetores iniciais
    for(int i = 0 ; i < 100; i++)
    {
    	entrada[i] = 0;
    	disponivel[i] = 0;
    }

    //Pega os números da entrada e coloca na memória
	do
	{
       scanf("%lu", &numLido); //Pega um numero da entrada

       entrada[numAteEntao] = numLido;
       disponivel[numAteEntao] = 1;
       numAteEntao++;

	}while(getchar() != 10); //Enquanto nao pega um \n


	//Cria as threads
  	if(numAteEntao < N_THREADS)
  	{
  		for(int i = 0 ; i < numAteEntao ; i++)
  		{
  			pthread_create(&(threads[i]), NULL, threadCalcPrimo, NULL);
  		}
  		threadsCriadas = numAteEntao;
  	}
  	else
  	{
  		for(int i = 0 ; i < N_THREADS ; i++)
  		{
  			pthread_create(&(threads[i]), NULL, threadCalcPrimo, NULL);
  		}
  		threadsCriadas = N_THREADS;
  	}



	/* Esperando threads */
  	for (int i = 0; i < threadsCriadas; i++) 
  	{
    	pthread_join(threadCalcPrimo[i], NULL);
	}

    //Le o vetor com os resultados de primalidade
    for(int i = 0 ; i < numAteEntao ; i++)
    {
    	if(entrada[i] == 1) numPrimos++;
    }

    //Imprime o resultado final e retorna
    printf("%d\n", numPrimos);

    return 0;

}

//Funcao que eh executada nos threads
//Verifica os vetores entrada e disponivel para saber o que calcular
void *threadCalcPrimo(void *w)
{
	unsigned long int n;
	int indiceLocal;
	char naoPrimo = 0;

	while(1)
	{
		
		pthread_mutex_lock(&trava);

			if(indice + 1 == numAteEntao) break;

			if(disponivel[indice] == 1)
			{
				disponivel[indice] == 0;
				n = entrada[indice];
				indiceLocal = indice;
				indice++;
			}

		pthread_mutex_unlock(&trava);

		naoPrimo = 0;

		//Se for 0 ou 1, nao eh primo
		if(n == 0 || n == 1) naoPrimo = 1;
		//Verifica se eh divisivel ate metade do numero. Se nao for, eh primo
		for(unsigned long int i=2 ; i <= n/2 ; i++)
		{
	        if(n%i==0)
	        {
	            naoPrimo=1;
	            break;
	        }
		}

		//Escreve no pipe de saida, enviando o resultado para o pai
		// 1 = Nao eh primo
		// 0 = Eh primo
		write(filhoParaPai[1] , &naoPrimo, 1);
	}

	//Finaliza a thread
	return NULL;
}