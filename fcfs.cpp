#include <bits/stdc++.h>

#define NUM_THREADS 100
#define VALOR 10000

using namespace std;
// variaveis de controle da região critica
mutex mtx;
condition_variable cond_var;
volatile int atual = -1;
volatile bool flag = false;
volatile int cont_thread = 0;

// variaveis compartilhadas
volatile int requisicao = 0;
volatile int resposta = 0;
volatile bool pronto = false;

// controle de tempo
volatile clock_t tempo_inicial_escalonador;

// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------

// função de somatorio
int somatorio(int n){
    // (n * (n+1))/2
    int soma = n;
    for(int i = 0; i < n; i++)
        soma += i;

    return soma;
}

// linha de execução do servidor
void servidor(int id){
    while(true){
        if(flag){
            resposta = somatorio(requisicao);
            cont_thread++;
            flag = false;
        }

        // condição de parada do servidor
        if(cont_thread == NUM_THREADS)
            break;
    }
}

// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------

// linha de execução da thread cliente
void cliente(int id){
    // controles do tempo
    clock_t inicio, fim;
    double duracao, espera;
    inicio = clock();

    // trava a si mesma, esperando a vez
    unique_lock<mutex> trava(mtx);
    // thread fica esperando ser a sua vez
    while(id != atual or !pronto)
        cond_var.wait(trava);

    // thread vê que é sua vez e sai do while
    // processo da thread
    requisicao = rand() % VALOR + 1;

    // testes
    cout << "Thread: ";
    cout << id << ".";
    cout << " Atual = ";
    cout << atual << endl;
    cout << "Valor requisitado: " << requisicao << endl;

    // thread chama o servidor e espera a resposta
    flag = true;
    while(flag);

    // cliente recebe a resposta e imprime o valor
    cout << "Valor respondido: " << resposta << endl;

    // controle do tempo
    fim = clock();
    duracao = 1000 * (double) (fim - inicio) / (double) (CLOCKS_PER_SEC);
    espera = 1000 * (double) (fim - tempo_inicial_escalonador) / (double) (CLOCKS_PER_SEC);

    // imprime o tempo de resposta / espera
    printf("Tempo de resposta: %.2fms\n", duracao);
    printf("Tempo de espera: %.2fms\n\n", espera);
}

// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------

// escalonador fcfs
void first_come_first_served(thread threads[]){
    // variaveis de controle de tempo
    clock_t fim;
    double duracao, vazao;
    for(int i = 0; i < NUM_THREADS; i++){
        pronto = true;
        atual++;
        cond_var.notify_all();
        threads[i].join();
        pronto = false;
    }

    // controle do tempo
    fim = clock();
    duracao = 1000 * (double) (fim - tempo_inicial_escalonador) / (double) (CLOCKS_PER_SEC);
    vazao = NUM_THREADS / duracao;
    printf("Vazão: %.2f threads/ms\n", vazao);
}

// ---------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------

int main(){
    cout << "Escalonador FCFS\n";
    // criação da thread servidor
    thread server = thread(servidor, -1);
    // inicia o tempo do escalonador
    tempo_inicial_escalonador = clock();
    // criação das threads cliente
    thread clientes[NUM_THREADS];
    for(int i = 0; i < NUM_THREADS; i++)
        clientes[i] = thread(cliente, i);

    first_come_first_served(clientes);
    return 0;
}
