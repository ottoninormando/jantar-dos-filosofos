//By João Vinicius Ottoni Normando

#include "iostream"
#include "process.h"
#include "Windows.h"
#include "vector"
#include "time.h"
#include "stdio.h"
using namespace std;

int Hashi[5] = {0,0,0,0,0};

const int num_threads = 5;

typedef struct {
	int Id_filosofo;
	int hashi_esquerda;
	int hashi_direita;
}parametros;

HANDLE hthread[num_threads];
parametros p[num_threads];
HANDLE hSemafaro;
HANDLE hMutex = CreateMutex(NULL, FALSE, NULL);

void devolverHashi(int Id_thread, int hashi_esquerda, int hashi_direita)
{
	Hashi[hashi_direita] = 0;
	Hashi[hashi_esquerda] = 0;
	printf("O Filosofo %d devolveu os seus Hashis. \n", Id_thread);
}

void comerHashi(int Id_thread, int hashi_esquerda, int hashi_direita)
{
	printf("O Filosofo %d esta comendo Hashi..... \n", Id_thread);
	int comer = rand() % 10000;
	Sleep(comer);
	printf("O Filosofo %d terminou de comer Hashi apos %d seg \n", Id_thread, comer / 1000);
	devolverHashi(Id_thread, hashi_esquerda, hashi_direita);
}

void meditar(int Id_thread)
{
	printf("O Filosofo %d comecou a meditar\n", Id_thread);
	int meditar = rand() % 10000;
	Sleep(meditar);
	printf("O Filosofo %d terminou de meditar e meditou por %d seg. \n", Id_thread, meditar/1000);

}

void pegaHashi(int Id_thread, int hashi_esquerda, int hashi_direita)
{
	int flag = 0;
	if (Hashi[hashi_esquerda] == 0) {	
		Hashi[hashi_esquerda] = 1;
		flag = flag + 1;
		printf("O Filosofo %d pegou o hashi %d. \n", Id_thread, hashi_esquerda);
	}
	if (Hashi[hashi_direita] == 0) {
		Hashi[hashi_direita] = 1;
		flag = flag + 1;
		printf("O Filosofo %d pegou o hashi %d. \n", Id_thread, hashi_direita);
	}
	if (flag == 2) {
		comerHashi(Id_thread, hashi_esquerda, hashi_direita);
		flag = 0;
	}
	else {
		printf("O Filosofo %d nao conseguio os seus dois hashi para comer.\n", Id_thread);
		//printf("Vetor %d %d %d %d %d: \n", Hashi[0], Hashi[1], Hashi[2], Hashi[3], Hashi[4] );
	}
}


void Vida_de_filosofo(void *param) {
	parametros *pr = (parametros*)param;

	int Id_filosofo = pr->Id_filosofo;
	int hashi_esquerda = pr->hashi_esquerda;
	int hashi_direita = pr->hashi_direita;

	while (TRUE) {

		meditar(Id_filosofo);

		if (WaitForSingleObject(hMutex, INFINITE) != WAIT_FAILED) { // Secao Critica

			pegaHashi(Id_filosofo, hashi_esquerda, hashi_direita);
		}
		ReleaseMutex(hMutex);
	}
	_endthread(); // Finalizando a Thread
}
void Cria_thread() {

	for (int i = 0; i < 5; i++) {
		
		if (i == 0) {
			p[i].Id_filosofo = i;
			p[i].hashi_esquerda = 4;
			p[i].hashi_direita = 0;
			
			hthread[i] = (HANDLE)_beginthread(Vida_de_filosofo, 0, &p[i]);
		}
		if (i == 1) {
			p[i].Id_filosofo = i;
			p[i].hashi_esquerda = 0;
			p[i].hashi_direita = 1;

			hthread[i] = (HANDLE)_beginthread(Vida_de_filosofo, 0, &p[i]);
		}
		if (i == 2) {
			p[i].Id_filosofo = i;
			p[i].hashi_esquerda = 1;
			p[i].hashi_direita = 2;

			hthread[i] = (HANDLE)_beginthread(Vida_de_filosofo, 0, &p[i]);
		}
		if (i == 3) {
			p[i].Id_filosofo = i;
			p[i].hashi_esquerda = 2;
			p[i].hashi_direita = 3;

			hthread[i] = (HANDLE)_beginthread(Vida_de_filosofo, 0, &p[i]);
		}
		if (i == 4) {
			p[i].Id_filosofo = i;
			p[i].hashi_esquerda = 3;
			p[i].hashi_direita = 4;

			hthread[i] = (HANDLE)_beginthread(Vida_de_filosofo, 0, &p[i]);
		}

		//printf("Filosofo %d, esquerda = %d, direita = %d\n", i, p[i].hashi_esquerda, p[i].hashi_direita);

	}
	WaitForMultipleObjects(num_threads, hthread, TRUE, INFINITE);
	
}


//Main
int main()
{
	Cria_thread();
	system("PAUSE");

}



