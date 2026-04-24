#pragma once

#include "Participant.h"

class Player final : public Participant {
public:
    Player();

    long long bankrollCents() const noexcept;
    bool canAfford(long long betCents) const noexcept;
    void adjustBankroll(long long deltaCents) noexcept;

private:
    // Geld wird in Cent gespeichert, um Rundungsfehler zu vermeiden.
    long long bankrollCents_ = 50000; // 500.00 EUR Startkapital
};

