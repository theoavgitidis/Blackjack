#pragma once

#include "Deck.h"
#include "Hand.h"

#include <string>

class Participant {
public:
    explicit Participant(std::string name);
    virtual ~Participant() = default;

    const std::string& name() const noexcept;
    const Hand& hand() const noexcept;
    Hand& hand() noexcept;

    void clearHand();
    void takeCard(Deck& deck);

    int score() const noexcept;
    bool isBust() const noexcept;
    bool hasBlackjack() const noexcept;

private:
    std::string name_;
    Hand hand_;
};

