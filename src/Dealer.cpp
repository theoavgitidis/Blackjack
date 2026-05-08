#include "Dealer.h"

namespace {
// The dealer must hit until their score is at least 17.
constexpr int kDealerStandThreshold = 17;
} // anonymous namespace

Dealer::Dealer() : Participant("Dealer") {}

bool Dealer::shouldHit() const noexcept {
    return score() < kDealerStandThreshold;
}
