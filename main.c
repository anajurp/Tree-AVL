#include <stdio.h>

// Implementar uma árvore binária de pesquisa auto-balanceável, do tipo AVL, com
// suporte as operações de inserção, exclusão e pesquisa.

typedef struct no {
  int valor;
  struct no *esquerdo, *direito;
  short altura; // short usa menos memoria nesse caso - pode usar INT mas nesse
                // caso n precisa
} No;

// função para criar um novo nó
No *novoNo(int valor_novo) {
  No *novo = malloc(sizeof(No));

  if (novo) {
    novo->valor = valor_novo;
    novo->esquerdo = NULL;
    novo->direito = NULL;
    novo->altura = 0; // a altura de um no folha smepre vai ser 0
  } else
    printf("\n Erro ao aolocar nó em novoNo");
  return novo;
}

// maior subarvore
short maiorSubarvore(short a, short b) {
  return (a > b) ? a : b; // se a for maior q b retorna a se n retorna b
}

// altura do no
short alturaDONo(No *no) {
  if (no == NULL)
    return -1;
  else
    return no->altura;
}

// fator de balancemaento de um no 1, -1 e 0 são aceitaveis
short fatorDebalanceamento(No *no) {
  if (no)
    return (alturaDONo(no->esquerdo) - alturaDONo(no->direito));
  else
    return 0;
}

// rotação a esquerda
No *rotacaoEsquerda(No *r) {
  No *y, *f;
  y = r->direito;
  f = y->esquerdo;

  y->esquerdo = r;
  r->direito = f;

  r->altura =
      maiorSubarvore(alturaDONo(r->esquerdo), alturaDONo(r->direito)) + 1;
  y->altura =
      maiorSubarvore(alturaDONo(y->esquerdo), alturaDONo(y->direito)) + 1;

  return y;
}

// rotação a direita
No *rotacaoDireita(No *r) {
  No *y, *f;

  y = r->esquerdo;
  f = y->direito;

  y->direito = r;
  r->esquerdo = f;

  r->altura =
      maiorSubarvore(alturaDONo(r->esquerdo), alturaDONo(r->direito)) + 1;
  y->altura =
      maiorSubarvore(alturaDONo(y->esquerdo), alturaDONo(y->direito)) + 1;

  return y;
}

// rotações DUPLAS
No *rotacaoDireitaEsquerda(No *r) {
  r->direito = rotacaoDireita(r->direito);
  return rotacaoEsquerda(r);
}

No *rotacaoEsquerdaDireita(No *r) {
  r->esquerdo = rotacaoEsquerda(r->esquerdo);
  return rotacaoDireita(r);
}

// balancelar
No *balancelar(No *r) {
  short fb = fatorDebalanceamento(r);

  // rotacao esquerda
  if (fb < -1 && fatorDebalanceamento(r->direito) <= 0)
    r = rotacaoEsquerda(r);

  // rotacao direita
  else if (fb > 1 && fatorDebalanceamento(r->esquerdo) >= 0)
    r = rotacaoDireita(r);

  // rotadao dupla esquerda
  else if (fb > 1 && fatorDebalanceamento(r->esquerdo) < 0)
    r = rotacaoEsquerdaDireita(r);

  // rotacao dupla direita
  else if (fb < -1 && fatorDebalanceamento(r->direito) > 0)
    r = rotacaoDireitaEsquerda(r);

  return r;
}

// isere um no
No *inserirNo(No *raiz, int x) {
  if (raiz == NULL)
    return novoNo(x);

  else {
    // insere na esquerda
    if (x < raiz->valor)
      raiz->esquerdo = inserirNo(raiz->esquerdo, x);

    // insere na direita
    else if (x > raiz->valor)
      raiz->direito = inserirNo(raiz->direito, x);

    // n é um elemento real
    else
      printf("\n Inserção não realizada");
  }

  // recalcula altura
  raiz->altura =
      maiorSubarvore(alturaDONo(raiz->esquerdo), alturaDONo(raiz->direito)) + 1;

  // verifica balanceamento
  raiz = balancelar(raiz);
  return raiz;
}

// remover no
No *remover(No *raiz, int chave) {
  if (raiz == NULL) {
    printf("Valor não encontrado");
    return NULL;
  } else { // procura nó
    if (raiz->valor == chave) {
      // no sem filhos
      if (raiz->esquerdo == NULL && raiz->direito == NULL) {
        free(raiz);
        printf("Elemento folha removido");
        return NULL;
      } else {
        // no com dois filhos
        if (raiz->esquerdo != NULL && raiz->direito != NULL) {
          No *aux = raiz->esquerdo;
          while (aux->direito != NULL)
            aux = aux->direito;
          raiz->valor = aux->valor;
          aux->valor = chave;
          printf("Elemento trocado");
          raiz->esquerdo = remover(raiz->esquerdo, chave);
          return raiz;
        } else {
          // remove no com um filho
          No *aux;
          if (raiz->esquerdo != NULL)
            aux = raiz->esquerdo;
          else
            aux = raiz->direito;
          free(raiz);
          printf("Elemento com um filho removido");
          return aux;
        }
      }
    } else {
      if (chave < raiz->valor)
        raiz->esquerdo = remover(raiz->esquerdo, chave);
      else
        raiz->direito = remover(raiz->direito, chave);
    }
    // recalcula altura
    raiz->altura =
        maiorSubarvore(alturaDONo(raiz->esquerdo), alturaDONo(raiz->direito)) +
        1;

    // rebalanceamento?
    raiz = balancelar(raiz);
    return raiz;
  }
}
// Imprime
void imprimir(No *raiz, int nivel) {
  int i;
  if (raiz) {
    imprimir(raiz->direito, nivel + 1);
    printf("\n \n");

    for (i = 0; i < nivel; i++)
      printf("\t");
    printf("%d", raiz->valor);
    imprimir(raiz->esquerdo, nivel + 1);
  }
}

// pesquisa
void pesquisa(No *raiz, int pesq) {
  if (raiz != NULL) {
    if (raiz->valor == pesq) {
      printf("Nodo encontrado %d. \n", raiz->valor);
    } else {
      if (pesq > raiz->valor) {
        printf("Pesquisando a direita. \n");
        pesquisa(raiz->direito, pesq);
      } else {
        printf("Pesquisando a esquerda. \n");
        pesquisa(raiz->esquerdo, pesq);
      }
    }
  } else {
    printf("Nodo nao encontrado\n");
  }
}

int main(void) {

  int opcao, valor;
  No *raiz = NULL;

  do {
    printf(" \n");
    printf("0 - Sair \n");
    printf("1 - Inserir \n");
    printf("2 - Remover \n");
    printf("3 - Imprimir \n");
    printf("4 - Procurar \n");
    scanf("%d", &opcao);

    switch (opcao) {
    case 0:
      printf("Obrigada! Até mais");
      break;
    case 1:
      printf("\t Digite o valor a ser inserido: ");
      scanf("%d", &valor);
      raiz = inserirNo(raiz, valor);
      break;
    case 2:
      printf("\t Digite o valor a ser removido: ");
      scanf("%d", &valor);
      raiz = remover(raiz, valor);
      break;
    case 3:
      imprimir(raiz, 1);
      break;
    case 4:
      printf("\t Digite o valor a ser procurado: ");
      scanf("%d", &valor);
      pesquisa(raiz, valor);
      break;
    default:
      printf("Opcao invalida :(");
    }

  } while (opcao != 0);

  return 0;
}