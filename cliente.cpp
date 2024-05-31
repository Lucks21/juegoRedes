#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

int main(int argc, char **argv) {
    if (argc != 3) {
        cout << "Uso: " << argv[0] << " <direccion IP> <puerto>" << endl;
        return 1;
    }

    const char* server_ip = argv[1];
    int port = atoi(argv[2]);
    
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[2048] = {0}; // Aumentamos el tamaño del buffer para el tablero

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Error de creación de socket" << endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
        cout << "Dirección inválida / Dirección no soportada" << endl;
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        cout << "Conexión fallida" << endl;
        return -1;
    }

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int n = read(sock, buffer, 2048);
        if (n > 0) {
            cout << buffer;

            if (strstr(buffer, "¡Has ganado!") || strstr(buffer, "¡El servidor ha ganado!") || strstr(buffer, "Empate")) {
                break;
            }

            if (strstr(buffer, "Elige una columna (1-7): ")) {
                int columna;
                cin >> columna;
                string movimiento = to_string(columna) + "\n";
                send(sock, movimiento.c_str(), movimiento.length(), 0);
            }
        } else if (n == 0) {
            cout << "Conexión cerrada por el servidor." << endl;
            break;
        } else {
            cout << "Error de lectura del socket." << endl;
            break;
        }
    }

    close(sock);
    return 0;
}
