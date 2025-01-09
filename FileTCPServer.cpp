#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <fstream>

#pragma comment(lib, "Ws2_32.lib")  // Link with the Ws2_32.lib for Winsock functions

int main() {
    WSADATA wsaData; //strct used to store info related winsock
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[1024];  // Buffer to store data received from the client
    int recvLen;  // Variable to store the number of bytes received

    // Initialize Winsock
    // WSAStartup is used to initialize the Winsock library. MAKEWORD(2, 2) specifies using Winsock version 2.2
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Create a TCP socket
    // AF_INET is for IPv4, SOCK_STREAM specifies the use of TCP, and protocol is set to 0 for default
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Prepare the sockaddr_in structure for binding the socket
    // sin_family = AF_INET specifies the use of IPv4
    // INADDR_ANY allows the socket to accept connections from any network interface
    // sin_port defines the listening port (8080), and htons converts it to network byte order
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;  // Accept connections from any IP address
    serverAddr.sin_port = htons(8080);  // Listening port

    // Bind the socket to the local address and port
    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    // Start listening for incoming connections
    // listen() puts the socket into listening mode, with a backlog of 1 (number of clients that can be queued)
    listen(serverSocket, 1);
    std::cout << "Waiting for a connection..." << std::endl;

    // Accept a client connection
    int addrLen = sizeof(clientAddr);  // Length of client address
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
    if (clientSocket == INVALID_SOCKET) {
        // If accept() fails, print an error message and clean up
        std::cerr << "Accept failed." << std::endl;
        closesocket(serverSocket);  // Close the server socket
        WSACleanup();  // Clean up Winsock resources
        return 1;
    }

    std::cout << "Connection accepted." << std::endl;

    // Open the output file in binary mode to save the data received from the client
    // std::ios::binary ensures the file is written in binary format (important for non-text files like mp4)
    std::ofstream outputFile("receiv_Lect.mp4", std::ios::binary);
    // Optional: You can use another file, e.g. "received_upgrade.txt"
    // std::ofstream outputFile("received_upgrade.txt", std::ios::binary);

    // Check if the file was opened successfully
    if (!outputFile) {
        // If the file can't be opened, print an error and clean up
        std::cerr << "Failed to open output file." << std::endl;
        closesocket(clientSocket);  // Close the client socket
        closesocket(serverSocket);  // Close the server socket
        WSACleanup();  // Clean up Winsock resources
        return 1;
    }

    // Receive data from the client in chunks
    // recv() is used to receive data from the client; it returns the number of bytes received
    // The loop continues as long as recv() returns a positive number of bytes
    while ((recvLen = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        // Write the received data into the file
        outputFile.write(buffer, recvLen);
        std::cout << "Received " << recvLen << " bytes." << std::endl;  // Print the number of bytes received
    }

    // Close the file and the sockets after the data transfer is complete
    outputFile.close();  // Close the output file
    closesocket(clientSocket);  // Close the client socket
    closesocket(serverSocket);  // Close the server socket
    WSACleanup();  // Clean up Winsock resources

    std::cout << "File saved successfully." << std::endl;  // Print success message
    return 0;
}
