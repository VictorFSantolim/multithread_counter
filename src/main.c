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
int numAteEntao = 0;
int indice = 0;
pthread_mutex_t trava;

int main() {

	pthread_t threads[N_THREADS]; //Vetor de threads que calculam nùmeros primos
	int threadID[N_THREADS];
	unsigned long int numLido; //Leitura atual

    int numPrimos = 0;		 //Numero de primos contado ate o momento
    int threadsCriadas = 0;

    //Zera os vetores iniciais
    for(int i = 0 ; i < 100; i++)
    {
    	entrada[i] = 0;
    }

    //Pega os números da entrada e coloca na memória
	do
	{
       scanf("%lu", &numLido); //Pega um numero da entrada

       entrada[numAteEntao] = numLido;
       printf("Pegou da entrada o n = %lu e colocou na posicao %d do vetor entrada\n", numLido , numAteEntao);
       numAteEntao++;

	}while(getchar() != 10); //Enquanto nao pega um \n


	//Cria as threads
  	if(numAteEntao < N_THREADS)
  	{
  		for(int i = 0 ; i < numAteEntao ; i++)
  		{
  			threadID[i] = i;
  			pthread_create(&(threads[i]), NULL, threadCalcPrimo, (void*)(threadID + i));
  		}
  		threadsCriadas = numAteEntao;
  	}
  	else
  	{
  		for(int i = 0 ; i < N_THREADS ; i++)
  		{
  			threadID[i] = i;
  			pthread_create(&(threads[i]), NULL, threadCalcPrimo, (void*)(threadID + i));
  		}
  		threadsCriadas = N_THREADS;
  	}

  	printf("chegou 1\n");

	/* Esperando threads */
  	for (int i = 0; i < threadsCriadas; i++) 
  	{
    	pthread_join(threads[i], NULL);
	}

	printf("chegou 2\n");

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

	int* pointerNum = (int*)w;
	int id = *pointerNum;

	while(1)
	{
		
		printf("Antes do Lock 1 da thread %d\n" , id);
		pthread_mutex_lock(&trava);

		printf("Indice = %d\n", indice);

		if(indice >= numAteEntao) break;

		n = entrada[indice];
		indiceLocal = indice;
		indice++;

		printf("A thread %d esta tratando o numero %lu\n", id , n);
			
		pthread_mutex_unlock(&trava);
		printf("Depois do Lock 1 da thread %d\n" , id);

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

		// 1 = Nao eh primo
		// 0 = Eh primo

		printf("Antes do Lock 2 da thread %d\n" , id);
		pthread_mutex_lock(&trava);

		if(naoPrimo == 0){ entrada[indiceLocal] = 1;}
		else {entrada[indiceLocal] = 0;}

		pthread_mutex_unlock(&trava);
		printf("Depois do Lock 2  da thread %d\n" , id);

	}

	//Finaliza a thread
	pthread_mutex_unlock(&trava);
	return NULL;
}