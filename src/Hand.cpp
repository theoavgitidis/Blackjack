#include "Hand.h"

#include <sstream>

namespace {
constexpr int kMaxValidScore = 21;
constexpr int kAceValueDifference = 10;
constexpr std::size_t kBlackjackCardCount = 2;
} // anonymous namespace

void Hand::clear() { cards_.clear(); }

void Hand::addCard(const Card& card) { cards_.push_back(card); }

const std::vector<Card>& Hand::cards() const noexcept { return cards_; }

int Hand::score() const noexcept {
    int total = 0;
    int aces = 0;

    for (const auto& card : cards_) {
        total += card.baseValue();
        if (card.rank() == Card::Rank::Ass) {
            ++aces;
        }
    }

    // Core Ace logic: Start with 11 and convert as many Aces as necessary to 1 to avoid a bust.
    while (total > kMaxValidScore && aces > 0) {
        total -= kAceValueDifference; // 11 -> 1 corresponds to -10
        --aces;
    }

    return total;
}

bool Hand::isBust() const noexcept { return score() > kMaxValidScore; }

bool Hand::isBlackjack() const noexcept {
    return cards_.size() == kBlackjackCardCount && score() == kMaxValidScore;
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
