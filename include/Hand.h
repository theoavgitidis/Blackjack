#pragma once

#include "Card.h"

#include <string>
#include <vector>

class Hand {
public:
    void clear();
    void addCard(const Card& card);
    const std::vector<Card>& cards() const noexcept;

    int score() const noexcept;
    bool isBust() const noexcept;
    bool isBlackjack() const noexcept; // genau 2 Karten und Score==21

    std::string toString(bool hideSecondCard) const;

private:
    std::vector<Card> cards_;
};

