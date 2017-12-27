//=====================================================
// Arquivo: main.c
//
// Autor: Guilherme Cardoso Silva 0022545
// Data: 15/11/2016
// Disciplina: AED II
// Atividade: Trabalho 2
//=====================================================

#include <stdlib.h>
#include <stdio.h>
#include "ArvoreExpressoes.h"

int main(){
  Info Entradas;
  FILE *arquivoSaida;
  char Expressao[1000];

  menu(&Entradas);
  if(Entradas.op2 == 1){
    arquivoSaida = stdout;
  }else{
    arquivoSaida = fopen(Entradas.arqSai, "wt");
    if(!arquivoSaida){
      printf("Erro ao criar o Arquivo");
      exit(EXIT_FAILURE);
    }
  }
  if(Entradas.op1 == 2){
    LeExpressoesArquivo(Entradas.arqEnt, arquivoSaida);
  }else{
    if(Entradas.op1 == 1){
      fgets(Expressao, sizeof(Expressao), stdin);
      LeExpressao(Expressao, arquivoSaida);
    }
  }
  fclose(arquivoSaida);
  return(0);
}
