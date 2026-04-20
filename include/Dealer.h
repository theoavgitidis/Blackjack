#pragma once

#include "Participant.h"

class Dealer final : public Participant {
public:
    Dealer();

    // Standardregel V1: Dealer zieht bis inkl. 16, steht ab 17.
    bool shouldHit() const noexcept;
};

