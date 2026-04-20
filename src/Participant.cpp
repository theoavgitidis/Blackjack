#include "Participant.h"

Participant::Participant(std::string name) : name_(std::move(name)) {}

const std::string& Participant::name() const noexcept { return name_; }

const Hand& Participant::hand() const noexcept { return hand_; }
Hand& Participant::hand() noexcept { return hand_; }

void Participant::clearHand() { hand_.clear(); }

void Participant::takeCard(Deck& deck) {
    hand_.addCard(deck.draw());
}

int Participant::score() const noexcept { return hand_.score(); }
bool Participant::isBust() const noexcept { return hand_.isBust(); }
bool Participant::hasBlackjack() const noexcept { return hand_.isBlackjack(); }

