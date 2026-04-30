#pragma once

#include "ConsoleUI.h"
#include "Dealer.h"
#include "Deck.h"
#include "Player.h"

class Game {
public:
    Game();
    void run();

#ifdef BLACKJACK_UNIT_TEST
public:
#else
private:
#endif
    enum class RoundOutcome { PlayerWin, DealerWin, Push };

    void ensureDeckReady();
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
    Deck deck_;
    static constexpr std::size_t reshuffleWhenAtOrBelow_ = 10;
};
