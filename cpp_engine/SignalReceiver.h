//
// Created by Ola on 16.03.2026.
//

#ifndef HTTPSERVER_SIGNALRECEIVER_H
#define HTTPSERVER_SIGNALRECEIVER_H


#pragma once

#include "Portfolio.h"
#include <thread>
#include <atomic>
#include <string>

class SignalReceiver {
private:
    Portfolio& portfolio;
    int port;
    std::thread network_thread;
    std::atomic<bool> running;

    void runServer();
    void processMessage(const std::string& msg);

public:
    SignalReceiver(Portfolio& portf, int p);
    ~SignalReceiver();

    void start();
    void stop();
};


#endif //HTTPSERVER_SIGNALRECEIVER_H
