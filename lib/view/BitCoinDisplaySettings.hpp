#ifndef BITCOIN_DISPLAY_SETTINGS_HPP
#define BITCOIN_DISPLAY_SETTINGS_HPP

#include "Fiat.hpp"

// Manages how an amount of bitcoin should be displayed, for now in text
class BitCoinDisplaySettings
{
public:

    enum class Currency {
        Fiat,
        BitCoin
    };

    // Constructor for showing BitCoin
    BitCoinDisplaySettings(int precision = 1);

    // Constructor for fiat
    BitCoinDisplaySettings(Fiat fiat, float rate, int precision = 1);

    // Getters and setters
    Currency currency() const;
    void setCurrency(Currency currency);

    Fiat fiat() const;
    void setFiat(Fiat fiat);

    float rate() const;
    void setRate(float rate);

    int precision() const;
    void setPrecision(int value);

private:

    // The currency of use
    Currency _currency;

    // If _currency == Currency::Fiat,
    // then use this fiat currency
    Fiat _fiat;

    // Exchange rate for fiat to btc: how many $ to buy 1 btc
    float _rate;

    // Precision on bitcoin representation
    int _precision;
};

#endif // BITCOIN_DISPLAY_SETTINGS_HPP
