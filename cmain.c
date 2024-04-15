#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 6666

int main() {
    int client_socket;
    struct sockaddr_in server_address;
    char buffer[1024] = {0};
    signal(SIGPIPE, SIG_IGN);
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("fallo creando socket");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    char address_ip[16];
    printf("Ingrese la dirección ip del servidor: ");
    scanf("%s", address_ip);

    if(inet_pton(AF_INET, address_ip, &server_address.sin_addr) <= 0) {
        perror("fallo convirtiendo la ip a binario");
        exit(EXIT_FAILURE);
    }

    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("fallo conectando al servidor");
        exit(EXIT_FAILURE);
    }

    char *changesCodes[] = {"AUD", "MXN", "GBP", "NOK", "SEK", "USD", "CAD", "JPY", "DKK", "CHF", "EUR", "TER"};

    int num_changes = sizeof(changesCodes) / sizeof(changesCodes[0]);

    for (int i = 0; i < num_changes; i++) {
        send(client_socket, changesCodes[i], strlen(changesCodes[i]), 0);

        read(client_socket, buffer, 1024);
        printf("El cambio de %s es de: %s\n", changesCodes[i], buffer);
    }

    close(client_socket);

    return 0;
}