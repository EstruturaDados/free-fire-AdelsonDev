#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITENS 10

// 1. ESTRUTURAS DE DADOS

typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

typedef struct No {
    Item dados;
    struct No* proximo;
} No;

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


// 2. MOTOR FÍSICO 1: VETOR (Com Ordenação Automática)


// Requisito: Ordenação (Oculta do jogador, roda após inserir)
void ordenarVetor(Item vetor[], int totalItens) {
    for (int i = 0; i < totalItens - 1; i++) {
        for (int j = 0; j < totalItens - i - 1; j++) {
            if (strcmp(vetor[j].nome, vetor[j + 1].nome) > 0) {
                Item temp = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = temp;
            }
        }
    }
}

void inserirItemVetor(Item vetor[], int* totalItens) {
    if (*totalItens >= MAX_ITENS) {
        printf("\n[AVISO] A mochila esta cheia! Nao e possivel carregar mais itens.\n");
        return;
    }
    printf("\n--- Coletar Novo Item ---\n");
    printf("Nome do item: "); scanf(" %29[^\n]", vetor[*totalItens].nome); limparBuffer();
    printf("Tipo (ex: arma, cura): "); scanf(" %19[^\n]", vetor[*totalItens].tipo); limparBuffer();
    printf("Quantidade: "); scanf("%d", &vetor[*totalItens].quantidade); limparBuffer();
    
    (*totalItens)++;
    ordenarVetor(vetor, *totalItens); // Organiza silenciosamente
    
    printf("\n[SUCESSO] '%s' foi adicionado e guardado na mochila!\n", vetor[*totalItens - 1].nome);
}

// Requisito: Busca Binária (Oculta do jogador, roda ao tentar remover)
void removerItemVetor(Item vetor[], int* totalItens) {
    if (*totalItens == 0) { printf("\n[AVISO] A mochila ja esta vazia.\n"); return; }
    
    char nome[30];
    printf("\n--- Descartar Item ---\n");
    printf("Digite o nome EXATO do item: "); scanf(" %29[^\n]", nome); limparBuffer();
    
    // Busca Binária
    int esquerda = 0, direita = *totalItens - 1, meio, indiceEncontrado = -1, comparacoes = 0;
    while (esquerda <= direita) {
        comparacoes++;
        meio = esquerda + (direita - esquerda) / 2;
        int cmp = strcmp(vetor[meio].nome, nome);
        
        if (cmp == 0) { indiceEncontrado = meio; break; }
        if (cmp < 0) esquerda = meio + 1; 
        else direita = meio - 1;          
    }

    // Exibe a métrica de performance exigida pelo Nível 2
    printf("\n[SISTEMA] Busca Binaria localizou o item em %d comparacoes.\n", comparacoes);

    if (indiceEncontrado != -1) {
        for (int i = indiceEncontrado; i < *totalItens - 1; i++) {
            vetor[i] = vetor[i + 1];
        }
        (*totalItens)--;
        printf("[SUCESSO] O item '%s' foi removido.\n", nome);
    } else {
        printf("[ERRO] Item nao encontrado.\n");
    }
}

void listarItensVetor(Item vetor[], int totalItens) {
    printf("\n   INVENTARIO DA MOCHILA   \n");
    if (totalItens == 0) {
        printf(" A mochila esta vazia no momento.\n");
    } else {
        printf("| %-4s | %-25s | %-15s | %-10s |\n", "ID", "NOME DO ITEM", "TIPO", "QTD");
        printf("-----------------------------------------------------------------\n");
        for (int i = 0; i < totalItens; i++) {
            printf("| %-4d | %-25s | %-15s | %-10d |\n", 
                   i + 1, vetor[i].nome, vetor[i].tipo, vetor[i].quantidade);
        }
    }
    printf("-------------------\n\n");
}


// 3. MOTOR FÍSICO 2: LISTA ENCADEADA DINÂMICA


void inserirItemLista(No** cabeca) {
    No* novoNo = (No*)malloc(sizeof(No));
    if (novoNo == NULL) { printf("\nErro critico de memoria.\n"); return; }
    
    printf("\n--- Coletar Novo Item ---\n");
    printf("Nome do item: "); scanf(" %29[^\n]", novoNo->dados.nome); limparBuffer();
    printf("Tipo (ex: arma, cura): "); scanf(" %19[^\n]", novoNo->dados.tipo); limparBuffer();
    printf("Quantidade: "); scanf("%d", &novoNo->dados.quantidade); limparBuffer();
    novoNo->proximo = NULL;

    if (*cabeca == NULL) {
        *cabeca = novoNo;
    } else {
        No* atual = *cabeca;
        while (atual->proximo != NULL) {
            atual = atual->proximo;
        }
        atual->proximo = novoNo;
    }
    printf("\n[SUCESSO] '%s' foi adicionado a mochila!\n", novoNo->dados.nome);
}

