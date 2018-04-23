#include <iostream>
#include <stdlib.h>
#include <thread>

#define NUM_THREADS 100

using namespace std;

// função de somatorio
int somatorio(int n){
    int soma = n;
    for(int i = 0; i < n; i++)
        soma += i;

    return soma;
}

// linha de execução da thread cliente
void cliente(int id){
    int r = rand()%10000;
    cout << "Thread " << id << ": " << r << endl;
}

int main(){
    // criação das threads
    thread clientes[NUM_THREADS];
    for(int i = 0; i < NUM_THREADS; i++)
        clientes[i] = thread(cliente, i);

    for(int i = 0; i < NUM_THREADS; i++)
        clientes[i].join();

    return 0;
}