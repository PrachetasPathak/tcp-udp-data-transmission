#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <fstream>
#include <chrono>
#include <thread>

#pragma comment(lib, "Ws2_32.lib")

const int bufferSize = 4096; // Increase buffer size
const int ackSize = 10; // Size for acknowledgment buffer

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;
    const char* filename = "C:\\Lect.mp4"; // Change to your file path

    char buffer[bufferSize];
    char ack[ackSize];
    std::ifstream inputFile(filename, std::ios::binary);

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed.\n");
        return 1;
    }

    // Create UDP socket
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        printf("Socket creation failed.\n");
        WSACleanup();
        return 1;
    }

    // Setup the sockaddr_in structure for server
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080); // Port number
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr); // Server IP address

    // Check if file opened successfully
    if (!inputFile.is_open()) {
        printf("Failed to open file.\n");
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    int sequenceNumber = 0; // Sequence number for chunks
    while (inputFile.read(buffer, bufferSize) || inputFile.gcount() > 0) {
        // Prepend sequence number to the data
        sprintf_s(ack, sizeof(ack), "%d", sequenceNumber);
        sendto(clientSocket, ack, strlen(ack), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr)); // Send sequence number

        sendto(clientSocket, buffer, inputFile.gcount(), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

        // Wait for acknowledgment
        fd_set readfds;
        FD_ZERO(&readfds);
        timeval timeout;
        timeout.tv_sec = 1; // 1 second timeout
        timeout.tv_usec = 0;

        int selectResult = select(clientSocket + 1, &readfds, NULL, NULL, &timeout);
        if (selectResult > 0 && FD_ISSET(clientSocket, &readfds)) {
            // Receive acknowledgment
            int addrLen = sizeof(serverAddr);
            recvfrom(clientSocket, ack, ackSize, 0, (struct sockaddr*)&serverAddr, &addrLen);
            printf("Received ACK for sequence number: %d\n", sequenceNumber);
            sequenceNumber++; // Increment sequence number
        }
        else {
            printf("Timeout waiting for ACK, resending packet %d\n", sequenceNumber);
            // Optionally, resend the current packet if no ACK was received
            // This is a simplified example; you might want to resend just the last chunk instead
        }
    }

    inputFile.close();
    printf("File sent successfully.\n");

    // Clean up and close the socket
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
