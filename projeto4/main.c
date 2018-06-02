#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define UNTEMPO 5 //minutos
#define DECOLAGEM 0

typedef struct aviao {
  char codigo[7];
  char status;
  int prioridade;
} Aviao;

typedef struct lista {
  Aviao info;
  struct lista* prox;
} Lista;

typedef struct fila {
  Lista* inicio;
  Lista* fim;
} Fila;

void aproximacoes_decolagens(int NVoos, int *NAproximacoes, int *NDecolagens);
Lista *novo_aviao();
Fila iniciar_avioes(int * NVoos, int *NAproximacoes, int *NDecolagens);

int main() {
  srand(time(NULL));
  int NVoos;
  int NAproximacoes;
  int NDecolagens;
  Fila voos;

  voos = iniciar_avioes( &NVoos, &NAproximacoes, &NDecolagens);
  printf("NVoos: %d\n", NVoos );
  printf("NAproximacoes: %d\n", NAproximacoes);
  printf("NDecolagens: %d\n", NDecolagens);

  Lista *avioes = voos.inicio;

  printf("Lista:\n");
  while (avioes != NULL) {

    printf("Código de Voo: %s - %c", avioes->info.codigo, avioes->info.status);
    if (avioes->info.status == 'A') {
      printf(" - prioridade: %d", avioes->info.prioridade);
    }
    printf("\n");
    avioes = avioes->prox;

  }
  printf("\n\nULTIMO:\n\n");
  printf("Código: %s  status: %c", voos.fim->info.codigo, voos.fim->info.status);
  if (voos.fim->info.status == 'A') {
    printf(" prioridade: %d", voos.fim->info.prioridade);
  }
  printf("\n");
  return 0;
}

//==========FUNCÔES============

void aproximacoes_decolagens(int NVoos, int *NAproximacoes, int *NDecolagens){
  do {
    *NAproximacoes = rand()%33;
    *NDecolagens = NVoos - (*NAproximacoes);
  } while(*NAproximacoes < 10 || *NDecolagens < 10 || *NDecolagens > 32 || *NAproximacoes > NVoos);
}

Fila iniciar_avioes(int * NVoos, int *NAproximacoes, int *NDecolagens){

  do {
    *NVoos = rand()%65;
  } while(*NVoos < 20);


  //Iniciando NAproximacoes e NDecolagens
  aproximacoes_decolagens(*NVoos, NAproximacoes, NDecolagens);

  char * codigos[] = {"VG3001","JJ4404", "LN7001", "TG1501", "GL7602", "TT1010", "AZ1009", "AZ1008",
"AZ1010", "TG1506", "VG3002", "JJ4402", "GL7603", "RL7880", "AL0012",
"TT4544", "TG1505", "VG3003", "JJ4403", "JJ4401", "LN7002", "AZ1002",
"AZ1007", "GL7604", "AZ1006", "TG1503", "AZ1003", "JJ4403", "AZ1001",
"LN7003", "AZ1004", "TG1504", "AZ1005", "TG1502", "GL7601",
"TT4500", "RL7801", "JJ4410", "GL7607", "AL0029", "VV3390", "VV3392",
"GF4681", "GF4690", "AZ1020", "JJ4435", "VG3010", "LF0920", "AZ1065",
"LF0978", "RL7867", "TT4502", "GL7645", "LF0932", "JJ4434", "TG1510",
"TT1020", "AZ1098", "BA2312", "VG3030", "BA2304", "KL5609","KL5610", "KL5611"};

  int *random = (int *) malloc((*NVoos) * sizeof(int));

  bool primeiro_aviao = true;
  int status, contagem_decolagem = 0, contagem_aproximacao = 0;

  Fila voos;
  voos.inicio = (Lista *) malloc(sizeof(Lista));    //Para criar o primeiro elemento, o inicio e o fim apontarão para a mesma struct Lista
  voos.fim = voos.inicio;

  for (size_t i = 0; i < *NVoos; i++) {   //Iniciando Lista de avioes

    do{                                       //Deixando os codigos aleatórios sem repetir
      random[i] = rand()%64;
      for (size_t j = 0; j < i; j++) {
        if (random[j] == random[i]) {
          random[i] = -1;
        }
      }
    }while(random[i] == -1);

    if (i) {
      voos.fim->prox = novo_aviao();      //Se for a primeira vez no loop, o novo avião tem q ser colocado na msm posião em que está apontando
      voos.fim = voos.fim->prox;          //Se não, será alocado um novo avião e o fim apontará para ele
    }

    strcpy(voos.fim->info.codigo,codigos[random[i]]);   //Colocando informações no aviao

    status = rand()%2;
    if (status == DECOLAGEM && contagem_decolagem < *NDecolagens) {
        voos.fim->info.status = 'D';
        contagem_decolagem++;
    }
    else{
      if (contagem_aproximacao < *NAproximacoes) {
        voos.fim->info.status = 'A';
        voos.fim->info.prioridade = rand()%13;
        contagem_aproximacao++;
      }
      else{
        voos.fim->info.status = 'D';
        contagem_decolagem++;
      }
    }
    voos.fim->prox = NULL;
  }

  return voos;
}

Lista *novo_aviao(){
  Lista *novo_aviao = (Lista *) malloc(sizeof(Lista));
  return novo_aviao;
}
