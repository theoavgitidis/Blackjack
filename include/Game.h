#pragma once

#include "ConsoleUI.h"
#include "Dealer.h"
#include "Deck.h"
#include "Player.h"

class Game {
public:
    Game();
    void run();

private:
    enum class RoundOutcome { PlayerWin, DealerWin, Push };

    void playRound();
    void initialDeal(Deck& deck);
    bool handleInitialBlackjack(long long betCents);
    void playerTurn(Deck& deck, long long& betCents);
    void dealerTurn(Deck& deck);
    RoundOutcome determineOutcome() const;
    void applyPayout(long long betCents, bool playerBlackjack, RoundOutcome outcome);

    Player player_;
    Dealer dealer_;
    ConsoleUI ui_;
};

