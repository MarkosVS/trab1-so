#include <bits/stdc++.h>
#include <pthread.h>

#define NUM_THREADS 100
#define VALUE 10000

using namespace std;

volatile int requisicoes[100];
volatile int request = 0;
volatile int respostas[100];
volatile int turn = 0;

// função de somatorio
int somatorio(int n){
    int soma = n;
    for(int i = 0; i < n; i++)
        soma += i;

    return soma;
}

// linha de execução da thread cliente
void* cliente(int id){
    // esperar ser a vez dele
    while(turn != id);

    request = rand()%VALUE;
    //string teste = "Thread " + ((string) id) + " - request = " + ((string) request) + ".\n";
    //cout << teste;

    // passar a vez para o próximo processo
    if(id == NUM_THREADS - 1)
        turn = 0;
    else
        turn++;
}

// linha de execução da thread servidor
void* servidor(){
    cout << "Deu certo" << endl;
}

int main(){
    // criação da thread servidor
    pthread_t server;
    pthread_create(&server, NULL, servidor, NULL);
    //thread a = thread(servidor);

    // criação das threads clientes
    pthread_t clientes[NUM_THREADS];
    for(int i = 0; i < NUM_THREADS; i++)
        pthread_create(&clientes[i], NULL, cliente, (void*)i);


    return 0;
}