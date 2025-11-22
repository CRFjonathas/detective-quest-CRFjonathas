#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.

typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// --- Funções ---

// Documentação: Cria dinamicamente uma sala com nome e ponteiros nulos.
Sala* criarSala(char *nome) {
    Sala *novaSala = (Sala*) malloc(sizeof(Sala));
    if (novaSala != NULL) {
        strcpy(novaSala->nome, nome);
        novaSala->esquerda = NULL;
        novaSala->direita = NULL;
    }
    return novaSala;
}

// Documentação: Permite a navegação do jogador pela árvore até um nó folha ou saída.
void explorarSalas(Sala *atual) {
    char opcao;
    
    if (atual == NULL) return;

    while (1) {
        printf("\n--- %s ---\n", atual->nome);
        
        // Verifica se é um nó folha (sem saídas)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Você chegou a um beco sem saída (Sala Final).\n");
            return;
        }

        printf("Escolha seu caminho: [e]squerda, [d]ireita ou [s]air: ");
        scanf(" %c", &opcao);

        if (opcao == 's') {
            printf("Saindo da exploração...\n");
            return;
        } else if (opcao == 'e' && atual->esquerda != NULL) {
            explorarSalas(atual->esquerda); // Recursão para navegar
            // Ao voltar da recursão, continua no loop deste nível (opcional, dependendo da lógica desejada)
            // Aqui optamos por retornar ao menu anterior após sair da sub-sala ou encerrar.
            // Para navegação contínua linear, basta chamar a função. 
            // Se o objetivo é 'descer' e não voltar automaticamente, o return abaixo controla isso.
            return; 
        } else if (opcao == 'd' && atual->direita != NULL) {
            explorarSalas(atual->direita);
            return;
        } else {
            printf("Opção inválida ou caminho inexistente.\n");
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

    Sala *hall = criarSala("Hall de Entrada");
    Sala *salaEstar = criarSala("Sala de Estar");
    Sala *cozinha = criarSala("Cozinha");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *jardim = criarSala("Jardim"); 

    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;

    printf("=== Detective Quest: Nível Novato ===\n");
    explorarSalas(hall);

    // Liberação de memória simplificada (idealmente percorreria a árvore)
    free(jardim); free(biblioteca); free(cozinha); free(salaEstar); free(hall);

    return 0;
}

