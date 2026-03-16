//
// Created by Ola on 16.03.2026.
//

#include "SignalReceiver.h"
#include <iostream>
#include <sstream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

SignalReceiver::SignalReceiver(Portfolio& portf, int p)
        : portfolio(portf), port(p), running(false) {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
}

SignalReceiver::~SignalReceiver() {
    stop();
    WSACleanup();
}

void SignalReceiver::start() {
    running = true;
    network_thread = std::thread(&SignalReceiver::runServer, this);
    std::cout << "[Siec] Uruchomiono nasluchiwanie na porcie " << port << " w tle.\n";
}

void SignalReceiver::stop() {
    running = false;
    if (network_thread.joinable()) {
        network_thread.join();
    }
}

void SignalReceiver::processMessage(const std::string& msg) {
    // Spodziewamy się tekstu w formacie: "BUY BTCUSDT 0.5 60000"
    std::istringstream iss(msg);
    std::string type_str, symbol;
    double quantity, price;

    if (iss >> type_str >> symbol >> quantity >> price) {
        OrderType type = (type_str == "BUY") ? OrderType::BUY : OrderType::SELL;
        Order order{type, symbol, quantity, price};

        std::cout << "\n[Siec] Otrzymano zewnetrzny sygnal gieldowy!\n";
        portfolio.executeOrder(order);
    } else {
        std::cerr << "[Siec] Blad parsowania sygnalu: " << msg << "\n";
    }
}

void SignalReceiver::runServer() {
    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);

    while (running) {
        SOCKET client_socket = accept(server_fd, nullptr, nullptr);
        if (client_socket != INVALID_SOCKET) {
            char buffer[256] = {0};
            int bytes_received = recv(client_socket, buffer, 256, 0);

            if (bytes_received > 0) {
                std::string message(buffer, bytes_received);
                processMessage(message);
            }
            // Zamykamy klienta i wracamy do nasłuchiwania kolejnych
            closesocket(client_socket);
        }
    }
    closesocket(server_fd);
}