/*
 * TODO ESSE CODIGO FOI ESCRITO POR: LUIZ EDUARDO DA SILVA FERNANDES
 * ESCREVI ATRAVÉS DO MEU PRÓPRIO VSCODE E NÃO PELO CODESPACE, ENTÃO SE TIVER ALGUM
 * ERRO NESSE CASO, PODE SER A FALTA DE ALGUMA COMPATIBILIDADE ENTRE ESSAS VERSÕES
 * DITO ISSO, ESSE CÓDIGO FOI DIFICIL DE MAIS. TIPO, BEM DIFICIL MESMO!!!
 */

 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Constantes e Definições Simples ---
#define MAX_NOME 30
#define MAX_COR 10
#define MAX_MISSAO 100
#define NUM_MISSOES 5 // Quantidade de missões disponíveis

// --- Estruturas (Structs) ---

// Struct para guardar as info de cada território
typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];   // Ex: "Azul" ou "Vermelho"
    int tropas;          
} Territorio;

// Struct para guardar as info do jogador
typedef struct {
    char nome[20];
    char* missao;        // Ponteiro para a missão (alocada na memória)
} Jogador;


// --- Protótipos das Funções ---

// Funções para rodar o jogo
void inicializarGeradorAleatorio();
void cadastrarTerritorios(Territorio* mapa, int num);
void exibirTerritorios(const Territorio* mapa, int num);
void exibirMissao(char* missao); 
void atacar(Territorio* atacante, Territorio* defensor);
void simularAtaques(Territorio* mapa, int numTerritorios, Jogador* jogador, int numJogadores);

// Funções de Missão
void atribuirMissao(char* destino, char* missoes[], int totalMissoes); 
int verificarMissao(char* missao, const Territorio* mapa, int tamanho); 

// Funções de Alocação e Memória
Territorio* alocarTerritorios(int num);
Jogador* alocarJogadores(int num);
void liberarMemoria(Territorio* mapa, Jogador* jogadores, int numJogadores);


// ---------------------------------------------
// --- FUNÇÕES DE UTILIDADE E MEMÓRIA ---
// ---------------------------------------------

// Liga o gerador de números aleatórios para os dados.
void inicializarGeradorAleatorio() {
    srand(time(NULL));
}

// Aloca espaço para o vetor de territórios na memória (dinamicamente)
Territorio* alocarTerritorios(int num) {
    // calloc zera tudo, é bom para começar limpo.
    Territorio* mapa = (Territorio*)calloc(num, sizeof(Territorio));
    if (mapa == NULL) {
        printf("ERRO: Falha ao alocar memória.\n");
        exit(1);
    }
    return mapa;
}

// Aloca espaço para o vetor de jogadores na memória
Jogador* alocarJogadores(int num) {
    Jogador* jogadores = (Jogador*)calloc(num, sizeof(Jogador));
    if (jogadores == NULL) {
        printf("ERRO: Falha ao alocar memória para jogadores.\n");
        exit(1);
    }
    return jogadores;
}

// Libera toda a memória que foi alocada dinamicamente com malloc/calloc.
void liberarMemoria(Territorio* mapa, Jogador* jogadores, int numJogadores) {
    int i;
    // 1. Libera o vetor de territórios
    if (mapa != NULL) {
        free(mapa);
    }
    // 2. Libera a missão de cada jogador (que é uma string alocada)
    if (jogadores != NULL) {
        for (i = 0; i < numJogadores; i++) {
            if (jogadores[i].missao != NULL) {
                free(jogadores[i].missao);
            }
        }
        // 3. Libera o vetor de jogadores
        free(jogadores);
    }
    printf("\n✅ Memória liberada.\n");
}

// ---------------------------------------------
// --- FUNÇÕES DE CADASTRO, EXIBIÇÃO E MISSÃO ---
// ---------------------------------------------

// Pede ao usuário para digitar os dados de cada território.
void cadastrarTerritorios(Territorio* mapa, int num) {
    int i;
    printf("\n--- CADASTRO DE TERRITÓRIOS ---\n");
    
    for (i = 0; i < num; i++) {
        // Usa ponteiro para acessar o território atual (mapa + i)
        Territorio* t = mapa + i; 

        printf("\nTerritório %d:\n", i + 1);
        printf("Nome: ");
        scanf("%29s", t->nome); 
        printf("Cor do exército: ");
        scanf("%9s", t->cor);
        printf("Tropas: ");
        while (scanf("%d", &t->tropas) != 1 || t->tropas < 1) {
            printf("Inválido. Tropas (min 1): ");
            while (getchar() != '\n'); 
        }
    }
}