// Requisito: Busca Sequencial (Oculta do jogador, roda ao tentar remover)
void removerItemLista(No** cabeca) {
    if (*cabeca == NULL) { printf("\n[AVISO] A mochila ja esta vazia.\n"); return; }
    
    char nome[30];
    printf("\n--- Descartar Item ---\n");
    printf("Digite o nome EXATO do item: "); scanf(" %29[^\n]", nome); limparBuffer();
    
    No* atual = *cabeca;
    No* anterior = NULL;
    int comparacoes = 0;
    
    // Busca Sequencial
    while (atual != NULL) {
        comparacoes++;
        if (strcmp(atual->dados.nome, nome) == 0) break; // Achou!
        anterior = atual;
        atual = atual->proximo;
    }
    
    // Exibe a métrica de performance exigida pelo Nível 2
    printf("\n[SISTEMA] Busca Sequencial localizou o item em %d comparacoes.\n", comparacoes);

    if (atual == NULL) {
        printf("[ERRO] Item nao encontrado.\n");
        return;
    }
    
    if (anterior == NULL) {
        *cabeca = atual->proximo;
    } else {
        anterior->proximo = atual->proximo;
    }
    
    free(atual);
    printf("[SUCESSO] O item '%s' foi removido.\n", nome);
}

void listarItensLista(No* cabeca) {
    printf("\n   INVENTARIO DA MOCHILA  \n");
    if (cabeca == NULL) {
        printf(" A mochila esta vazia no momento.\n");
    } else {
        printf("| %-4s | %-25s | %-15s | %-10s |\n", "ID", "NOME DO ITEM", "TIPO", "QTD");
        printf("-----------------------------------------------------------------\n");
        No* atual = cabeca;
        int i = 1;
        while (atual != NULL) {
            printf("| %-4d | %-25s | %-15s | %-10d |\n", 
                   i++, atual->dados.nome, atual->dados.tipo, atual->dados.quantidade);
            atual = atual->proximo;
        }
    }
    printf("-------------------------------------------------\n\n");
}

void liberarMemoriaLista(No** cabeca) {
    No* atual = *cabeca;
    No* proximo;
    while (atual != NULL) {
        proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    *cabeca = NULL;
}


// 4. FLUXO PRINCIPAL COM INTERFACE LIMPA (Idêntica ao Nível 1)

int main() {
    int motorFisico;

    printf("SISTEMA DE SOBREVIVENCIA - SETUP INICIAL\n");
    printf("[1] Usar Motor de Vetor\n");
    printf("[2] Usar Motor de Lista Encadeada\n");
    printf("Escolha: ");
    scanf("%d", &motorFisico); limparBuffer();

    int opcao;

    // Motor Vetor
    if (motorFisico == 1) {
        Item mochilaVetor[MAX_ITENS];
        int totalVetor = 0;

        do {
            printf("\n  MENU DE LOOT \n");
            printf("1. Coletar Item (Inserir)\n");
            printf("2. Descartar Item (Remover)\n");
            printf("3. Abrir Mochila (Listar Todos)\n");
            printf("4. Fechar Inventario (Sair)\n");
            printf("----------------------\n");
            printf("Escolha uma acao (1-4): ");
            scanf("%d", &opcao); limparBuffer();

            switch (opcao) {
                case 1: inserirItemVetor(mochilaVetor, &totalVetor); break;
                case 2: removerItemVetor(mochilaVetor, &totalVetor); break;
                case 3: listarItensVetor(mochilaVetor, totalVetor); break;
                case 4: printf("\nFechando inventario...\n"); break;
                default: printf("\n[ERRO] Comando invalido!\n");
            }
        } while (opcao != 4);
    } 
    // Motor Lista Encadeada
    else if (motorFisico == 2) {
        No* mochilaLista = NULL;

        do {
            printf("\n     MENU DE LOOT     \n");
            printf("1. Coletar Item (Inserir)\n");
            printf("2. Descartar Item (Remover)\n");
            printf("3. Abrir Mochila (Listar Todos)\n");
            printf("4. Fechar Inventario (Sair)\n");
            printf("---------------------------\n");
            printf("Escolha uma acao (1-4): ");
            scanf("%d", &opcao); limparBuffer();

            switch (opcao) {
                case 1: inserirItemLista(&mochilaLista); break;
                case 2: removerItemLista(&mochilaLista); break;
                case 3: listarItensLista(mochilaLista); break;
                case 4: printf("\nFechando inventario...\n"); break;
                default: printf("\n[ERRO] Comando invalido!\n");
            }
        } while (opcao != 4);

        liberarMemoriaLista(&mochilaLista);
    }

    return 0;
}