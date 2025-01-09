#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <fstream>

#pragma comment(lib, "Ws2_32.lib")  // Link with Ws2_32.lib for Winsock functions

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024];  // Buffer to hold the data read from the file

    // Initialize Winsock library (required for using sockets in Windows)
    // MAKEWORD(2, 2) specifies the Winsock version 2.2
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Create a TCP socket
    // AF_INET specifies IPv4, SOCK_STREAM for TCP, protocol set to 0 (default)
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Prepare the sockaddr_in structure for connecting to the server
    // sin_family = AF_INET specifies IPv4
    // sin_port sets the port number to 8080, and htons converts it to network byte order
    // inet_pton converts the IP address "127.0.0.1" (localhost) into network format
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);  // Port number
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);  // Server IP address
    
    //Internet Protocol – Presentation to Numeric.
    //This stands for Host to Network Short. It converts the port number (which is typically in "host byte order") to "network byte order."
   // Host byte order and network byte order refer to the byte arrangement(endian - ness) on different machines :
    // Connect to the server using the prepared address and port
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        // If the connection fails, print an error message and clean up
        std::cerr << "Connection to server failed." << std::endl;
        closesocket(clientSocket);  // Close the socket
        WSACleanup();  // Clean up Winsock resources
        return 1;
    }

    // Open the file in binary mode for reading
    // std::ios::binary ensures the file is read as binary (necessary for non-text files like mp4)
    std::ifstream inputFile("C:\\Lect.mp4", std::ios::binary);
    // Optionally, you can use another file, e.g. "upgrade.txt" instead of "Lect.mp4"
    // std::ifstream inputFile("C:\\upgrade.txt", std::ios::binary);

    // Check if the file was opened successfully
    if (!inputFile) {
        // If file opening fails, print an error and clean up
        std::cerr << "Failed to open input file." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Read from the file and send data to the server in chunks of buffer size (1024 bytes)
    // inputFile.read(buffer, sizeof(buffer)) reads from the file into buffer
    while (inputFile.read(buffer, sizeof(buffer))) {
        int bytesRead = inputFile.gcount();  // Get the actual number of bytes read
        send(clientSocket, buffer, bytesRead, 0);  // Send the read data to the server
    }

    // If there are remaining bytes after the last full read, send them
    if (inputFile.gcount() > 0) {
        send(clientSocket, buffer, inputFile.gcount(), 0);
    }

    // Close the file and the socket after the data is sent
    inputFile.close();  // Close the file
    closesocket(clientSocket);  // Close the client socket
    WSACleanup();  // Clean up Winsock resources

    std::cout << "File sent successfully." << std::endl;  // Print success message
    return 0;
}