// Mostra o estado atual de todos os territórios.
void exibirTerritorios(const Territorio* mapa, int num) {
    int i;
    printf("\n--- RELATÓRIO ATUAL ---\n");

    for (i = 0; i < num; i++) {
        // Acessa o território como um ponteiro constante
        const Territorio* t = mapa + i; 
        
        printf("[%d] %s (Cor: %s | Tropas: %d)\n", 
               i, t->nome, t->cor, t->tropas);
    }
    printf("-----------------------------------\n");
}

// Mostra a missão atual do jogador.
void exibirMissao(char* missao) {
    if (missao != NULL) {
        printf("\n🎯 Sua Missão: **%s**\n", missao);
        printf("-----------------------------------\n");
    }
}

// Sorteia uma missão do vetor e a copia para o espaço do jogador.
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int indiceSorteado = rand() % totalMissoes;
    // Copia a string da missão sorteada para o destino.
    strcpy(destino, missoes[indiceSorteado]);
}

// Verifica se o jogador cumpriu a missão.
int verificarMissao(char* missao, const Territorio* mapa, int tamanho) {
    int i, countVermelho = 0, countAzul = 0;
    
    // Conta quantos territórios cada cor controla
    for (i = 0; i < tamanho; i++) {
        if (strcmp((mapa + i)->cor, "Vermelho") == 0) {
            countVermelho++;
        } else if (strcmp((mapa + i)->cor, "Azul") == 0) {
            countAzul++;
        }
    }

    // --- LÓGICA DE VERIFICAÇÃO SIMPLES ---

    // Exemplo 1: Conquistar 3 ou mais territórios (Seja Azul ou Vermelho)
    if (strstr(missao, "Conquistar 3") != NULL) {
        if (countAzul >= 3 || countVermelho >= 3) {
             return 1;
        } 
    }
    
    // Exemplo 2: Eliminar todas as tropas de outra cor (ex: Vermelho)
    if (strstr(missao, "Eliminar todas as tropas da cor Vermelho") != NULL) {
        if (countVermelho == 0) {
            return 1;
        }
    }

    return 0; // Missão não cumprida
}

// ---------------------------------------------
// --- FUNÇÕES DE SIMULAÇÃO DE BATALHA ---
// ---------------------------------------------

// Simula o ataque rolando dados e atualizando tropas e cor.
void atacar(Territorio* atacante, Territorio* defensor) {
    // Rola um "dado" de 1 a 6 para cada lado
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\n*** BATALHA: %s vs %s ***\n", atacante->nome, defensor->nome);
    printf("Atacante tirou: %d | Defensor tirou: %d\n", dadoAtacante, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("🎉 VITÓRIA do %s! O território é conquistado.\n", atacante->cor);
        
        // Se vencer, transfere a cor e metade das tropas
        if (atacante->tropas > 1) { 
            int tropasMovidas = atacante->tropas / 2;
            
            // Defensor muda de cor
            strcpy(defensor->cor, atacante->cor);
            defensor->tropas = tropasMovidas;
            atacante->tropas -= tropasMovidas;

            printf("Conquista! %d tropas movidas.\n", tropasMovidas);
        } else {
             printf("Venceu, mas não tinha tropas para mover.\n");
        }
        
    } else {
        printf("🛡️ DEFESA BEM SUCEDIDA!\n");
        
        // Se perder, atacante perde 1 tropa (mínimo 1)
        if (atacante->tropas > 1) {
            atacante->tropas--;
            printf("%s perdeu 1 tropa.\n", atacante->nome);
        }
    }
}

