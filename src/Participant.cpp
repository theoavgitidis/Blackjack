#include "Participant.h"

#include <utility>

Participant::Participant(std::string name) : name_(std::move(name)) {}

const std::string& Participant::name() const noexcept { return name_; }

// Hand accessors
const Hand& Participant::hand() const noexcept { return hand_; }
Hand& Participant::hand() noexcept { return hand_; }

// Hand manipulation
void Participant::clearHand() { hand_.clear(); }

void Participant::takeCard(Deck& deck) {
    hand_.addCard(deck.draw());
}

// Status queries delegated to the Hand class
int Participant::score() const noexcept { return hand_.score(); }
bool Participant::isBust() const noexcept { return hand_.isBust(); }
bool Participant::hasBlackjack() const noexcept { return hand_.isBlackjack(); }
