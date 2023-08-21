#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORTNO 5656
#define MAX_CLIENTS 5

void sortArray(int arr[], int size) {
    int temp;
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            if (arr[i] > arr[j]) {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

void handleClient(int client_socket) {
    int arr_size, process_id;
    int arr[100]; // Assuming a maximum of 100 integers in the array

    // Receive array size and array data from the client
    recv(client_socket, &arr_size, sizeof(arr_size), 0);
    recv(client_socket, arr, sizeof(arr[0]) * arr_size, 0);

    // Get the process ID
    process_id = getpid();

    // Sort the array
    sortArray(arr, arr_size);

    // Send the sorted array and process ID back to the client
    send(client_socket, &process_id, sizeof(process_id), 0);
    send(client_socket, arr, sizeof(arr[0]) * arr_size, 0);

    close(client_socket);
}

int main() {
    int sock_id, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    sock_id = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_id == -1) {
        perror("Socket creation error");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORTNO);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock_id, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Binding error");
        exit(1);
    }

    if (listen(sock_id, MAX_CLIENTS) == -1) {
        perror("Listening error");
        exit(1);
    }

    printf("Server is listening...\n");

    while (1) {
        client_sock = accept(sock_id, (struct sockaddr *)&client_addr, &client_len);
        if (client_sock == -1) {
            perror("Accepting error");
            exit(1);
        }
        printf("Connection accepted from a client\n");

        if (fork() == 0) {
            close(sock_id); //Child process doesn't need the listening socket
            handleClient(client_sock);
            exit(0);
        } else {
            close(client_sock); //Parent process doesn't need this client socket
        }
    }

    return 0;
}