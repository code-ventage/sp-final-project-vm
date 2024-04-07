#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <arpa/inet.h>

#define PORT 6666
#define BUFFER_SIZE 1024

const double changes[] = {
        78.27000, // AUD
        7.23382, // MXN
        151.66200, // GBP
        11.08806, // NOK
        11.24343, // SEK
        120.00000, // USD
        88.64593, // CAD
        1.26021, // JPY
        17.37796, // DKK
        133.11148, // CHF
        129.61800 // EUR
};

void handle_sigchld(int sig) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

void handle_sigint(int sig) {
    signal(SIGINT, SIG_IGN);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket fallido");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("Bind fallido");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("fallo en el listen");
        exit(EXIT_FAILURE);
    }

    signal(SIGCHLD, handle_sigchld);
    signal(SIGINT, handle_sigint);

    printf("Servidor escuchando en el puerto %d\n", PORT);

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &addrlen)) < 0) {
            perror("fallo aceptando conexión");
            exit(EXIT_FAILURE);
        }

        printf("Conexión aceptada\n");

        pid_t pid = fork();

        if (pid < 0) {
            perror("fallo en el fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            close(server_fd);
            while (1) {
                memset(buffer, 0, sizeof(buffer));
                ssize_t read_size = read(new_socket, buffer, BUFFER_SIZE);

                if (read_size == 0) {
                    break;
                }

                if (strcmp(buffer, "TER") == 0) {
                    printf("Cerrando conexión\n");
                    break;
                }

                switch (buffer[0]) {
                    case 'A':
                        sprintf(buffer, "1 AUD = %.5f USD", changes[0]);

                        break;
                    case 'M':
                        sprintf(buffer, "1 MXN = %.5f USD", changes[1]);
                        break;
                    case 'G':
                        sprintf(buffer, "1 GBP = %.5f USD", changes[2]);
                        break;
                    case 'N':
                        sprintf(buffer, "1 NOK = %.5f USD", changes[3]);
                        break;
                    case 'S':
                        sprintf(buffer, "1 SEK = %.5f USD", changes[4]);
                        break;
                    case 'U':
                        sprintf(buffer, "1 USD = %.5f USD", changes[5]);
                        break;
                    case 'C':
                        if (buffer[1] == 'A')
                            sprintf(buffer, "1 CAD = %.5f USD", changes[6]);
                        else
                            sprintf(buffer, "1 CHF = %.5f USD", changes[9]);
                        break;
                    case 'J':
                        sprintf(buffer, "1 JPY = %.5f USD", changes[7]);
                        break;
                    case 'D':
                        sprintf(buffer, "1 DKK = %.5f USD", changes[8]);
                        break;
                    case 'E':
                        sprintf(buffer, "1 EUR = %.5f USD", changes[10]);
                        break;
                    default:
                        sprintf(buffer, "Moneda no soportada");
                        break;

                }

                printf("Enviando respuesta: %s\n", buffer);
                send(new_socket, buffer, strlen(buffer), 0);
            }

            close(new_socket);
            exit(0);

        } else {
            close(new_socket);

            int fd[2];
            pipe(fd);
            write(fd[1], inet_ntoa(address.sin_addr), strlen(inet_ntoa(address.sin_addr)));
            close(fd[1]);

            char ip_address[INET_ADDRSTRLEN];
            read(fd[0], ip_address, INET_ADDRSTRLEN);
            printf("IP del cliente: %s\n", ip_address);
            close(fd[0]);
        }
        printf("Conexión cerrada\n");
    }

    return 0;
}
