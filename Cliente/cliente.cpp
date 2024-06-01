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

    const char* server_ip = argv[1];  // Dirección IP del servidor
    int port = atoi(argv[2]);  // Puerto del servidor
    
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[2048] = {0};

    // Crear el socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Error de creación de socket");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Convertir la dirección IP a formato binario
    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
        perror("Dirección inválida / Dirección no soportada");
        return -1;
    }

    // Conectar al servidor
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Conexión fallida");
        return -1;
    }

    // Bucle principal de comunicación con el servidor
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int n = read(sock, buffer, 2048);  // Leer datos del servidor
        if (n > 0) {
            cout << buffer;

            // Verificar si el juego ha terminado
            if (strstr(buffer, "¡Has ganado!") || strstr(buffer, "¡El servidor ha ganado!") || strstr(buffer, "Empate")) {
                break;
            }

            // Pedir al usuario que elija una columna
            if (strstr(buffer, "Elige una columna (1-7): ")) {
                int columna;
                cin >> columna;
                string movimiento = to_string(columna) + "\n";
                send(sock, movimiento.c_str(), movimiento.length(), 0);  // Enviar movimiento al servidor
            }
        } else if (n == 0) {
            cout << "Conexión cerrada por el servidor." << endl;
            break;
        } else {
            perror("Error de lectura del socket");
            break;
        }
    }

    // Leer cualquier mensaje final del servidor
    memset(buffer, 0, sizeof(buffer));
    int n = read(sock, buffer, 2048);
    if (n > 0) {
        cout << buffer;
    }

    close(sock);  // Cerrar el socket
    return 0;
}
