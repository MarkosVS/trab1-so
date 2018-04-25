#include <bits/stdc++.h>

#define NUM_THREADS 100
#define VALOR 10000

using namespace std;
// variaveis de controle da região critica
mutex mtx;
condition_variable cond_var;
volatile int atual = 0;

// variaveis compartilhadas
volatile int requisicao = 0;
volatile int resposta = 0;
volatile int id_thread = -1;

// função de somatorio
int somatorio(int n){
    // (n * (n+1))/2
    int soma = n;
    for(int i = 0; i < n; i++)
        soma += i;

    return soma;
}

// linha de execução da thread cliente
void cliente(int id){
    // trava a si mesma, esperando a vez
    unique_lock<mutex> trava(mtx);
    // thread fica esperando ser a sua vez
    while(id != atual)
        cond_var.wait(trava);

    // thread vê que é sua vez e sai do while
    atual++;

    // processo da thread
    requisicao = rand() % VALOR + 1;

    // testes
    cout << "Thread: ";
    cout << id + 1 << ".";
    cout << " Atual = ";
    cout << atual << endl;
    cout << requisicao << endl;

    // thread passa a vez
    requisicao = 0;
    cond_var.notify_all();
}

// linha de execução do servidor
void servidor(int id){
    cout << "Deu certo\n";
}

int main(){
    // criação da thread servidor
    thread server = thread(servidor, -1);
    // criação das threads cliente
    thread clientes[NUM_THREADS];
    for(int i = 0; i < NUM_THREADS; i++)
        clientes[i] = thread(cliente, i);

    for(int i = 0; i < NUM_THREADS; i++)
        clientes[i].join();

    return 0;
}
