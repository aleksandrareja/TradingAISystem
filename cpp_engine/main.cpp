#include <iostream>
#include <unordered_map>
#include "Order.h"
#include "Portfolio.h"


int main() {
    Portfolio portfolio(10000);

    std::unordered_map<std::string, double> market_prices={
            {"BTCUSDT", 60000.0},
            {"ETHUSDT", 3000.0}
    };

    std::cout << "Stan poczatkowy:\n";
    portfolio.printStatus(market_prices);

    Order buyBTC = {OrderType::BUY, "BTCUSDT", 0.1, 60000.0};
    portfolio.executeOrder(buyBTC);

    Order buyETH = {OrderType::BUY, "ETHUSDT", 2.0, 3000.0};
    portfolio.executeOrder(buyETH);

    portfolio.printStatus(market_prices);

    std::cout << ">>> RYNEK SIE ZMIENIA <<<\n";
    market_prices["BTCUSDT"] = 65000.0;
    market_prices["ETHUSDT"] = 2800.0;

    portfolio.printStatus(market_prices);

    Order sellBTC = {OrderType::SELL, "BTCUSDT", 0.1, 65000.0};
    portfolio.executeOrder(sellBTC);

    portfolio.printStatus(market_prices);

    return 0;
}