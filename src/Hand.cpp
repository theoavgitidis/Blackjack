#include "Hand.h"

#include <sstream>

void Hand::clear() { cards_.clear(); }

void Hand::addCard(const Card& card) { cards_.push_back(card); }

const std::vector<Card>& Hand::cards() const noexcept { return cards_; }

int Hand::score() const noexcept {
    int total = 0;
    int aces = 0;

    for (const auto& c : cards_) {
        total += c.baseValue();
        if (c.rank() == Card::Rank::Ass) {
            ++aces;
        }
    }

    // Kernlogik Ass: Starte mit 11 und wandle so viele Asse wie nötig zu 1 um, um Bust zu vermeiden.
    while (total > 21 && aces > 0) {
        total -= 10; // 11 -> 1 entspricht -10
        --aces;
    }

    return total;
}

bool Hand::isBust() const noexcept { return score() > 21; }

bool Hand::isBlackjack() const noexcept {
    return cards_.size() == 2 && score() == 21;
}

std::string Hand::toString(bool hideSecondCard) const {
    std::ostringstream oss;
    for (std::size_t i = 0; i < cards_.size(); ++i) {
        if (i > 0) oss << ", ";
        if (hideSecondCard && i == 1) {
            oss << "[verdeckt]";
        } else {
            oss << cards_[i].toString();
        }
    }
    return oss.str();
}

