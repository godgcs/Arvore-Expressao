//=====================================================
// Arquivo: ArvoreExpressoes.h
//
// Autor: Guilherme Cardoso Silva 0022545
// Data: 15/11/2016
// Disciplina: AED II
// Atividade: Trabalho 2
//=====================================================

#ifndef _ArvoreExpressoes_XxX
#define _ArvoreExpressoes_XxX

struct info{
  int op1;
  int op2;
  char arqEnt[255];
  char arqSai[255];
};

typedef struct info Info;
typedef struct arvore *Arv;
typedef struct pilha *Pilha;

// Declarações de Funções Publicas
void menu(Info* Entradas);
void ImprimeResultados(Arv Arvore, FILE *arquivo);
void LeExpressao(char Expre[], FILE *arquivo);
void LeExpressoesArquivo(char Arq[], FILE *arquivo);
//Funções Gerais de Leitura de Expressões
int PosiCaracter(char Expre[], char Caracter);
int VerificaOperador(char operador);
//Funções Pilha
Pilha PilhaCria();
void  PilhaInsere(Pilha PPilha, Arv PArvore);
void  PilhaDestroi(Pilha PPilha);
Arv   PilhaRetira(Pilha PPilha);
int   PilhaVazia(Pilha PPilha);
//Funções Arvore de Expressões
Arv   ArvoreCria(Arv ArvEsq, Arv ArvDir, int Tipo, int Operando, char Operador);
void  ImprimeParenteseEsquerdo(Arv Arvore, char parentese, FILE *arquivo);
void  ImprimeParenteseDireito(Arv Arvore, char parentese, FILE *arquivo);
void  ArvoreImprimePosOrdem(Arv Arvore, FILE *arquivo);
void  ArvoreImprimePreOrdem(Arv Arvore, FILE *arquivo);
void  ArvoreImprimeInOrdem(Arv Arvore, FILE *arquivo);
float ArvoreCalcula(Arv Arvore);
void  ArvoreDestroi(Arv Arvore);

#endif
