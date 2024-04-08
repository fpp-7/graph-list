#include <stdio.h>
#include <stdlib.h>

#define true 1
#define false 0

#define BRANCO 0
#define AMARELO 1
#define VERMELHO 2

typedef int bool;
typedef int TIPOPESO;

// Estrutura com vértice final, peso da aresta e próximo vértice da lista ligada.
typedef struct adjacencia {
    int vertice;
    TIPOPESO peso;
    struct adjacencia *prox;
} ADJACENCIA;

// Estrutura que guarda toda a lista ligada, todos os nós aos quais se conectam.
typedef struct vertice {
    // Dados armazenados vão aqui
    ADJACENCIA *cab;
} VERTICE;

// Estrutura com vértices e arestas
typedef struct grafo {
    int vertices;
    int arestas;
    VERTICE *adj;
} GRAFO;

// Função para criar um grafo com um número específico de vértices
GRAFO *criaGrafo(int v) {
    // Aloca espaço para estrutura GRAFO
    GRAFO *g = (GRAFO *) malloc(sizeof(GRAFO));
    // Atualiza os valores dos vértices e das arestas na estrutura GRAFO
    g->vertices = v;
    g->arestas = 0;
    g->adj = (VERTICE *) malloc(v * sizeof(VERTICE));
    // Memória alocada setada com valor NULL
    int i;
    for (i = 0; i < v; i++) {
        g->adj[i].cab = NULL;
    }
    return g;
}

// Função para criar uma nova adjacência
ADJACENCIA *criaAdj(int v, int peso) {
    // Aloca espaço para um nó da lista de adjacências
    ADJACENCIA *temp = (ADJACENCIA *) malloc(sizeof(ADJACENCIA));
    // Vértice alvo da aresta
    temp->vertice = v;
    temp->peso = peso;
    temp->prox = NULL;
    return temp;
}

// Função para criar uma nova aresta entre dois vértices (não direcionado)
bool criaAresta(GRAFO *gr, int vi, int vf, TIPOPESO p) {
    // Testa os parâmetros
    if (!gr) return false;
    if ((vf < 0) || (vf >= gr->vertices)) return false;
    if ((vi < 0) || (vi >= gr->vertices)) return false;
    // Cria uma nova adjacência recebendo o vértice final e o peso
    ADJACENCIA *novo_vi = criaAdj(vf, p);
    // Adiciona a aresta na outra direção
    ADJACENCIA *novo_vf = criaAdj(vi, p);
    // O próximo recebe o vértice inicial
    novo_vi->prox = gr->adj[vi].cab;
    gr->adj[vi].cab = novo_vi;
    novo_vf->prox = gr->adj[vf].cab;
    gr->adj[vf].cab = novo_vf;
    gr->arestas++;
    return true;
}

void profundidade(GRAFO *g, int origem, int destino, int *encontrado) {
    int cor[g->vertices];
    int anterior[g->vertices];

    //Inicializa todos os vertices com o valor de BRANCO para indicar que não foram visitados
    for (int i = 0; i < g->vertices; i++) {
        cor[i] = BRANCO;
        // Inicializa o vetor de anteriores
        anterior[i] = -1;
    }

    //Verifica se é um caminho valido
    if (origem < 0 || origem >= g->vertices || destino < 0 || destino >= g->vertices) {
        printf("Vertice de origem ou destino invalido.\n");
        return;
    }

    visitaP(g, origem, destino, cor, anterior, encontrado);
}

void visitaP(GRAFO *g, int u, int destino, int *cor, int *anterior, int *encontrado) {
    // Pinta o vértice de amarelo para indicar o caminho sendo feito
    cor[u] = AMARELO;

    ADJACENCIA *v = g->adj[u].cab;
    while (v) {
        if (cor[v->vertice] == BRANCO) {
            anterior[v->vertice] = u;
            visitaP(g, v->vertice, destino, cor, anterior, encontrado);
        }
        v = v->prox;
    }
    //Pinta de vermelho indicando que já foi lido
    cor[u] = VERMELHO;

    // Se chegamos ao destino, construímos o caminho reverso
    if (u == destino) {
        printf("\nCaminho encontrado: ");
        *encontrado = 1;
        int caminho[100]; // Vetor para armazenar o caminho reverso
        int aux = 0;
        int atual = destino;

        // Constrói o caminho reverso usando o vetor 'anterior'
        while (atual != -1) {
            caminho[aux++] = atual;
            atual = anterior[atual];
        }

        // Imprime o caminho na ordem correta (da origem até o destino)
        for (int i = aux - 1; i >= 0; i--) {
            printf("v%d ", caminho[i]);
        }
        printf("\n");
    }
}

// Função para imprimir o grafo
void imprime(GRAFO *gr) {
    printf("\nRepresentacao do Grafo:\n");
    printf("Vertices: %d Arestas: %d. \n", gr->vertices, gr->arestas);
    int i;
    for (i = 0; i < gr->vertices; i++) {
       ADJACENCIA *ad = gr->adj[i].cab;
       printf("Vertice %d: ", i);
       while(ad) {
        printf("v%d(%d) ", ad->vertice, ad->peso);
        ad = ad->prox;
       }
       printf("\n");
    }
}

int main(void) {
   while(1) {
        int vertices, arestas, origem, destino;
        int encontrado = 0;

        printf("Digite o numero de vertices do grafo: ");
        scanf("%d", &vertices);
        printf("Digite o numero de arestas do grafo: ");
        scanf("%d", &arestas);

        GRAFO *gr = criaGrafo(vertices);

        // Solicita ao usuário as arestas do grafo
        printf("\nDigite as arestas do grafo (origem destino peso):\n");
        for (int i = 0; i < arestas; i++) {
            int origem, destino, peso;
            scanf("%d %d %d", &origem, &destino, &peso);
            criaAresta(gr, origem, destino, peso);
        }

        // Imprime a representação do grafo em lista
        imprime(gr);

        printf("\nDigite o vertice de origem para a busca em profundidade: ");
        scanf("%d", &origem);

        printf("\nDigite o vertice de destino para a busca em profundidade: ");
        scanf("%d", &destino);

        // Realiza a busca em profundidade a partir do vértice de origem até o vértice de destino
        profundidade(gr, origem, destino, &encontrado);

        // Caso não haja um caminho
        if (encontrado == 0) {
            printf("Nao existe um caminho de %d a %d no grafo.\n", origem, destino);
        }

        printf("\n");

        while(1) {
            printf("Pressione qualquer tecla para continuar...");
            getch();
            system("cls");
            break;
        }

    }
    return 0;
}
