#include "Deck.h"

#include <algorithm>
#include <stdexcept>

Deck::Deck() : rng_(std::random_device{}()) {
    cards_.reserve(52);
    const Card::Suit suits[] = {Card::Suit::Kreuz, Card::Suit::Pik, Card::Suit::Herz, Card::Suit::Karo};
    const Card::Rank ranks[] = {
        Card::Rank::Zwei, Card::Rank::Drei, Card::Rank::Vier, Card::Rank::Fuenf, Card::Rank::Sechs,
        Card::Rank::Sieben, Card::Rank::Acht, Card::Rank::Neun, Card::Rank::Zehn,
        Card::Rank::Bube, Card::Rank::Dame, Card::Rank::Koenig, Card::Rank::Ass
    };

    for (auto s : suits) {
        for (auto r : ranks) {
            cards_.emplace_back(s, r);
        }
    }
}

void Deck::shuffle() {
    std::shuffle(cards_.begin(), cards_.end(), rng_);
}

Card Deck::draw() {
    if (cards_.empty()) {
        throw std::runtime_error("Der Stapel ist leer.");
    }
    Card c = cards_.back();
    cards_.pop_back();
    return c;
}

std::size_t Deck::size() const noexcept { return cards_.size(); }

