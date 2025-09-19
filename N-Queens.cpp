#include <iostream>
#include <vector>
using namespace std;

/**
 * Problema das N-Rainhas
 *
 * Objetivo:
 *   Colocar N rainhas em um tabuleiro NxN de forma que
 *   nenhuma ataque a outra (linhas, colunas ou diagonais).
 *
 * Complexidade:
 *   - Tempo: O(N!) no pior caso
 *   - Espaço: O(N^2) para armazenar o tabuleiro
 */

bool isSafe(const vector<vector<int>>& board, int row, int col, int N) {
    // Checar coluna
    for (int i = 0; i < row; i++)
        if (board[i][col]) return false;

    // Diagonal esquerda superior
    for (int i = row, j = col; i >= 0 && j >= 0; i--, j--)
        if (board[i][j]) return false;

    // Diagonal direita superior
    for (int i = row, j = col; i >= 0 && j < N; i--, j++)
        if (board[i][j]) return false;

    return true;
}

bool solveNQueens(vector<vector<int>>& board, int row, int N) {
    if (row >= N) return true;

    for (int col = 0; col < N; col++) {
        if (isSafe(board, row, col, N)) {
            board[row][col] = 1;

            if (solveNQueens(board, row + 1, N)) return true;

            // Backtrack
            board[row][col] = 0;
        }
    }
    return false;
}

void printBoard(const vector<vector<int>>& board, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            cout << (board[i][j] ? "Q " : ". ");
        cout << endl;
    }
}

int main() {
    int N = 8; // Tamanho do tabuleiro
    vector<vector<int>> board(N, vector<int>(N, 0));

    if (solveNQueens(board, 0, N))
        printBoard(board, N);
    else
        cout << "Solução não encontrada!" << endl;

    return 0;
}
