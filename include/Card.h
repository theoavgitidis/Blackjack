#pragma once

#include <string>

class Card {
public:
    enum class Suit { Kreuz, Pik, Herz, Karo };
    enum class Rank {
        Zwei = 2, Drei, Vier, Fuenf, Sechs, Sieben, Acht, Neun, Zehn,
        Bube, Dame, Koenig, Ass
    };

    Card(Suit suit, Rank rank);

    Suit suit() const noexcept;
    Rank rank() const noexcept;

    // Blackjack-Wert: 2-10 face value, Bildkarten=10, Ass initial=11 (wird in Hand angepasst).
    int baseValue() const noexcept;
    std::string toString() const;

private:
    Suit suit_;
    Rank rank_;
};

