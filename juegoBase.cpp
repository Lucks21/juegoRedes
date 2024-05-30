#include <iostream>
#include <vector>

using namespace std;

const int FILAS = 6;
const int COLUMNAS = 7;

void inicializarTablero(vector<vector<char>>& tablero) {
    for (int i = 0; i < FILAS; ++i) {
        for (int j = 0; j < COLUMNAS; ++j) {
            tablero[i][j] = ' ';
        }
    }
}

void imprimirTablero(const vector<vector<char>>& tablero) {
    for (int i = 0; i < FILAS; ++i) {
        for (int j = 0; j < COLUMNAS; ++j) {
            cout << "| " << tablero[i][j] << " ";
        }
        cout << "|" << endl;
        cout << "-----------------------------" << endl;
    }
    for (int j = 0; j < COLUMNAS; ++j) {
        cout << "  " << j + 1 << " ";
    }
    cout << endl;
}

bool hacerMovimiento(vector<vector<char>>& tablero, int columna, char jugador) {
    if (columna < 0 || columna >= COLUMNAS || tablero[0][columna] != ' ') {
        return false;
    }
    for (int i = FILAS - 1; i >= 0; --i) {
        if (tablero[i][columna] == ' ') {
            tablero[i][columna] = jugador;
            return true;
        }
    }
    return false;
}

bool verificarVictoria(const vector<vector<char>>& tablero, char jugador) {
    for (int i = 0; i < FILAS; ++i) {
        for (int j = 0; j < COLUMNAS - 3; ++j) {
            if (tablero[i][j] == jugador && tablero[i][j + 1] == jugador && tablero[i][j + 2] == jugador && tablero[i][j + 3] == jugador) {
                return true;
            }
        }
    }
    for (int i = 0; i < FILAS - 3; ++i) {
        for (int j = 0; j < COLUMNAS; ++j) {
            if (tablero[i][j] == jugador && tablero[i + 1][j] == jugador && tablero[i + 2][j] == jugador && tablero[i + 3][j] == jugador) {
                return true;
            }
        }
    }
    for (int i = 3; i < FILAS; ++i) {
        for (int j = 0; j < COLUMNAS - 3; ++j) {
            if (tablero[i][j] == jugador && tablero[i - 1][j + 1] == jugador && tablero[i - 2][j + 2] == jugador && tablero[i - 3][j + 3] == jugador) {
                return true;
            }
        }
    }
    for (int i = 0; i < FILAS - 3; ++i) {
        for (int j = 0; j < COLUMNAS - 3; ++j) {
            if (tablero[i][j] == jugador && tablero[i + 1][j + 1] == jugador && tablero[i + 2][j + 2] == jugador && tablero[i + 3][j + 3] == jugador) {
                return true;
            }
        }
    }
    return false;
}

bool tableroLleno(const vector<vector<char>>& tablero) {
    for (int j = 0; j < COLUMNAS; ++j) {
        if (tablero[0][j] == ' ') {
            return false;
        }
    }
    return true;
}

int main() {
    vector<vector<char>> tablero(FILAS, vector<char>(COLUMNAS));
    inicializarTablero(tablero);
    
    char jugadorActual = 'C';
    bool victoria = false;

    while (!victoria && !tableroLleno(tablero)) {
        imprimirTablero(tablero);
        int columna;
        cout << "Jugador " << (jugadorActual == 'C' ? "Cliente" : "Servidor") << ", elige una columna (1-" << COLUMNAS << "): ";
        cin >> columna;
        --columna;

        if (!hacerMovimiento(tablero, columna, jugadorActual)) {
            cout << "Movimiento inválido. Intenta de nuevo." << endl;
            continue;
        }

        victoria = verificarVictoria(tablero, jugadorActual);

        if (victoria) {
            imprimirTablero(tablero);
            cout << "Jugador " << (jugadorActual == 'C' ? "Cliente" : "Servidor") << " ha ganado!" << endl;
        } else if (tableroLleno(tablero)) {
            imprimirTablero(tablero);
            cout << "Empate! El tablero está lleno." << endl;
        } else {
            jugadorActual = (jugadorActual == 'C' ? 'S' : 'C');
        }
    }

    return 0;
}