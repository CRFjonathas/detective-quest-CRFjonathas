#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TAM_HASH 10

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.

// --- Estruturas ---

// BST para guardar pistas coletadas
typedef struct PistaNode {
    char texto[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// Hash: Nó para encadeamento (Pista -> Suspeito)
typedef struct HashNode {
    char pista[100];
    char suspeito[50];
    struct HashNode *prox;
} HashNode;

// Mapa da Mansão
typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Tabela Hash global (para simplificar o exemplo)
HashNode* tabelaHash[TAM_HASH];

// --- Funções Hash ---

// Função de hash simples (soma ASCII % TAM)
int funcaoHash(char *chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        soma += chave[i];
    }
    return soma % TAM_HASH;
}

// Documentação: Insere associação pista/suspeito na tabela hash.
void inserirNaHash(char *pista, char *suspeito) {
    int indice = funcaoHash(pista);
    HashNode *novo = (HashNode*) malloc(sizeof(HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabelaHash[indice]; // Inserção no início (tratamento de colisão)
    tabelaHash[indice] = novo;
}

// Documentação: Consulta o suspeito correspondente a uma pista.
char* encontrarSuspeito(char *pista) {
    int indice = funcaoHash(pista);
    HashNode *atual = tabelaHash[indice];
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->prox;
    }
    return NULL; // Não encontrado
}

// --- Funções Árvores (BST e Mapa) ---

Sala* criarSala(char *nome, char *pista) {
    Sala *s = (Sala*) malloc(sizeof(Sala));
    strcpy(s->nome, nome);
    strcpy(s->pista, pista ? pista : "");
    s->esquerda = NULL;
    s->direita = NULL;
    return s;
}

PistaNode* inserirPista(PistaNode *raiz, char *texto) {
    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(novo->texto, texto);
        novo->esquerda = NULL; 
        novo->direita = NULL;
        printf(">> Pista NOVA encontrada: [%s]\n", texto);
        return novo;
    }
    // Evita duplicatas na BST
    if (strcmp(texto, raiz->texto) == 0) return raiz;
    
    if (strcmp(texto, raiz->texto) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, texto);
    else
        raiz->direita = inserirPista(raiz->direita, texto);
    return raiz;
}

// Função auxiliar para percorrer a BST e contar evidências contra o acusado
void contarEvidencias(PistaNode *raiz, char *acusado, int *contador) {
    if (raiz != NULL) {
        contarEvidencias(raiz->esquerda, acusado, contador);
        
        // Verifica a pista atual na Hash
        char *suspeitoAssociado = encontrarSuspeito(raiz->texto);
        if (suspeitoAssociado != NULL) {
            // Compara (case insensitive ou exato, aqui faremos exato para simplificar)
            if (strcmp(suspeitoAssociado, acusado) == 0) {
                (*contador)++;
                printf("  - Pista '%s' aponta para %s.\n", raiz->texto, acusado);
            }
        }
        
        contarEvidencias(raiz->direita, acusado, contador);
    }
}

void exibirPistasOrdem(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistasOrdem(raiz->esquerda);
        printf(" - %s\n", raiz->texto);
        exibirPistasOrdem(raiz->direita);
    }
}

// Documentação: Navega pela árvore e ativa o sistema de pistas.
void explorarSalas(Sala *atual, PistaNode **raizPistas) {
    char op;
    if (!atual) return;

    while(1) {
        printf("\n--- %s ---\n", atual->nome);
        
        if (strlen(atual->pista) > 0) {
            *raizPistas = inserirPista(*raizPistas, atual->pista);
        }

        printf("[e]squerda | [d]ireita | [s]air: ");
        scanf(" %c", &op);

        if (op == 's') return;
        
        if (op == 'e' && atual->esquerda) explorarSalas(atual->esquerda, raizPistas);
        else if (op == 'd' && atual->direita) explorarSalas(atual->direita, raizPistas);
        else if ((op == 'e' || op == 'd')) printf("Caminho fechado.\n");
        else printf("Opção inválida.\n");
        
        // Após retornar da recursão, se quiser sair totalmente, deve-se implementar lógica extra.
        // Aqui o fluxo volta para o loop da sala atual.
        // Se o usuário digitou 's' na sub-sala, ele volta para cá. 
        // Vamos perguntar se quer continuar explorando esta sala ou sair geral?
        // Simplificação: Continua no loop.
    }
}

// Documentação: Conduz à fase de julgamento final.
void verificarSuspeitoFinal(PistaNode *raizPistas) {
    char acusado[50];
    int evidencias = 0;

    printf("\n=== FASE DE JULGAMENTO ===\n");
    printf("Pistas reunidas:\n");
    exibirPistasOrdem(raizPistas);

    printf("\nQuem você acusa? (Digite o nome, ex: Mordomo): ");
    scanf("%s", acusado);

    printf("\nAnalisando evidências contra %s...\n", acusado);
    contarEvidencias(raizPistas, acusado, &evidencias);

    if (evidencias >= 2) {
        printf("\n>>> SUCESSO! Você encontrou %d provas contra %s. O culpado foi preso!\n", evidencias, acusado);
    } else {
        printf("\n>>> FRACASSO. Apenas %d prova(s) encontrada(s). O suspeito foi liberado por falta de provas.\n", evidencias);
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

    // Inicializa Hash
    for(int i=0; i<TAM_HASH; i++) tabelaHash[i] = NULL;

    // Configuração do Cenário (Regras Codificadas)
    // Pistas: "Relogio quebrado" -> Mordomo
    //         "Luva branca" -> Mordomo
    //         "Batom vermelho" -> Governanta
    
    inserirNaHash("Relogio quebrado", "Mordomo");
    inserirNaHash("Luva branca", "Mordomo");
    inserirNaHash("Batom vermelho", "Governanta");

    // Mapa
    Sala *hall = criarSala("Hall", NULL);
    Sala *sala1 = criarSala("Sala de Jantar", "Relogio quebrado");
    Sala *sala2 = criarSala("Cozinha", "Batom vermelho");
    Sala *sala3 = criarSala("Sotao", "Luva branca");

    hall->esquerda = sala1;
    hall->direita = sala2;
    sala1->esquerda = sala3; // Escondido no Sótão

    PistaNode *minhasPistas = NULL;

    printf("=== Detective Quest: Nível Mestre ===\n");
    printf("Dica: Explore bem para garantir pelo menos 2 pistas contra o culpado.\n");
    
    explorarSalas(hall, &minhasPistas);

    verificarSuspeitoFinal(minhasPistas);

    return 0;
}

