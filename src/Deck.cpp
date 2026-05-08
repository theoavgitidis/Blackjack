#include "Deck.h"

#include <algorithm>
#include <stdexcept>

namespace {
    // Standard number of cards in a Blackjack deck.
    constexpr std::size_t kStandardDeckSize = 52;
} // anonymous namespace

Deck::Deck() : rng_(std::random_device{}()) { // Initializes random number generator
    cards_.reserve(kStandardDeckSize);
    // Note: Card enum names (Kreuz, Pik, etc.) are kept in German as they are defined in Card.h.
    const Card::Suit suits[] = {Card::Suit::Kreuz, Card::Suit::Pik, Card::Suit::Herz, Card::Suit::Karo}; // Array of all suits
    const Card::Rank ranks[] = { // Array of all ranks
        Card::Rank::Zwei, Card::Rank::Drei, Card::Rank::Vier, Card::Rank::Fuenf, Card::Rank::Sechs,
        Card::Rank::Sieben, Card::Rank::Acht, Card::Rank::Neun, Card::Rank::Zehn,
        Card::Rank::Bube, Card::Rank::Dame, Card::Rank::Koenig, Card::Rank::Ass
    };

    for (auto s : suits) {
        for (auto r : ranks) {
            cards_.emplace_back(s, r); // Add each card to the deck
        }
    }
}

// Shuffles the cards in the deck using the random number generator.
void Deck::shuffle() {
    std::shuffle(cards_.begin(), cards_.end(), rng_);
}

// Draws the top card from the deck. Throws an error if the deck is empty.
Card Deck::draw() {
    if (cards_.empty()) {
        throw std::runtime_error("Cannot draw from an empty deck.");
    }
    Card drawnCard = cards_.back(); // Get the last card
    cards_.pop_back();              // Remove the last card
    return drawnCard;               // Return the drawn card
}

// Returns the current number of cards in the deck.
std::size_t Deck::size() const noexcept { return cards_.size(); }
