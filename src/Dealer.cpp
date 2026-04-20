#include "Dealer.h"

Dealer::Dealer() : Participant("Dealer") {}

bool Dealer::shouldHit() const noexcept {
    return score() < 17;
}

