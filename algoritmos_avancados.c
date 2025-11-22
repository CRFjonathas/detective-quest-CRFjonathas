#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.

// --- Estruturas ---

// Estrutura da Árvore de Busca para Pistas
typedef struct PistaNode {
    char texto[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// Estrutura da Árvore Binária do Mapa (agora com pista opcional)
typedef struct Sala {
    char nome[50];
    char pista[100]; // Se vazio, sala não tem pista
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// --- Funções Auxiliares ---

Sala* criarSala(char *nome, char *pista) {
    Sala *novaSala = (Sala*) malloc(sizeof(Sala));
    if (novaSala != NULL) {
        strcpy(novaSala->nome, nome);
        if (pista != NULL) strcpy(novaSala->pista, pista);
        else strcpy(novaSala->pista, "");
        novaSala->esquerda = NULL;
        novaSala->direita = NULL;
    }
    return novaSala;
}

// Documentação: Insere uma nova pista na árvore BST em ordem alfabética.
PistaNode* inserirPista(PistaNode *raiz, char *texto) {
    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(novo->texto, texto);
        novo->esquerda = NULL;
        novo->direita = NULL;
        printf(">> Pista coletada: '%s'\n", texto);
        return novo;
    }
    
    if (strcmp(texto, raiz->texto) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, texto);
    } else if (strcmp(texto, raiz->texto) > 0) {
        raiz->direita = inserirPista(raiz->direita, texto);
    }
    // Se for igual, não insere duplicado
    return raiz;
}

// Documentação: Imprime a árvore de pistas em ordem alfabética (em-ordem).
void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->texto);
        exibirPistas(raiz->direita);
    }
}

// Documentação: Controla a navegação e coleta de pistas.
// Passamos o ponteiro da raiz de pistas por referência para atualizá-lo
void explorarSalasComPistas(Sala *salaAtual, PistaNode **raizPistas) {
    if (salaAtual == NULL) return;

    char opcao;
    while (1) {
        printf("\n--- Local: %s ---\n", salaAtual->nome);

        // Coleta automática de pista se houver e ainda não tiver sido pega (simplificado aqui para sempre tentar inserir)
        if (strlen(salaAtual->pista) > 0) {
            printf("Você encontrou algo aqui!\n");
            *raizPistas = inserirPista(*raizPistas, salaAtual->pista);
            // Para evitar spam, poderíamos limpar a pista da sala, mas o requisito diz "estático" no mapa.
            // O `inserirPista` trata duplicatas simples.
        }

        printf("Opções: [e]squerda, [d]ireita, [s]air: ");
        scanf(" %c", &opcao);

        if (opcao == 's') return;
        
        if (opcao == 'e' && salaAtual->esquerda != NULL) {
            explorarSalasComPistas(salaAtual->esquerda, raizPistas);
            return;
        } else if (opcao == 'd' && salaAtual->direita != NULL) {
            explorarSalasComPistas(salaAtual->direita, raizPistas);
            return;
        } else if ((opcao == 'e' && !salaAtual->esquerda) || (opcao == 'd' && !salaAtual->direita)) {
            printf("Não há caminho nessa direção.\n");
        } else {
            printf("Comando inválido.\n");
        }
    }
}

int main() {

    // 🌱 Nível Novato: Mapa da Mansão com Árvore Binária
    //
    // - Crie uma struct Sala com nome, e dois ponteiros: esquerda e direita.
    // - Use funções como criarSala(), conectarSalas() e explorarSalas().
    // - A árvore pode ser fixa: Hall de Entrada, Biblioteca, Cozinha, Sótão etc.
    // - O jogador deve poder explorar indo à esquerda (e) ou à direita (d).
    // - Finalize a exploração com uma opção de saída (s).
    // - Exiba o nome da sala a cada movimento.
    // - Use recursão ou laços para caminhar pela árvore.
    // - Nenhuma inserção dinâmica é necessária neste nível.

    // 🔍 Nível Aventureiro: Armazenamento de Pistas com Árvore de Busca
    //
    // - Crie uma struct Pista com campo texto (string).
    // - Crie uma árvore binária de busca (BST) para inserir as pistas coletadas.
    // - Ao visitar salas específicas, adicione pistas automaticamente com inserirBST().
    // - Implemente uma função para exibir as pistas em ordem alfabética (emOrdem()).
    // - Utilize alocação dinâmica e comparação de strings (strcmp) para organizar.
    // - Não precisa remover ou balancear a árvore.
    // - Use funções para modularizar: inserirPista(), listarPistas().
    // - A árvore de pistas deve ser exibida quando o jogador quiser revisar evidências.

    // 🧠 Nível Mestre: Relacionamento de Pistas com Suspeitos via Hash
    //
    // - Crie uma struct Suspeito contendo nome e lista de pistas associadas.
    // - Crie uma tabela hash (ex: array de ponteiros para listas encadeadas).
    // - A chave pode ser o nome do suspeito ou derivada das pistas.
    // - Implemente uma função inserirHash(pista, suspeito) para registrar relações.
    // - Crie uma função para mostrar todos os suspeitos e suas respectivas pistas.
    // - Adicione um contador para saber qual suspeito foi mais citado.
    // - Exiba ao final o “suspeito mais provável” baseado nas pistas coletadas.
    // - Para hashing simples, pode usar soma dos valores ASCII do nome ou primeira letra.
    // - Em caso de colisão, use lista encadeada para tratar.
    // - Modularize com funções como inicializarHash(), buscarSuspeito(), listarAssociacoes().

    PistaNode *inventarioPistas = NULL;

    // Montagem do Mapa
    Sala *hall = criarSala("Hall de Entrada", "Pegada de lama");
    Sala *cozinha = criarSala("Cozinha", "Faca suja");
    Sala *biblioteca = criarSala("Biblioteca", "Livro rasgado");
    
    hall->direita = cozinha;
    hall->esquerda = biblioteca;

    printf("=== Detective Quest: Nível Aventureiro ===\n");
    explorarSalasComPistas(hall, &inventarioPistas);

    printf("\n=== Fim da Exploração ===\n");
    printf("Pistas coletadas (Ordem Alfabética):\n");
    if (inventarioPistas == NULL) printf("Nenhuma pista encontrada.\n");
    else exibirPistas(inventarioPistas);

    return 0;
}

