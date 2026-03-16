//
// Created by Ola on 16.03.2026.
//

#ifndef HTTPSERVER_ORDER_H
#define HTTPSERVER_ORDER_H

#pragma once

#include <string>

enum class OrderType {
    BUY,
    SELL
};

struct Order {
    OrderType type;
    std::string symbol;
    double quantity;
    double price;
};

#endif //HTTPSERVER_ORDER_H
