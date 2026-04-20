#pragma once

#include "Card.h"

#include <random>
#include <vector>

class Deck {
public:
    Deck();                 // erstellt ein Standarddeck (52 Karten)
    void shuffle();         // mischt die Karten
    Card draw();            // zieht eine Karte vom Stapel
    std::size_t size() const noexcept;

private:
    std::vector<Card> cards_;
    std::mt19937 rng_;
};

