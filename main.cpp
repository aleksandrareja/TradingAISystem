#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>

// Pragma for MSVC compiler to link the library in case of CMake issues
#pragma comment(lib, "Ws2_32.lib")

int main() {
    // 0. Initialize Winsock (Required ONLY on Windows)
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error: Winsock initialization failed.\n";
        return 1;
    }

    // 1. Create a socket (On Windows, we use the SOCKET type instead of int)
    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd == INVALID_SOCKET) {
        std::cerr << "Error: Failed to create socket.\n";
        WSACleanup();
        return 1;
    }

    // 2. Configure address and port
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    // 3. Bind the socket to the port
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        std::cerr << "Error: Failed to bind to port 8080.\n";
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    // 4. Start listening for incoming connections
    if (listen(server_fd, 3) == SOCKET_ERROR) {
        std::cerr << "Error: Listen failed.\n";
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    std::cout << "Server started. Waiting for connections on port 8080...\n";

    // 5. Accept an incoming connection
    int addrlen = sizeof(address);
    SOCKET new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
    if (new_socket == INVALID_SOCKET) {
        std::cerr << "Error: Failed to accept connection.\n";
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    std::cout << "SUCCESS: Connection established with a client!\n\n";

    // 6. Receive data (Recv)
    char buffer[1024] = {0};
    recv(new_socket, buffer, 1024, 0);
    std::cout << "--- Client sent: ---\n" << buffer << "--------------------\n";

    // 7. Send the response
    const char* response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 5\r\n\r\nHello";
    send(new_socket, response, strlen(response), 0);
    std::cout << "Response 'Hello' sent to the client.\n";

    // 8. Close the connection and clean up (closesocket instead of close)
    closesocket(new_socket);
    closesocket(server_fd);
    WSACleanup(); // Shut down the Windows networking subsystem.

    return 0;
}