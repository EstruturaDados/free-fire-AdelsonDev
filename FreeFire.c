#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // Necessário para usar clock() e CLOCKS_PER_SEC

#define MAX_COMP 20

// 1. ESTRUTURA DE DADOS OBRIGATÓRIA

typedef struct {
    char nome[30];
    char tipo[20];
    int prioridade;
} Componente;

// Variável global usada exclusivamente para contar comparações dentro dos algoritmos
int comparacoes_globais = 0;


// FUNÇÕES AUXILIARES DE ENTRADA E SAÍDA


// Limpa o caractere de nova linha '\n' deixado pelo fgets
void removerNovaLinha(char* str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

// Limpa o buffer do teclado ao misturar scanf com fgets
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Requisito: Exibir os componentes formatados
void mostrarComponentes(Componente vetor[], int tamanho) {
    printf("\n================ ESTOQUE DA TORRE DE FUGA ================\n");
    if (tamanho == 0) {
        printf(" Nenhum componente cadastrado ainda.\n");
    } else {
        printf("| %-22s | %-15s | %-10s |\n", "NOME DO COMPONENTE", "TIPO", "PRIORIDADE");
        printf("----------------------------------------------------------\n");
        for(int i = 0; i < tamanho; i++){
            printf("| %-22s | %-15s | %-10d |\n", vetor[i].nome, vetor[i].tipo, vetor[i].prioridade);
        }
    }
    printf(" \n\n");
}


// 2. ALGORITMOS DE ORDENAÇÃO


// Bubble Sort: Compara vizinhos e "flutua" o maior para o final. (Ordena por Nome)
void bubbleSortNome(Componente vetor[], int tamanho) {
    for (int i = 0; i < tamanho - 1; i++) {
        for (int j = 0; j < tamanho - i - 1; j++) {
            comparacoes_globais++; // Conta a comparação
            if (strcmp(vetor[j].nome, vetor[j + 1].nome) > 0) {
                Componente temp = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = temp;
            }
        }
    }
}

// Insertion Sort: Constrói a ordenação inserindo um item de cada vez na posição correta. (Ordena por Tipo)
void insertionSortTipo(Componente vetor[], int tamanho) {
    for (int i = 1; i < tamanho; i++) {
        Componente chave = vetor[i];
        int j = i - 1;
        
        // Move os elementos maiores que a chave para a direita
        while (j >= 0) {
            comparacoes_globais++; // Conta a comparação
            if (strcmp(vetor[j].tipo, chave.tipo) > 0) {
                vetor[j + 1] = vetor[j];
                j = j - 1;
            } else {
                break; // Se não for maior, para a busca para otimizar
            }
        }
        vetor[j + 1] = chave;
    }
}

// Selection Sort: Encontra o menor elemento e o coloca na primeira posição desordenada. (Ordena por Prioridade)
void selectionSortPrioridade(Componente vetor[], int tamanho) {
    for (int i = 0; i < tamanho - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < tamanho; j++) {
            comparacoes_globais++; // Conta a comparação
            if (vetor[j].prioridade < vetor[min_idx].prioridade) {
                min_idx = j;
            }
        }
        // Troca o menor elemento encontrado com o primeiro elemento da parte não ordenada
        if (min_idx != i) {
            Componente temp = vetor[i];
            vetor[i] = vetor[min_idx];
            vetor[min_idx] = temp;
        }
    }
}


// 3. MEDIÇÃO E BUSCA


// Requisito: Função ponteiro para rodar qualquer algoritmo de ordenação e medir seu tempo
void medirTempo(void (*algoritmo)(Componente[], int), Componente vetor[], int tamanho) {
    if (tamanho <= 1) {
        printf("\n[AVISO] Cadastre pelo menos 2 componentes para ordenar.\n");
        return;
    }

    comparacoes_globais = 0; // Zera o contador global antes de começar
    
    // Captura o tempo antes de rodar o algoritmo
    clock_t inicio = clock();
    
    // Executa a função de ordenação recebida por parâmetro
    algoritmo(vetor, tamanho);
    
    // Captura o tempo assim que o algoritmo termina
    clock_t fim = clock();
    
    // Calcula o tempo em segundos
    double tempoExecucao = (double)(fim - inicio) / CLOCKS_PER_SEC;
    
    printf("\n[ANALISE DE SISTEMA CONCLUIDA]\n");
    printf("-> Comparacoes realizadas: %d\n", comparacoes_globais);
    // Usamos %f para mostrar frações de segundo (ex: 0.000003 s)
    printf("-> Tempo de execucao: %f segundos\n", tempoExecucao);
}

