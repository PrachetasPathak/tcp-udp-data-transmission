#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <fstream>

#pragma comment(lib, "Ws2_32.lib")

int main() {
    WSADATA wsaData;
    SOCKET serverSocket;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[1024];
    int addrLen = sizeof(clientAddr);

    // File to save received data
    const char* filename = "C:\\received_file.mp4"; // Change as necessary
    std::ofstream outputFile(filename, std::ios::binary);

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed.\n");
        return 1;
    }

    // Create UDP socket
    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        printf("Socket creation failed.\n");
        WSACleanup();
        return 1;
    }

    // Setup the sockaddr_in structure for server
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080); // Port number
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces

    // Bind the socket
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Bind failed.\n");
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    printf("Waiting for data...\n");

    // Receive data in a loop
    while (true) {
        int bytesReceived = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddr, &addrLen);
        if (bytesReceived > 0) {
            outputFile.write(buffer, bytesReceived);
            printf("Received %d bytes.\n", bytesReceived);
        }
        else {
            printf("No data received or error occurred.\n");
            break;
        }
    }

    outputFile.close();
    printf("File received successfully.\n");

    // Clean up and close the socket
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
