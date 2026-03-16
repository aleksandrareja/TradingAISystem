//
// Created by Ola on 16.03.2026.
//

#include "Portfolio.h"
#include <iostream>
#include <iomanip>

Portfolio::Portfolio(double initial_cash) : cash_balance(initial_cash) {}

bool Portfolio::executeOrder(const Order& order) {
    std::lock_guard<std::mutex> lock(mtx);

    double total_value = order.quantity * order.price;

    if (order.type == OrderType::BUY) {
        if (cash_balance < total_value) {
            std::cerr << "[ODRZUCONO] Brak srodkow na zakup " << order.symbol << ".\n";
            return false;
        }
        cash_balance -= total_value;
        holdings[order.symbol] += order.quantity;
        std::cout << "[KUPIONO] " << order.quantity << " " << order.symbol << " po cenie $" << order.price << ".\n";
    }
    else if (order.type == OrderType::SELL) {
        if (holdings[order.symbol] < order.quantity) {
            std::cerr << "[ODRZUCONO] Niewystarczajaca ilosc " << order.symbol << " do sprzedazy.\n";
            return false;
        }
        cash_balance += total_value;
        holdings[order.symbol] -= order.quantity;
        std::cout << "[SPRZEDANO] " << order.quantity << " " << order.symbol << " po cenie $" << order.price << ".\n";
    }
    return true;
}

double Portfolio::calculateTotalValue(const std::unordered_map<std::string, double>& current_prices) const {
    double total = cash_balance;
    for (const auto& [symbol, quantity] : holdings) {
        auto it = current_prices.find(symbol);
        if (it != current_prices.end()) {
            total += quantity * it->second;
        } else {
            std::cerr << "[OSTRZEZENIE] Brak aktualnej ceny dla " << symbol << "!\n";
        }
    }
    return total;
}

void Portfolio::printStatus(const std::unordered_map<std::string, double>& current_prices) const {
    std::lock_guard<std::mutex> lock(mtx);

    std::cout << "\n--- STATUS PORTFELA ---\n";
    std::cout << "Gotowka: $" << std::fixed << std::setprecision(2) << cash_balance << "\n";
    for (const auto& [symbol, quantity] : holdings) {
        if (quantity > 0) {
            std::cout << "Aktywa [" << symbol << "]: " << quantity << " szt.\n";
        }
    }
    std::cout << "Calkowita wartosc: $" << calculateTotalValue(current_prices) << "\n";
    std::cout << "-----------------------\n\n";
}