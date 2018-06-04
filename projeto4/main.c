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
Lista *novo_aviao();                                                             //Heads
Fila iniciar_avioes(int * NVoos, int *NAproximacoes, int *NDecolagens);
bool verifica_emergencia(Fila *voos);
void diminui_tanque(Fila *voos);
void simular(Fila *voos);
void pedido_aceito(Fila *voos, int tempo, int pista);
void aviao_cai(Fila *voos);


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
  simular(&voos);
  printf("\n");
  return 0;
}

//==========FUNCÔES============

//FUNÇÃO 1
//Inicialiaza o numero de aproximacoes e decolagens
void aproximacoes_decolagens(int NVoos, int *NAproximacoes, int *NDecolagens){
  do {
    *NAproximacoes = rand()%33;
    *NDecolagens = NVoos - (*NAproximacoes);
  } while(*NAproximacoes < 10 || *NDecolagens < 10 || *NDecolagens > 32 || *NAproximacoes > NVoos);
}


//FUNÇÃO 2
//Aloca um novo aviao para a fila
Lista *novo_aviao(){
  Lista *novo_aviao = (Lista *) malloc(sizeof(Lista));
  return novo_aviao;
}

//FUNÇÃO 3
//Inicializa a fila de aviões para começar a contagem do tempo
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
"LN7003", "AZ1004", "TG1504", "AZ1005", "TG1502", "GL7601",                                 //Códigos dados
"TT4500", "RL7801", "JJ4410", "GL7607", "AL0029", "VV3390", "VV3392",
"GF4681", "GF4690", "AZ1020", "JJ4435", "VG3010", "LF0920", "AZ1065",
"LF0978", "RL7867", "TT4502", "GL7645", "LF0932", "JJ4434", "TG1510",
"TT1020", "AZ1098", "BA2312", "VG3030", "BA2304", "KL5609","KL5610", "KL5611"};

  int *random = (int *) malloc((*NVoos) * sizeof(int));

  int status, contagem_decolagem = 0, contagem_aproximacao = 0;

  Fila voos;
  voos.inicio = (Lista *) malloc(sizeof(Lista));    //Para criar o primeiro elemento, o inicio e o fim apontarão para a mesma struct Lista
  voos.fim = voos.inicio;

  for (size_t i = 0; i < *NVoos; i++) {   //Iniciando Fila de avioes

    do{
      random[i] = rand()%64;
      for (size_t j = 0; j < i; j++) {
        if (random[j] == random[i]) {         //Deixando os codigos aleatórios sem repetir
          random[i] = -1;
        }
      }
    }while(random[i] == -1);

    if (i) {
      voos.fim->prox = novo_aviao();      //Se for a primeira vez no loop, o novo avião tem q ser colocado na msm posião em que está apontando
      voos.fim = voos.fim->prox;          //Se não, será alocado um novo avião e o fim apontará para ele
    }

    strcpy(voos.fim->info.codigo,codigos[random[i]]);   //Colocando código no avião

    status = rand()%2;
    if (status == DECOLAGEM && contagem_decolagem < *NDecolagens) {
        voos.fim->info.status = 'D';
        contagem_decolagem++;
    }
    else{
      if (contagem_aproximacao < *NAproximacoes) {             //Deixando aleatório as aproximacoes e decolagens
        voos.fim->info.status = 'A';                           //De acordo com a quandide de cada uma
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

//FUNÇÃO 4
//Verifica se não tem alguma prioridade 0 e, caso tenha, passa para o inicio da fila
//Verifica se tem 3 ou mais com prioridade 0.
bool verifica_emergencia(Fila *voos){
  int emergencia = false;
  if (voos->inicio != NULL) {
      /* code */
    Lista *voo = voos->inicio->prox;
    Lista *voo_anterior = voos->inicio;
    while (voo != NULL && voo_anterior != NULL) {
      if (voo->info.status == 'A') {
        if (voo->info.prioridade == 0) {
          voo_anterior->prox = voo->prox;
          voo->prox = voos->inicio;
          voos->inicio = voo;
        }
      }
      voo_anterior = voo_anterior->prox;
      if (voo_anterior != NULL) {
        voo = voo_anterior->prox;
      }
    }
    //Agora vamos verificar se há emergencia
    if (voos->inicio->prox != NULL)
      if (voos->inicio->prox->prox != NULL) {
        voo = voos->inicio->prox->prox;    //colocando ponteiro para o terceiro da fila
        if (voo->info.prioridade == 0) {      //verificando se o terceiro da fila é 0
            emergencia = true;
            printf("ALERTA GERAL DE DESVIO DE AERONAVE\n");                //Se só o terceiro for 0, ele apenas abre a pista apenas de decolagem,                   //Se se o quarto também for 0, ele avisará que algum/alguns avião(ões) irá(ão) cair;
        }
      }
    }
    return emergencia;
}

//faz a simulação do aeroporto
//FUNÇÃO PRINCIPAL
void simular(Fila *voos){
  int tempo = 0;
  bool emergencia;
  bool pista1 = true, pista2 = true, pista3 = true;
  int tempo1, tempo2, tempo3;
  while (voos->inicio != NULL) {    //iniciando simulação
    if (!tempo) {
      emergencia = verifica_emergencia(voos);     //Coloca as prioridades 0 na frente e avisa se tem emergencia
    }

    //Se qualquer pista estiver liberada, ele entrará no if da pista para o pedido ser aceito;
    if (pista1) {
      if (voos->inicio->info.status == 'D')
        tempo1 = 2*UNTEMPO;
      else
        tempo1 = 3*UNTEMPO;
      pedido_aceito(voos, tempo, 1 );
      pista1 = false;                                          //Se for solicitação para decolagem, ele poderá pedir para as 3 pistas
    }

    if (pista2 && voos->inicio != NULL) {
      if (voos->inicio->info.status == 'D')
        tempo2 = 2*UNTEMPO;
      else
        tempo2 = 3*UNTEMPO;
      pedido_aceito(voos, tempo, 2 );
      pista2 = false;
    }
    if (!pista3 && emergencia) {
      aviao_cai(voos);
    }

    if (pista3 && voos->inicio != NULL) {
      if (emergencia){      //a pista 3 abre para pouso em caso de emergencia
        tempo3 = 3*UNTEMPO;
        pedido_aceito(voos, tempo, 3);
        pista3 = false;
        emergencia = false;
      }
      else if (voos->inicio->info.status == 'D'){
        tempo3 = 2*UNTEMPO;
        pedido_aceito(voos, tempo, 3);
        pista3 = false;
      }
    }

    //Esses próximos if fazem a contagem do tempo em cada pista e, quando a pista liberar, ela se torna true
    if (!pista1) {
      tempo1 -= UNTEMPO;
      if (!tempo1) {
        pista1 = true;
      }
    }
    if (!pista2) {
      tempo2 -= UNTEMPO;
      if (!tempo2) {
        pista2 = true;
      }
    }
    if (!pista3) {
      tempo3 -= UNTEMPO;
      if (!tempo3) {
        pista3 = true;
      }
    }

    //Esse if é para diminuir o tanque de tds os avioes a cada 10*UNTEMPO
    if (tempo%(10*UNTEMPO) == 0 && tempo != 0) {
      printf("DIMINUI TANQUE\n");
      diminui_tanque(voos);
      printf("Antes de entrar o EMERGENCIA\n");
      emergencia = verifica_emergencia(voos);     //Coloca as prioridades 0 na frente e avisa se tem emergencia
    }

    //tempo passa de 5 em 5 minutos a cada loop
    tempo += UNTEMPO;
  }
}

void pedido_aceito(Fila *voos, int tempo, int pista){
  Lista *pedido = voos->inicio;
  voos->inicio = voos->inicio->prox;

  printf("Código de Voo: %s\n", pedido->info.codigo);
  printf("Status: %s", pedido->info.status == 'D' ? "aeronave decolou" : "aeronave pousou");
  printf("\nHorário do início do procedimento: %d\n", tempo);
  printf("Número da pista: %d\n", pista);
  printf("\n");

  free(pedido);
}

void diminui_tanque(Fila *voos){
  Lista *tanque = voos->inicio;

  while (tanque != NULL) {
    tanque->info.prioridade--;
    tanque = tanque->prox;
  }
}

void aviao_cai(Fila *voos){
  Lista *queda = voos->inicio;
  voos->inicio = voos->inicio->prox;
  free(queda);
  printf("ALERTA CRÍTICO, AERONAVE IRÁ CAIR\n");
}