// Requisito: Busca Binária otimizada (Só pode rodar se estiver ordenado por Nome)
void buscaBinariaPorNome(Componente vetor[], int tamanho, char nomeBuscado[]) {
    int esquerda = 0, direita = tamanho - 1;
    int comparacoes = 0;
    
    while (esquerda <= direita) {
        comparacoes++;
        int meio = esquerda + (direita - esquerda) / 2;
        int cmp = strcmp(vetor[meio].nome, nomeBuscado);
        
        if (cmp == 0) {
            printf("\n[ACESSO LIBERADO] Componente-chave localizado!\n");
            printf("Nome: %s | Tipo: %s | Prioridade: %d\n", vetor[meio].nome, vetor[meio].tipo, vetor[meio].prioridade);
            printf("-> Custo da busca: Apenas %d comparacoes!\n", comparacoes);
            return;
        }
        if (cmp < 0) esquerda = meio + 1;
        else direita = meio - 1;
    }
    
    printf("\n[FALHA] Componente '%s' nao encontrado na torre. (Comparacoes: %d)\n", nomeBuscado, comparacoes);
}


// 4. FLUXO PRINCIPAL DO JOGO (MENU)

int main() {
    Componente estoque[MAX_COMP];
    int totalComp = 0;
    int ordenadoPorNome = 0; // Flag de segurança para habilitar a busca binária
    int opcao;

    printf("     \n");
    printf(" SAFE ZONE FINAL: SISTEMA DE MONTAGEM DA TORRE\n");
    printf("      \n");

    do {
        printf("\n   TERMINAL DE ENGENHARIA  \n");
        printf("1. Cadastrar Componente (%d/%d)\n", totalComp, MAX_COMP);
        printf("2. Estrategia 1: Ordenar por Nome (Bubble Sort)\n");
        printf("3. Estrategia 2: Ordenar por Tipo (Insertion Sort)\n");
        printf("4. Estrategia 3: Ordenar por Prioridade (Selection Sort)\n");
        printf("5. Localizar Componente-Chave (Busca Binaria)\n");
        printf("6. Visualizar Status da Montagem\n");
        printf("7. Ativar Torre de Resgate e Sair\n");
        printf("              \n");
        printf("Comando: ");
        
        scanf("%d", &opcao);
        limparBuffer(); // Extremamente importante antes do fgets

        switch (opcao) {
            case 1:
                if (totalComp >= MAX_COMP) {
                    printf("\n[ERRO] Limite de componentes atingido!\n");
                    break;
                }
                printf("\n--- Novo Componente ---\n");
                
                printf("Nome do componente: ");
                fgets(estoque[totalComp].nome, 30, stdin);
                removerNovaLinha(estoque[totalComp].nome);
                
                printf("Tipo (ex: suporte, controle): ");
                fgets(estoque[totalComp].tipo, 20, stdin);
                removerNovaLinha(estoque[totalComp].tipo);
                
                printf("Nivel de prioridade (1 a 10): ");
                scanf("%d", &estoque[totalComp].prioridade);
                limparBuffer();
                
                totalComp++;
                ordenadoPorNome = 0; // Se um novo item entrou, bagunçou a ordem
                printf("\n[SUCESSO] Componente registrado!\n");
                break;

            case 2:
                printf("\n[SISTEMA] Iniciando Bubble Sort (Por Nome)...");
                // Passamos o nome da função como parâmetro para medirTempo!
                medirTempo(bubbleSortNome, estoque, totalComp);
                ordenadoPorNome = 1; // Habilita a busca binária
                mostrarComponentes(estoque, totalComp);
                break;

            case 3:
                printf("\n[SISTEMA] Iniciando Insertion Sort (Por Tipo)...");
                medirTempo(insertionSortTipo, estoque, totalComp);
                ordenadoPorNome = 0; // Desabilita a busca binária
                mostrarComponentes(estoque, totalComp);
                break;

            case 4:
                printf("\n[SISTEMA] Iniciando Selection Sort (Por Prioridade)...");
                medirTempo(selectionSortPrioridade, estoque, totalComp);
                ordenadoPorNome = 0; // Desabilita a busca binária
                mostrarComponentes(estoque, totalComp);
                break;

            case 5:
                if (ordenadoPorNome == 0) {
                    printf("\n[ALERTA FATAL] A Busca Binaria so funciona se os componentes estiverem ORDENADOS POR NOME.\n");
                    printf("Execute a Opcao 2 (Estrategia 1) antes de realizar esta busca!\n");
                } else {
                    char chave[30];
                    printf("\nDigite o nome exato do componente-chave que ira destravar a torre: ");
                    fgets(chave, 30, stdin);
                    removerNovaLinha(chave);
                    buscaBinariaPorNome(estoque, totalComp, chave);
                }
                break;

            case 6:
                mostrarComponentes(estoque, totalComp);
                break;

            case 7:
                printf("\n[TORRE ATIVADA] Resgate a caminho! Parabens por sobreviver a ilha.\n");
                break;

            default:
                printf("\n[ERRO] Comando nao reconhecido.\n");
        }
    } while (opcao != 7);

    return 0;
}