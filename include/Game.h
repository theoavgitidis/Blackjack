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
    bool handleInitialBlackjack();
    void playerTurn(Deck& deck);
    void dealerTurn(Deck& deck);
    RoundOutcome determineOutcome() const;

    Player player_;
    Dealer dealer_;
    ConsoleUI ui_;
};

