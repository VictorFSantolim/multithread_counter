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
#include <stdint.h>
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>


//Numero maximo de threads que calculam primalidade
#define N_THREADS 4

//Prototipo da funcao que os threads executam
void threadCalcPrimo( TODO  );

int main() {

	pthread_t threads[THREADS]; //Vetor de threads que calculam nùmeros primos
	unsigned long int numLido; //Leitura atual

    int threadsAteEntao = 0; //Filhos criados ate o momento
    char resPrim;			 //Resultado do teste de primalidade de que uma thread fez
    int numPrimos = 0;		 //Numero de primos contado ate o momento

	do
	{
       scanf("%lu", &numLido); //Pega um numero da entrada

       //Cria novos filhos caso ainda existam menos que N_FILHOS
       if(filhosAteEntao < N_FILHOS)
       {
       		//Cria o filho
       		filhos[filhosAteEntao] = fork();

       		//Erro
	    	if(filhos[filhosAteEntao] < 0)
	    	{
	    		printf("Erro no fork do filho %d\n", filhosAteEntao);
	    		return -1;
	    	}

	    	//Processo filho
	    	else if(filhos[filhosAteEntao] == 0)
	    	{
	    		
	    		//Inicializa um filho, que calcula primalidades
	    		filhoCalcPrimo(filhoParaPai , paiParaFilho , filhosAteEntao + 1);
	    	}
	    	//Processo pai
	    	else
	    	{
	    		filhosAteEntao++;
	    	}

       }

	}while(getchar() != 10); //Enquanto nao pega um \n


	//Fecha as streams nao usadas pelo pai
	close(paiParaFilho[0]);
   	close(filhoParaPai[1]);
	    		

	//Fecha a escrita na pipe do pai para filho, o que sinaliza aos filhos
	//que podem parar de tentar ler novos numeros e podem encerrar.
	close(paiParaFilho[1]);


	//Aguarda o encerramento de todos os processos filhos
  	for(int i = 0; i < filhosAteEntao; i++) 
    	waitpid(filhos[i], NULL, 0);

    //Le as informacoes que os filhos colocaram na pipe, referente aos
    //resultados dos testes de primalidade
    while(read(filhoParaPai[0], &resPrim, 1) > 0) 
    {
      
      //A cada zero recebido , acrescenta o numero de primos
      if(resPrim == 0) numPrimos++;
      
    }

    //Por fim, fecha a pipe de leitura dos dos filhos para o pai
    close(filhoParaPai[0]);

    //Imprime o resultado final e retorna
    printf("%d\n", numPrimos);

    return 0;

}

//Funcao que eh executada nos processos filhos
//Recebe os pipes de comunicacao interprocesso cradas
void filhoCalcPrimo(int filhoParaPai[2] , int paiParaFilho[2] , int numFilho)
{
	unsigned long int n;
	char naoPrimo = 0;

	//Fecha as extremidades das pipe que nao serao utilizadas
	close(filhoParaPai[0]);
	close(paiParaFilho[1]);

	//Tenta realizar a leitura de novos numeros no pipe
	//Se houver, verifica se eh primo
	while(read(paiParaFilho[0] , &n , 8) > 0)
	{
		
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

	//Finaliza o processo
	exit(1);
}