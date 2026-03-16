//
// Created by Ola on 16.03.2026.
//

#ifndef HTTPSERVER_PORTFOLIO_H
#define HTTPSERVER_PORTFOLIO_H

#pragma once

#include <string>
#include <unordered_map>
#include "Order.h"

class Portfolio {
private:
    double cash_balance;
    std::unordered_map<std::string, double> holdings;

public:
    explicit Portfolio(double initial_cash);

    bool executeOrder(const Order& order);
    double calculateTotalValue(const std::unordered_map<std::string, double>& current_prices) const;
    void printStatus(const std::unordered_map<std::string, double>& current_prices) const;
};


#endif //HTTPSERVER_PORTFOLIO_H
