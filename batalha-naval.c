#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>

#define TAM 5        // tamanho do tabuleiro (5x5)
#define NAVIOS 3     // quantidade de navios (1 célula cada)

void iniciar_tabuleiro(char tab[TAM][TAM]) {
    for (int i = 0; i < TAM; i++)
        for (int j = 0; j < TAM; j++)
            tab[i][j] = '.';
}

void mostrar_tabuleiro(char tab[TAM][TAM]) {
    printf("  1 2 3 4 5\n");
    for (int i = 0; i < TAM; i++) {
        printf("%c ", 'A' + i);  // linhas A, B, C, ...
        for (int j = 0; j < TAM; j++) {
            printf("%c ", tab[i][j]);
        }
        printf("\n");
    }
}

void posicionar_navios(int pos[NAVIOS][2]) {
    int colocados = 0;
    int usados[TAM][TAM] = {0};

    while (colocados < NAVIOS) {
        int linha = rand() % TAM;
        int coluna = rand() % TAM;
        if (!usados[linha][coluna]) {
            usados[linha][coluna] = 1;
            pos[colocados][0] = linha;
            pos[colocados][1] = coluna;
            colocados++;
        }
    }
}

int verificar_tiro(int pos[NAVIOS][2], int linha, int coluna) {
    for (int i = 0; i < NAVIOS; i++) {
        if (pos[i][0] == linha && pos[i][1] == coluna) return i;
    }
    return -1;
}

int ja_tentado(char tab[TAM][TAM], int linha, int coluna) {
    return tab[linha][coluna] != '.';
}

int ler_jogada(char *entrada, int *linha, int *coluna) {
    // aceita "A1", "a1" ou "1 1"
    while (*entrada == ' ' || *entrada == '\t') entrada++;
    if (isalpha(entrada[0])) {
        char letra = toupper(entrada[0]);
        if (letra < 'A' || letra >= 'A' + TAM) return 0;
        *linha = letra - 'A';
        int col;
        if (sscanf(entrada + 1, "%d", &col) != 1) return 0;
        if (col < 1 || col > TAM) return 0;
        *coluna = col - 1;
        return 1;
    } else {
        int l, c;
        if (sscanf(entrada, "%d %d", &l, &c) == 2) {
            if (l < 1 || l > TAM || c < 1 || c > TAM) return 0;
            *linha = l - 1;
            *coluna = c - 1;
            return 1;
        }
        return 0;
    }
}

int main() {
    char tabuleiro[TAM][TAM];
    int navios[NAVIOS][2];
    int navios_vivos[NAVIOS];
    int acertos = 0;
    int tentativas = 0;
    char entrada[100];

    srand((unsigned) time(NULL));

    iniciar_tabuleiro(tabuleiro);
    posicionar_navios(navios);
    for (int i = 0; i < NAVIOS; i++) navios_vivos[i] = 1;

    printf("=== BATALHA NAVAL ===\n");
    printf("Objetivo: afunde os %d navios escondidos no tabuleiro %dx%d.\n", NAVIOS, TAM, TAM);
    printf("Digite coordenadas como A1 ou '1 1' (linha coluna).\n\n");

    while (acertos < NAVIOS) {
        mostrar_tabuleiro(tabuleiro);
        printf("Sua jogada: ");
        if (!fgets(entrada, sizeof(entrada), stdin)) {
            printf("Erro de leitura.\n");
            return 1;
        }
        entrada[strcspn(entrada, "\n")] = 0; // tira o ENTER

        int l, c;
        if (!ler_jogada(entrada, &l, &c)) {
            printf("Entrada inválida. Use A1 ou '1 1'.\n");
            continue;
        }
        if (ja_tentado(tabuleiro, l, c)) {
            printf("Você já tentou essa posição!\n");
            continue;
        }

        tentativas++;
        int acerto = verificar_tiro(navios, l, c);
        if (acerto >= 0 && navios_vivos[acerto]) {
            printf(">>> ACERTOU! Navio afundado.\n");
            tabuleiro[l][c] = 'X';
            navios_vivos[acerto] = 0;
            acertos++;
        } else {
            printf("ÁGUA!\n");
            tabuleiro[l][c] = 'O';
        }
        printf("\n");
    }

    printf("Parabéns! Você afundou todos os navios em %d tentativas.\n", tentativas);
    mostrar_tabuleiro(tabuleiro);

    return 0;
}
