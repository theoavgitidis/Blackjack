#include "Player.h"

namespace {
// The bankroll cannot drop below zero.
constexpr long long kMinimumBankroll = 0;
} // anonymous namespace

Player::Player() : Participant("Spieler") {}

long long Player::bankrollCents() const noexcept { return bankrollCents_; }

bool Player::canAfford(long long betCents) const noexcept {
    return betCents > 0 && betCents <= bankrollCents_;
}

void Player::adjustBankroll(long long deltaCents) noexcept {
    bankrollCents_ += deltaCents;
    if (bankrollCents_ < kMinimumBankroll) {
        bankrollCents_ = kMinimumBankroll;
    }
}
