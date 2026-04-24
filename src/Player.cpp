#include "Player.h"

Player::Player() : Participant("Spieler") {}

long long Player::bankrollCents() const noexcept { return bankrollCents_; }

bool Player::canAfford(long long betCents) const noexcept {
    return betCents > 0 && betCents <= bankrollCents_;
}

void Player::adjustBankroll(long long deltaCents) noexcept {
    bankrollCents_ += deltaCents;
    if (bankrollCents_ < 0) bankrollCents_ = 0;
}

