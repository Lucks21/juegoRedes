#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <mutex>

using namespace std;

const int FILAS = 6;
const int COLUMNAS = 7;
int contadorJugadores = 0;
mutex contadorMutex;

void inicializarTablero(vector<vector<char>>& tablero) {
    for (int i = 0; i < FILAS; ++i) {
        for (int j = 0; j < COLUMNAS; ++j) {
            tablero[i][j] = ' ';
        }
    }
}

void imprimirTablero(const vector<vector<char>>& tablero, int socket_cliente) {
    string tablero_str;
    for (int i = 0; i < FILAS; ++i) {
        for (int j = 0; j < COLUMNAS; ++j) {
            tablero_str += "| ";
            tablero_str += tablero[i][j];
            tablero_str += " ";
        }
        tablero_str += "|\n";
        tablero_str += "-----------------------------\n";
    }
    for (int j = 0; j < COLUMNAS; ++j) {
        tablero_str += "  ";
        tablero_str += to_string(j + 1);
        tablero_str += " ";
    }
    tablero_str += "\n";
    send(socket_cliente, tablero_str.c_str(), tablero_str.size(), 0);
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

void jugar(int socket_cliente, struct sockaddr_in direccionCliente, int numeroJugador) {
    char buffer[1024];
    memset(buffer, '\0', sizeof(char) * 1024);
    int n_bytes = 0;

    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(direccionCliente.sin_addr), ip, INET_ADDRSTRLEN);

    cout << "[jugador " << numeroJugador << ", ip: " << ip << ":" << ntohs(direccionCliente.sin_port) << "] Nuevo jugador." << endl;

    vector<vector<char>> tablero(FILAS, vector<char>(COLUMNAS));
    inicializarTablero(tablero);

    send(socket_cliente, "Sorteando quien empieza el juego...\n", 36, 0);

    char fichaServidor = 'S';
    char fichaCliente = 'C';
    char jugadorActual = rand() % 2 == 0 ? fichaServidor : fichaCliente;

    this_thread::sleep_for(chrono::seconds(2));

    if (jugadorActual == fichaServidor) {
        send(socket_cliente, "El servidor empieza el juego.\n", 30, 0);
    } else {
        send(socket_cliente, "El cliente empieza el juego.\n", 29, 0);
    }

    bool victoria = false;

    while (!victoria && !tableroLleno(tablero)) {
        if (jugadorActual == fichaCliente) {
            imprimirTablero(tablero, socket_cliente);
            send(socket_cliente, "Elige una columna (1-7): ", 25, 0);
            n_bytes = recv(socket_cliente, buffer, 1024, 0);
            buffer[n_bytes] = '\0';
            int columna = atoi(buffer) - 1;

            if (!hacerMovimiento(tablero, columna, fichaCliente)) {
                send(socket_cliente, "Movimiento inválido. Intenta de nuevo.\n", 39, 0);
                continue;
            }
            victoria = verificarVictoria(tablero, fichaCliente);

            if (victoria) {
                imprimirTablero(tablero, socket_cliente);
                send(socket_cliente, "¡Has ganado!\n", 13, 0);
                cout << "[jugador " << numeroJugador << ", ip: " << ip << ":" << ntohs(direccionCliente.sin_port) << "] ha ganado." << endl;
                break;
            } else if (tableroLleno(tablero)) {
                imprimirTablero(tablero, socket_cliente);
                send(socket_cliente, "Empate! El tablero está lleno.\n", 30, 0);
                cout << "[jugador " << numeroJugador << ", ip: " << ip << ":" << ntohs(direccionCliente.sin_port) << "] empate." << endl;
                break;
            } else {
                jugadorActual = fichaServidor;
            }
        } else {
            int columna = rand() % COLUMNAS;
            while (!hacerMovimiento(tablero, columna, fichaServidor)) {
                columna = rand() % COLUMNAS;
            }
            victoria = verificarVictoria(tablero, fichaServidor);

            if (victoria) {
                imprimirTablero(tablero, socket_cliente);
                send(socket_cliente, "¡El servidor ha ganado!\n", 24, 0);
                cout << "[jugador " << numeroJugador << ", ip: " << ip << ":" << ntohs(direccionCliente.sin_port) << "] ha perdido." << endl;
                break;
            } else if (tableroLleno(tablero)) {
                imprimirTablero(tablero, socket_cliente);
                send(socket_cliente, "Empate! El tablero está lleno.\n", 30, 0);
                cout << "[jugador " << numeroJugador << ", ip: " << ip << ":" << ntohs(direccionCliente.sin_port) << "] empate." << endl;
                break;
            } else {
                jugadorActual = fichaCliente;
            }
        }
    }

    close(socket_cliente);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        cout << "Uso: " << argv[0] << " <puerto>" << endl;
        return 1;
    }

    srand(time(0));

    int port = atoi(argv[1]);
    int socket_server = 0;
    struct sockaddr_in direccionServidor, direccionCliente;

    cout << "Creando socket de escucha...\n";
    if ((socket_server = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Error al crear el socket de escucha");
        exit(EXIT_FAILURE);
    }

    cout << "Configurando la estructura de la dirección del socket...\n";
    memset(&direccionServidor, 0, sizeof(direccionServidor));
    direccionServidor.sin_family = AF_INET;
    direccionServidor.sin_addr.s_addr = htonl(INADDR_ANY);
    direccionServidor.sin_port = htons(port);

    cout << "Enlazando el socket...\n";
    if (bind(socket_server, (struct sockaddr *)&direccionServidor, sizeof(direccionServidor)) < 0) {
        perror("Error al llamar a bind()");
        exit(EXIT_FAILURE);
    }

    cout << "Llamando a listen...\n";
    if (listen(socket_server, 1024) < 0) {
        perror("Error al llamar a listen()");
        exit(EXIT_FAILURE);
    }

    socklen_t addr_size = sizeof(struct sockaddr_in);

    cout << "Esperando solicitud de cliente...\n";
    while (true) {
        int socket_cliente;

        if ((socket_cliente = accept(socket_server, (struct sockaddr *)&direccionCliente, &addr_size)) < 0) {
            perror("Error al llamar a accept()");
            exit(EXIT_FAILURE);
        }

        int numeroJugador;
        {
            lock_guard<mutex> lock(contadorMutex);
            numeroJugador = ++contadorJugadores;
        }

        thread cliente_thread(jugar, socket_cliente, direccionCliente, numeroJugador);
        cliente_thread.detach();
    }

    close(socket_server);
    return 0;
}
