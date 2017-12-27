//=====================================================
// Arquivo: ArvoreExpressoes.c
//
// Autor: Guilherme Cardoso Silva 0022545
// Data: 15/11/2016
// Disciplina: AED II
// Atividade: Trabalho 2
//=====================================================

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ArvoreExpressoes.h"

#define OPERANDO 1
#define OPERADOR 2

struct arvore{
  int Tipo;
  int Operando;
  int Operador;
  struct arvore *Esq;
  struct arvore *Dir;
};

struct nodoPilha{
  struct nodoPilha *Prox;
  struct arvore *Arvore;
};

struct pilha{
  struct nodoPilha *Inicio;
};

typedef struct nodoPilha *NodoPilha;

/*Funções Pilha*/
Pilha PilhaCria(){
  Pilha aux = (Pilha) malloc(sizeof(struct pilha));
  aux->Inicio = NULL;

  return(aux);
}

void PilhaInsere(Pilha PPilha, Arv PArvore){
  NodoPilha Nodo = (NodoPilha) malloc(sizeof(struct nodoPilha));

  Nodo->Arvore   = PArvore;
  Nodo->Prox     = PPilha->Inicio;
  PPilha->Inicio = Nodo;
}

Arv PilhaRetira(Pilha PPilha){
  NodoPilha Nodo;
  Arv PPArvore;

  Nodo = PPilha->Inicio;
  PPilha->Inicio = Nodo->Prox;
  PPArvore = Nodo->Arvore;
  free(Nodo);

  return(PPArvore);
}

int PilhaVazia(Pilha PPilha){
  if(!PPilha->Inicio){
    return(1);
  }else{
    return(0);
  }
}

void PilhaDestroi(Pilha PPilha){
  while(!PilhaVazia(PPilha)){
    PilhaRetira(PPilha);
  }
  free(PPilha);
}

/*Funções auxiliares*/
int PosiCaracter(char Expre[], char Caracter){
  /*Não foi necessario utilizar*/
  int i;
  /*Faço uma busca pela primeira occorencia de um determinado caracter em uma string*/
  for(i=0; i < strlen(Expre); i++){
    if(Expre[i] == Caracter){
      return(i);
    }
  }
  return(-1);
}

int VerificaOperador(char operador){
  /*Função de verificação de operador, caso seja um operador retorna 1*/
  if(operador == '+' || operador == '-' || operador == '*' || operador == '/'){
    return(1);
  }
  return(0);
}

/*Funções de Arvore*/
Arv ArvoreCria(Arv ArvEsq, Arv ArvDir, int Tipo, int Operando, char Operador){
  Arv Arvore = (Arv) malloc(sizeof(struct arvore));

  Arvore->Esq  = ArvEsq;
  Arvore->Dir  = ArvDir;
  Arvore->Tipo = Tipo;
  if(Tipo == OPERANDO){
    Arvore->Operando = Operando;
  }else{
    Arvore->Operador = Operador;
  }
  return(Arvore);
}

void  ArvoreDestroi(Arv Arvore){
  if(Arvore){
    ArvoreDestroi(Arvore->Esq);
    ArvoreDestroi(Arvore->Dir);
    free(Arvore);
  }
  Arvore = NULL;
}

void ImprimeResultados(Arv Arvore, FILE *arquivo){
  float Resultado;

  Resultado = ArvoreCalcula(Arvore);
  fprintf(arquivo, "O resultado da expressão é: %.2f\n\n", Resultado);

  fprintf(arquivo, "Impressão PosOrdem: \n");
  ArvoreImprimePosOrdem(Arvore, arquivo);

  fprintf(arquivo, "\nImpressão PreOrdem: \n");
  ArvoreImprimePreOrdem(Arvore, arquivo);

  fprintf(arquivo, "\nImpressão InOrdem: \n");
  ArvoreImprimeInOrdem(Arvore, arquivo);
  fprintf(arquivo, "\n");
}

void LeExpressao(char Expre[], FILE *arquivo){
  char *ExpreTemp;
  int Num1, Num2;
  char Simb;
  Pilha PPilha = PilhaCria();
  Arv Arvore;
  /*Os dois primeiros valores na expressão sao numeros*/
  sscanf(Expre,"%d %d",&Num1, &Num2);
  Arvore = ArvoreCria(NULL, NULL, OPERANDO, Num1, ' '); /*Cria um nó folha com o numero*/
  PilhaInsere(PPilha, Arvore);                          /*Insere na Pilha*/
  Arvore = ArvoreCria(NULL, NULL, OPERANDO, Num2, ' ');
  PilhaInsere(PPilha, Arvore);

  ExpreTemp = &Expre[2];  /*Posição do segundo numero*/
  while(strlen(ExpreTemp)>2){ /*Busca por mais termos na expressão*/
    ExpreTemp = &ExpreTemp[2];
    sscanf(ExpreTemp,"%c", &Simb);
    if(VerificaOperador(Simb)){ /*Se o valor pego é um simbolo cria uma nova arvore*/
      Arvore = PilhaRetira(PPilha);
      /*A arvore é criada com o primeiro valor retirado da pilha para o lado direito
        e o segundo valor retirado da pilha para o lado esquerdo*/
      Arvore = ArvoreCria(PilhaRetira(PPilha), Arvore, OPERADOR, 0, Simb);
      PilhaInsere(PPilha, Arvore);
    }else{                      /*Se o valor pego é um numero coloca na pilha*/
      Num1 = atoi(&Simb);
      Arvore = ArvoreCria(NULL, NULL, OPERANDO, Num1, ' ');
      PilhaInsere(PPilha, Arvore);
    }
  }

  Arvore = PilhaRetira(PPilha);
  PilhaDestroi(PPilha);

  ImprimeResultados(Arvore, arquivo);
  ArvoreDestroi(Arvore);
}