// Gerencia o loop de ataques e a verificação de vitória.
void simularAtaques(Territorio* mapa, int numTerritorios, Jogador* jogadores, int numJogadores) {
    int i, j, indiceAtacante, indiceDefensor, vencedor = -1;
    char continuar = 's';
    
    printf("\n--- INÍCIO DA SIMULAÇÃO ---\n");

    // Roda enquanto não houver vencedor e o jogador quiser continuar
    while (vencedor == -1 && (continuar == 's' || continuar == 'S')) {
        
        for (j = 0; j < numJogadores; j++) {
            
            if (vencedor != -1) break; // Se venceu na rodada anterior, para.
            printf("\n--- TURNO DO JOGADOR %s ---\n", jogadores[j].nome);

            // 1. Mostrar status atual do mapa
            exibirTerritorios(mapa, numTerritorios);
            
            // 2. Escolha do Atacante
            printf("Índice do território ATACANTE (0 a %d, ou -1 para passar): ", numTerritorios - 1);
            if (scanf("%d", &indiceAtacante) != 1) { break; }
            if (indiceAtacante == -1) continue; 
            
            // Valida se o índice existe no mapa
            if (indiceAtacante < 0 || indiceAtacante >= numTerritorios) {
                printf("⚠️ Índice inválido.\n"); j--; continue;
            }

            // 3. Escolha do Defensor
            printf("Índice do território DEFENSOR (0 a %d): ", numTerritorios - 1);
            if (scanf("%d", &indiceDefensor) != 1) { break; }
            
            if (indiceDefensor < 0 || indiceDefensor >= numTerritorios) {
                printf("⚠️ Índice inválido.\n"); j--; continue;
            }
            
            Territorio* atacante = mapa + indiceAtacante;
            Territorio* defensor = mapa + indiceDefensor;
            
            // 4. Validações importantes
            if (indiceAtacante == indiceDefensor) { printf("⚠️ Não pode atacar a si mesmo!\n"); j--; continue; }
            // Verifica se as cores são diferentes (ataque inimigo)
            if (strcmp(atacante->cor, defensor->cor) == 0) { 
                printf("⚠️ Não pode atacar a própria cor!\n"); j--; continue;
            }
            if (atacante->tropas <= 1) { printf("⚠️ Precisa de no mínimo 2 tropas para atacar.\n"); j--; continue; }

            // 5. Executar o ataque
            atacar(atacante, defensor); 
            
            // 6. Verificar se o jogador venceu
            if (verificarMissao(jogadores[j].missao, mapa, numTerritorios)) {
                vencedor = j;
                break; 
            }
        } // Fim do turno do jogador
        
        if (vencedor == -1) {
            printf("\nContinuar o jogo? (s/n): ");
            while (getchar() != '\n');
            scanf(" %c", &continuar);
        }
    } // Fim do loop principal

    // Declara o vencedor se houver
    if (vencedor != -1) {
        printf("\n\n🎉🎉🎉 O VENCEDOR É O JOGADOR %s! 🎉🎉🎉\n", jogadores[vencedor].nome);
        printf("Missão Cumprida: %s\n", jogadores[vencedor].missao);
    } else {
        printf("\nJogo encerrado.\n");
    }
}

// ---------------------------------------------
// --- FUNÇÃO PRINCIPAL (MAIN) ---
// ---------------------------------------------

int main() {
    // Vetor de strings com as missões
    char* missoes[NUM_MISSOES] = {
        "Conquistar 3 ou mais territórios",
        "Eliminar todas as tropas da cor Vermelho",
        "Conquistar 4 territorios seguidos",
        "Ter no minimo 10 tropas em um unico territorio",
        "Conquistar o territorio chamado 'Capital'"
    };

    int numTerritorios, numJogadores = 1; // Simplificado para 1 jogador vs 1 exército inimigo
    Territorio* mapa = NULL;
    Jogador* jogadores = NULL;

    // Inicia o aleatório
    inicializarGeradorAleatorio();
    
    // Pede o tamanho do mapa
    printf("Quantos territórios no mapa? (mínimo 2): ");
    while (scanf("%d", &numTerritorios) != 1 || numTerritorios < 2) {
        printf("Inválido. Mínimo 2: ");
        while (getchar() != '\n'); 
    }
    
    // Aloca memória para o mapa e jogadores
    mapa = alocarTerritorios(numTerritorios);
    jogadores = alocarJogadores(numJogadores);
    
    // Configura o Jogador
    printf("\nNome do Jogador: ");
    scanf("%19s", jogadores[0].nome);
    
    // Aloca memória para a string da missão do jogador
    jogadores[0].missao = (char*)malloc(MAX_MISSAO * sizeof(char));
    if (jogadores[0].missao == NULL) {
        printf("ERRO: Falha ao alocar missão.\n");
        liberarMemoria(mapa, jogadores, numJogadores);
        return 1;
    }
    
    // Sorteia e atribui a missão
    atribuirMissao(jogadores[0].missao, missoes, NUM_MISSOES);
    
    // Cadastro dos territórios iniciais
    cadastrarTerritorios(mapa, numTerritorios);
    
    // Mostra a missão e começa o jogo
    exibirMissao(jogadores[0].missao);
    
    // Loop principal de ataques e verificação
    simularAtaques(mapa, numTerritorios, jogadores, numJogadores);
    
    // Libera toda a memória usada no final
    liberarMemoria(mapa, jogadores, numJogadores);

    return 0;
}