#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_ITENS 10

// ESTRUTURA DE DADOS

typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

// FUNÇÕES AUXILIARES limpa buffer

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// FUNÇÕES DO SISTEMA

// Lista todos os itens atuais da mochila

void listarItens(Item mochila[], int totalItens) {
    printf("\n==================== INVENTARIO DA MOCHILA ====================\n");
    if (totalItens == 0) {
        printf(" A mochila esta vazia no momento.\n");
    } else {
        printf("| %-4s | %-25s | %-15s | %-10s |\n", "ID", "NOME DO ITEM", "TIPO", "QUANTIDADE");
        printf("-----------------------------------------------------------------\n");
        for (int i = 0; i < totalItens; i++) {
            printf("| %-4d | %-25s | %-15s | %-10d |\n", 
                   i + 1, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
        }
    }
    printf("=================================================================\n\n");
}

// Insere um novo item na mochila

void inserirItem(Item mochila[], int *totalItens) {
    if (*totalItens >= MAX_ITENS) {
        printf("\n[AVISO] A mochila esta cheia! Nao e possivel carregar mais itens.\n");
        return;
    }

    printf("\n--- Adicionar Novo Item ---\n");
    
    printf("Nome do item: ");
    scanf(" %29[^\n]", mochila[*totalItens].nome); 
    limparBuffer();

    printf("Tipo (ex: arma, municao, cura): ");
    scanf(" %19[^\n]", mochila[*totalItens].tipo);
    limparBuffer();

    printf("Quantidade: ");
    scanf("%d", &mochila[*totalItens].quantidade);
    limparBuffer();

    (*totalItens)++; 
    
    printf("\n[SUCESSO] '%s' foi adicionado a mochila!\n", mochila[*totalItens - 1].nome);
    
    listarItens(mochila, *totalItens);
}

// Remove um item buscando pelo seu nome exato

void removerItem(Item mochila[], int *totalItens) {
    if (*totalItens == 0) {
        printf("\n[AVISO] A mochila ja esta vazia.\n");
        return;
    }

    char nomeBuscado[30];
    printf("\n--- Descartar Item ---\n");
    printf("Digite o nome EXATO do item que deseja remover: ");
    scanf(" %29[^\n]", nomeBuscado);
    limparBuffer();

    int indiceEncontrado = -1;

    for (int i = 0; i < *totalItens; i++) {
        if (strcmp(mochila[i].nome, nomeBuscado) == 0) {
            indiceEncontrado = i;
            break;
        }
    }

    if (indiceEncontrado != -1) {
        for (int i = indiceEncontrado; i < *totalItens - 1; i++) {
            mochila[i] = mochila[i + 1];
        }
        (*totalItens)--;
        printf("\n[SUCESSO] O item '%s' foi removido da mochila.\n", nomeBuscado);
    } else {
        printf("\n[ERRO] O item '%s' nao foi encontrado na mochila.\n", nomeBuscado);
    }

    listarItens(mochila, *totalItens);
}


int main() {
    Item mochila[MAX_ITENS]; 
    int totalItens = 0; 
    int opcao;

    printf("Iniciando sistema de inventario...\n");

    do {
        printf("========== MENU DE LOOT ==========\n");
        printf("1. Adicionar Item (Inserir)\n");
        printf("2. Descartar Item (Remover)\n");
        printf("3. Abrir Mochila (Listar Todos)\n");
        printf("4. Fechar Inventario (Sair)\n");
        printf("==================================\n");
        printf("Escolha uma acao (1-4): ");
        
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                inserirItem(mochila, &totalItens);
                break;
            case 2:
                removerItem(mochila, &totalItens);
                break;
            case 3:
                listarItens(mochila, totalItens);
                break;
            case 4:
                printf("\nFechando inventario. Voltando ao jogo...\n");
                break;
            default:
                printf("\n[ERRO] Comando invalido! Tente novamente.\n\n");
        }
    } while (opcao != 4);

    return 0;
}