void LeExpressoesArquivo(char Arq[], FILE *arquivo){
  char Expressao[1000];
  FILE *arquivoLeitura;
  int cont = 0;

  arquivoLeitura = fopen(Arq, "r");
  if(arquivo){
    /*Enquanto todo o arquivo não for lido, realiza os calculos para cada expressão*/
    while(fgets(Expressao, sizeof(Expressao), arquivoLeitura)){
      if(strlen(Expressao)>3){
        cont++;
        fprintf(arquivo, "************ Expressao %d ************\n\n", cont);
        fprintf(arquivo, "Expressão: %s", Expressao);
        LeExpressao(Expressao, arquivo);
        fprintf(arquivo, "*********** ************* ***********\n");
      }
    }
    fclose(arquivoLeitura);
  }
}

void ArvoreImprimePosOrdem(Arv Arvore, FILE *arquivo){
  /*Impressão da expressão em PosOrdem*/
  if(Arvore){
    ArvoreImprimePosOrdem(Arvore->Esq, arquivo);
    ArvoreImprimePosOrdem(Arvore->Dir, arquivo);
    if(Arvore->Tipo == OPERANDO){
      fprintf(arquivo, "%d ", Arvore->Operando);
    }else{
      fprintf(arquivo, "%c ", Arvore->Operador);
    }
  }
}

void ArvoreImprimePreOrdem(Arv Arvore, FILE *arquivo){
  /*Impressão da expressão em PreOrdem*/
  if(Arvore){
    if(Arvore->Tipo == OPERANDO){
      fprintf(arquivo, "%d ", Arvore->Operando);
    }else{
      fprintf(arquivo, "%c ", Arvore->Operador);
    }
    ArvoreImprimePreOrdem(Arvore->Esq, arquivo);
    ArvoreImprimePreOrdem(Arvore->Dir, arquivo);
  }
}

void ImprimeParenteseEsquerdo(Arv Arvore, char parentese, FILE *arquivo){
  /*Parentização para arvores ramificadas para a esquerda*/
  if((Arvore->Tipo == OPERADOR && (Arvore->Operador == '*' || Arvore->Operador == '/')) &&
  (Arvore->Esq->Tipo == OPERADOR && (Arvore->Esq->Operador == '+' || Arvore->Esq->Operador == '-'))){
    fprintf(arquivo, "%c", parentese);
  }
}

void ImprimeParenteseDireito(Arv Arvore, char parentese, FILE *arquivo){
  /*Parentização para arvores ramificadas para a direita*/
  if((Arvore->Tipo == OPERADOR && (Arvore->Operador == '*' || Arvore->Operador == '/')) &&
  (Arvore->Dir->Tipo == OPERADOR && (Arvore->Dir->Operador == '+' || Arvore->Dir->Operador == '-'))){
    fprintf(arquivo, "%c", parentese);
  }
}

void ArvoreImprimeInOrdem(Arv Arvore, FILE *arquivo){
  /*Impressão da expressão em InOrdem*/
  if(Arvore){
    ImprimeParenteseEsquerdo(Arvore, '(',arquivo);
    ArvoreImprimeInOrdem(Arvore->Esq, arquivo);
    ImprimeParenteseEsquerdo(Arvore, ')',arquivo);
    if(Arvore->Tipo == OPERANDO){
      fprintf(arquivo, "%d ", Arvore->Operando);
    }else{
      fprintf(arquivo,  "%c ", Arvore->Operador);
    }
    ImprimeParenteseDireito(Arvore, '(',arquivo);
    ArvoreImprimeInOrdem(Arvore->Dir, arquivo);
    ImprimeParenteseDireito(Arvore, ')',arquivo);
  }
}

float ArvoreCalcula(Arv Arvore){
  float Resultado, Esq, Dir;
  /*Faz o calculo recursivamente dos resultado da expressão*/
  if(Arvore){
    Esq = ArvoreCalcula(Arvore->Esq); /*Busca o resultado das operações do lado esquerdo*/
    Dir = ArvoreCalcula(Arvore->Dir); /*Busca o resultado das operações do lado esquerdo*/

    if(Arvore->Tipo == OPERADOR){
      switch (Arvore->Operador){
         case '+':
           Resultado = Esq + Dir;
         break;
         case '-':
           Resultado = Esq - Dir;
         break;
         case '/':
           if(Dir != 0){
             Resultado = Esq / Dir;
           }else{
             printf("Não é possivel realizar uma divisão por 0.\n");
             exit(EXIT_FAILURE);
           }
         break;
         case '*':
           Resultado = Esq * Dir;
         break;
      }
    }else{
      Resultado = Arvore->Operando;
    }
    return(Resultado);
  }
  return(0);
}

void menu(Info* Entradas){
  char lixo;

  printf("Entrada de Dados\n");
  printf("1 - Entrada de Dados pelo Teclado\n");
  printf("2 - Entrada de Dados por Arquivo Texto\n");
  printf("Digite uma opção: ");
  scanf("%d", &Entradas->op1);
  scanf("%c",&lixo);
  if(Entradas->op1 == 2){
    printf("Nome do Arquivo de Entrada de Dados: ");
    scanf("%s", Entradas->arqEnt);
    scanf("%c",&lixo);
  }
  printf("Saída de Dados\n");
  printf("1 - Saida de Dados pelo Terminal\n");
  printf("2 - Saída de Dados por Arquivo Texto\n");
  printf("Digite uma opção: ");
  scanf("%d", &Entradas->op2);
  scanf("%c",&lixo);
  if(Entradas->op2 == 2){
    printf("Nome do Arquivo de Saida de Dados: ");
    scanf("%s", Entradas->arqSai);
    scanf("%c",&lixo);
  }
}
